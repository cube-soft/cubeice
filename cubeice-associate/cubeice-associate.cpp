#include <string>
#include <windows.h>
#include <tchar.h>
#include <cubeice/user-setting.h>

/* ------------------------------------------------------------------------- */
/*
 *  RegDeleteKeyNT
 *
 *  see also: http://support.microsoft.com/kb/142491/ja
 */
/* ------------------------------------------------------------------------- */
inline DWORD RegDeleteKeyNT(HKEY hStartKey , LPCTSTR pKeyName ){
	static const int max_keylen = 2048;
	DWORD   dwRtn, dwSubKeyLength;
	LPTSTR  pSubKey = NULL;
	TCHAR   szSubKey[max_keylen] = {};
	HKEY    hKey;
	
	// Do not allow NULL or empty key name
	if ( pKeyName &&  lstrlen(pKeyName)) {
		if( (dwRtn=RegOpenKeyEx(hStartKey,pKeyName, 0, KEY_ENUMERATE_SUB_KEYS | DELETE, &hKey )) == ERROR_SUCCESS ) {
			while (dwRtn == ERROR_SUCCESS ) {
				dwSubKeyLength = max_keylen;
				dwRtn = RegEnumKeyEx(
							hKey,
							0,       // always index zero
							szSubKey,
							&dwSubKeyLength,
							NULL,
							NULL,
							NULL,
							NULL
						);
				
				if(dwRtn == ERROR_NO_MORE_ITEMS) {
					dwRtn = RegDeleteKey(hStartKey, pKeyName);
					break;
				}
				else if(dwRtn == ERROR_SUCCESS) dwRtn=RegDeleteKeyNT(hKey, szSubKey);
			}
			RegCloseKey(hKey);
			// Do not save return code because error
			// has already occurred
		}
	}
	else dwRtn = ERROR_BADKEY;
	
	return dwRtn;
}

/* ------------------------------------------------------------------------- */
//  associate
/* ------------------------------------------------------------------------- */
void associate(const std::basic_string<TCHAR>& key, const std::basic_string<TCHAR>& value, bool flag) {
	static const TCHAR* clsid_associate = _T("{F3DB85F4-4731-4e80-BC2E-754A7320D830}");
	static const TCHAR* clsid_tooltip = _T("{00021500-0000-0000-C000-000000000046}");
	HKEY subkey;
	LONG status = RegOpenKeyEx(HKEY_CLASSES_ROOT, ( key + _T( "\\shellex" ) ).c_str(), 0, KEY_ALL_ACCESS, &subkey );
	
	if( !status ) {
		RegDeleteKeyNT( subkey, clsid_tooltip );
		RegCloseKey( subkey );
	}
	
	if (flag) {
		DWORD disposition = 0;
		LONG status = RegCreateKeyEx(HKEY_CLASSES_ROOT, key.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &subkey, &disposition);
		if (!status) { 
			TCHAR buffer[32] = {};
			DWORD type = 0;
			DWORD size = sizeof(buffer);
			if (RegQueryValueEx(subkey, _T(""), NULL, &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS && std::basic_string<TCHAR>(buffer) != value) {
				RegSetValueEx(subkey, CUBEICE_REG_PREVARCHIVER, 0, REG_SZ, (CONST BYTE*)buffer, (_tcslen(buffer) + 1) * sizeof(TCHAR));
			}
			RegSetValueEx(subkey, _T(""), 0, REG_SZ, (CONST BYTE*)value.c_str(), (value.size() + 1) * sizeof(TCHAR));
			
			disposition = 0;
			status = RegCreateKeyEx(HKEY_CLASSES_ROOT, ( key + _T( "\\shellex\\") + clsid_tooltip ).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &subkey, &disposition);
			if (!status) {
				RegSetValueEx(subkey, _T(""), 0, REG_SZ, (CONST BYTE*)clsid_associate, (_tcslen(clsid_associate) + 1) * sizeof(TCHAR));
			}
		}
	}
	else {
		LONG status = RegOpenKeyEx(HKEY_CLASSES_ROOT, key.c_str(), 0, KEY_ALL_ACCESS, &subkey);
		if (!status) {
			TCHAR buffer[32] = {};
			DWORD type = 0;
			DWORD size = sizeof(buffer);
			if (RegQueryValueEx(subkey, _T(""), NULL, &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS && std::basic_string<TCHAR>(buffer) == value) {
				TCHAR prev[32] = {};
				type = 0;
				size = sizeof(prev);
				if (RegQueryValueEx(subkey, CUBEICE_REG_PREVARCHIVER, NULL, &type, (LPBYTE)prev, &size) == ERROR_SUCCESS) {
					RegSetValueEx(subkey, _T(""), 0, REG_SZ, (CONST BYTE*)prev, (_tcslen(prev) + 1) * sizeof(TCHAR));
				}
				else {
					RegCloseKey(subkey);
					RegDeleteKeyNT(HKEY_CLASSES_ROOT, key.c_str());
				}
			}
		}
	}
}

int _tmain(int argc, TCHAR* argv[]) {
	if (argc < 2) return -1;
	
	int flags = _ttoi(argv[1]);
	const cubeice::detail::ext_map& exts = cubeice::detail::extensions();
	for (cubeice::detail::ext_map::const_iterator pos = exts.begin(); pos != exts.end(); pos++) {
		associate(pos->first, pos->second.first, ((flags & pos->second.second) != 0));
	}
	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_FLUSH,0,0);

	return 0;
}