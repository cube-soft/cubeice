#include <Windows.h>
#include <shlwapi.h>
#include <initguid.h>
#include "cubeice-comment.h"
#include "..\cubeice-7z\CPP\7zip\Archive\IArchive.h"
#include "..\cubeice-7z\CPP\Common\MyCom.h"
#include "..\cubeice-7z\CPP\7zip\Common\FileStreams.h"
#include "..\cubeice-7z\CPP\7zip\IPassword.h"
#include "..\cubeice-7z\CPP\Windows\PropVariant.h"


#define		SEVENZIP_DLL_PATH		TEXT("7z.dll")


// use another CLSIDs, if you want to support other formats (zip, rar, ...).
// {23170F69-40C1-278A-1000-000110070000}
DEFINE_GUID(CLSID_CFormatZip, 0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x01, 0x00, 0x00);
//DEFINE_GUID(CLSID_CFormat7z, 0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x07, 0x00, 0x00);

extern HINSTANCE	hDllInstance;

typedef	HRESULT (STDMETHODCALLTYPE *CreateObjectFunc)(const GUID *clsid, const GUID *iid, void **outObject);

std::wstring GetArchiveComment(const std::wstring &filepath)
{
	TCHAR						modulePath[10*MAX_PATH];
	std::basic_string<TCHAR>	res = TEXT("");
	HMODULE						hModule;

	GetModuleFileName(hDllInstance, modulePath, sizeof(modulePath) / sizeof(modulePath[0]));
	PathRemoveFileSpec(modulePath);
	PathAppend(modulePath, SEVENZIP_DLL_PATH);
	hModule = LoadLibrary(modulePath);
	if(hModule == NULL)
		return res;

	CreateObjectFunc CreateObject = reinterpret_cast<CreateObjectFunc>(GetProcAddress(hModule, "CreateObject"));
	if(CreateObject) {
		CMyComPtr<IInArchive>	archive;
		if(CreateObject(&CLSID_CFormatZip, &IID_IInArchive, reinterpret_cast<void**>(&archive)) == S_OK) {
			CInFileStream			*fileSpec = new CInFileStream;
			CMyComPtr<IInStream>	file = fileSpec;

			if(fileSpec->Open(filepath.c_str())) {
				class CArchiveOpenCallback : public IArchiveOpenCallback, public CMyUnknownImp {
				public:
					MY_UNKNOWN_IMP

					STDMETHOD(SetTotal)(const UInt64 * /* files */, const UInt64 * /* bytes */) {
						return S_OK;
					}
					STDMETHOD(SetCompleted)(const UInt64 * /* files */, const UInt64 * /* bytes */) {
						return S_OK;
					}
				};

				CArchiveOpenCallback			*openCallbackSpec = new CArchiveOpenCallback;
				CMyComPtr<IArchiveOpenCallback>	openCallback(openCallbackSpec);

				if(archive->Open(file, 0, openCallback) == S_OK) {
					NWindows::NCOM::CPropVariant	prop;
					archive->GetArchiveProperty(kpidComment, &prop);
					if(prop.vt == VT_BSTR)
						res = prop.bstrVal;
				}
			}
		}
	}
	FreeModule(hModule);
	return res;
}
