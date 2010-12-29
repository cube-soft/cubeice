/* ------------------------------------------------------------------------- */
/*
 *  user-setting.h
 *
 *  Copyright (c) 2010 CubeSoft.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    - No names of its contributors may be used to endorse or promote
 *      products derived from this software without specific prior written
 *      permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  Last-modified: Mon 13 Dec 2010 12:31:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_USER_SETTING_H
#define CUBEICE_USER_SETTING_H

#include <tchar.h>
#include <cstdlib>
#include <set>
#include <string>
#include <windows.h>
#include <winreg.h>
#include <shlobj.h>
#include <clx/base64.h>
#include <clx/split.h>

/* ------------------------------------------------------------------------- */
//  関連付けに関連するフラグ
/* ------------------------------------------------------------------------- */
#define ZIP_FLAG            0x00000001
#define LZH_FLAG            0x00000002
#define RAR_FLAG            0x00000004
#define TAR_FLAG            0x00000008
#define GZ_FLAG             0x00000010
#define SEVENZIP_FLAG       0x00000020
#define ARJ_FLAG            0x00000040
#define BZ2_FLAG            0x00000080
#define CAB_FLAG            0x00000100
#define CHM_FLAG            0x00000200
#define CPIO_FLAG           0x00000400
#define DEB_FLAG            0x00000800
#define DMG_FLAG            0x00001000
#define ISO_FLAG            0x00002000
#define RPM_FLAG            0x00004000
#define TBZ_FLAG            0x00008000
#define TGZ_FLAG            0x00010000
#define WIM_FLAG            0x00020000
#define XAR_FLAG            0x00040000
#define XZ_FLAG             0x00080000
#define EXE_FLAG            0x00100000 // 自己解凍形式

/* ------------------------------------------------------------------------- */
//  コンテキストメニューに関連するフラグ
/* ------------------------------------------------------------------------- */
#define COMPRESS_FLAG       0x0001
#define DECOMPRESS_FLAG     0x0002
#define SETTING_FLAG        0x0004

/* ------------------------------------------------------------------------- */
//  出力先フォルダに関連するフラグ
/* ------------------------------------------------------------------------- */
#define OUTPUT_RUNTIME      0x00    // 実行時に指定する
#define OUTPUT_SPECIFIC     0x01    // 指定したフォルダ
#define OUTPUT_SOURCE       0x02    // 元のファイルと同じフォルダ

/* ------------------------------------------------------------------------- */
//  上書きに関連するフラグ
/* ------------------------------------------------------------------------- */
#define OVERWRITE_FORCE     0x00    // 強制的に上書き
#define OVERWRITE_NOTIFY    0x01    // 上書きの確認
#define OVERWRITE_NEWER     0x02    // 更新日時が元のファイルより新しい場合上書き

/* ------------------------------------------------------------------------- */
//  圧縮/解凍タブの詳細設定に関連するフラグ
/* ------------------------------------------------------------------------- */
#define DETAIL_OVERWRITE      0x0001  // 上書きの確認
#define DETAIL_IGNORE_NEWER   0x0002  // 更新日時が新しい場合は無視
#define DETAIL_OPEN           0x0004  // 保存先を開く
#define DETAIL_CREATE_FOLDER  0x0008  // フォルダを作成する
#define DETAIL_CHARCODE       0x0010  // 文字コードを変換する
#define DETAIL_FILTER         0x0020  // フィルタリングを行う
#define DETAIL_SKIP_DESKTOP   0x0040  // デスクトップの場合は開かない

/* ------------------------------------------------------------------------- */
//  レジストリに関する情報
/* ------------------------------------------------------------------------- */
#define CUBEICE_REG_ROOT                _T("Software\\CubeSoft\\CubeICE")
#define CUBEICE_REG_COMPRESS            _T("Compress")
#define CUBEICE_REG_DECOMPRESS          _T("Decompress")
#define CUBEICE_REG_FLAGS               _T("Flags")
#define CUBEICE_REG_DETAILS	            _T("Details")
#define CUBEICE_REG_OUTPUT_CONDITION    _T("OutputCondition")
#define CUBEICE_REG_OUTPUT_PATH         _T("OutputPath")
#define CUBEICE_REG_CONTEXT             _T("ContextFlags")
#define CUBEICE_REG_SHORTCUT            _T("ShortcutFlags")
#define CUBEICE_REG_FILTER              _T("Filter")
#define CUBEICE_REG_INSTALL             _T("InstallDirectory")
#define CUBEICE_REG_VERSION             _T("Version")
#define CUBEICE_REG_PREVARCHIVER        _T("PrevArchiver")

namespace cubeice {
	/* --------------------------------------------------------------------- */
	/*
	 *  archive_setting
	 *
	 *  圧縮/解凍の設定をレジストリへ入出力するためのクラス．
	 */
	/* --------------------------------------------------------------------- */
	class archive_setting {
	public:
		typedef std::size_t size_type;
		typedef TCHAR char_type;
		typedef std::basic_string<char_type> string_type;
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		explicit archive_setting(const string_type& root) :
			root_(root), flags_(0), details_(0), output_condition_(0), output_path_() {
			this->load();
		}
		
		/* ----------------------------------------------------------------- */
		//  load
		/* ----------------------------------------------------------------- */
		void load() {
			HKEY hkResult;
			LONG lResult = RegOpenKeyEx(HKEY_CURRENT_USER, root_.c_str(), 0, KEY_ALL_ACCESS, &hkResult);
			if (!lResult) {
				DWORD dwType;
				DWORD dwSize;
				
				dwSize = sizeof(flags_);
				RegQueryValueEx(hkResult, CUBEICE_REG_FLAGS, NULL, &dwType, (LPBYTE)&flags_, &dwSize);
				
				dwSize = sizeof(details_);
				RegQueryValueEx(hkResult, CUBEICE_REG_DETAILS, NULL, &dwType, (LPBYTE)&details_, &dwSize);

				dwSize = sizeof(output_condition_);
				RegQueryValueEx(hkResult, CUBEICE_REG_OUTPUT_CONDITION, NULL, &dwType, (LPBYTE)&output_condition_, &dwSize);
				
				char_type buffer[1024] = {};
				dwSize = sizeof(buffer);
				if (RegQueryValueEx(hkResult, CUBEICE_REG_OUTPUT_PATH, NULL, &dwType, (LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
					output_path_ = buffer;
				}
			}
		}
		
		/* ----------------------------------------------------------------- */
		//  save
		/* ----------------------------------------------------------------- */
		void save() {
			HKEY hkResult; // キーのハンドル
			DWORD dwDisposition; // 処理結果を受け取る
			LONG lResult; // 関数の戻り値を格納する
			lResult = RegCreateKeyEx(HKEY_CURRENT_USER, root_.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, &dwDisposition);
			if (!lResult) {
				RegSetValueEx(hkResult, CUBEICE_REG_FLAGS, 0, REG_DWORD, (CONST BYTE*)&flags_, sizeof(flags_));
				RegSetValueEx(hkResult, CUBEICE_REG_DETAILS, 0, REG_DWORD, (CONST BYTE*)&details_, sizeof(details_));
				RegSetValueEx(hkResult, CUBEICE_REG_OUTPUT_CONDITION, 0, REG_DWORD, (CONST BYTE*)&output_condition_, sizeof(output_condition_));
				RegSetValueEx(hkResult, CUBEICE_REG_OUTPUT_PATH, 0, REG_SZ, (CONST BYTE*)output_path_.c_str(), output_path_.length() + 1);
			}
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  flags
		 *
		 *  圧縮に対応するファイル群．圧縮はコンテキストメニューの
		 *  「圧縮」のサブ項目に表示させるものになり，
		 *  各フラグ (FLAG_ZIP, ..., FLAG_XZ) の和集合となる．ただし，
		 *  7-zip が圧縮に対応しているもののみ．
		 */
		/* ----------------------------------------------------------------- */
		size_type& flags() { return flags_; }
		const size_type& flags() const { return flags_; }
		
		/* ----------------------------------------------------------------- */
		//  details
		/* ----------------------------------------------------------------- */
		size_type& details() { return details_; }
		const size_type& details() const { return details_; }

		/* ----------------------------------------------------------------- */
		/*
		 *  output_condition
		 *
		 *  出力先フォルダの設定．OUTPUT_SPECIFIC, OUTPUT_SOURCE, 
		 *  OUTPUT_RUNTIME のどれか．
		 */
		/* ----------------------------------------------------------------- */
		size_type& output_condition() { return output_condition_; }
		const size_type& output_condition() const { return output_condition_; }

		/* ----------------------------------------------------------------- */
		/*
		 *  output_path
		 *
		 *  output_condition が OUTPUT_SPECIFIC の場合，path_compress
		 *  の値が有効となる．
		 */
		/* ----------------------------------------------------------------- */
		string_type& output_path() { return output_path_; }
		const string_type& output_path() const { return output_path_; }
		
	private:
		string_type root_;
		size_type flags_;
		size_type details_;
		size_type output_condition_;
		string_type output_path_;
	};
	
	/* --------------------------------------------------------------------- */
	//  user_setting
	/* --------------------------------------------------------------------- */
	class user_setting {
	public:
		typedef archive_setting archive_type;
		typedef archive_type::size_type size_type;
		typedef archive_type::char_type char_type;
		typedef archive_type::string_type string_type;
		typedef std::set<string_type> container_type;
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		user_setting() :
			root_(CUBEICE_REG_ROOT),
			comp_(string_type(CUBEICE_REG_ROOT) + _T('\\') + CUBEICE_REG_COMPRESS),
			decomp_(string_type(CUBEICE_REG_ROOT) + _T('\\') + CUBEICE_REG_DECOMPRESS),
			ctx_flags_(0), sc_flags_(0), filters_() {
			this->load();	
		}
		
		explicit user_setting(const string_type& root) :
			root_(root),
			comp_(root + _T('\\') + CUBEICE_REG_COMPRESS),
			decomp_(root + _T('\\') + CUBEICE_REG_DECOMPRESS),
			ctx_flags_(0), sc_flags_(0), filters_() {
			this->load();
		}
		
		/* ----------------------------------------------------------------- */
		//  load
		/* ----------------------------------------------------------------- */
		void load() {
			comp_.load();
			decomp_.load();
			
			HKEY hkResult;
			LONG lResult = RegOpenKeyEx(HKEY_CURRENT_USER, root_.c_str(), 0, KEY_ALL_ACCESS, &hkResult);
			if (!lResult) {
				DWORD dwType;
				DWORD dwSize;
				
				dwSize = sizeof(ctx_flags_);
				RegQueryValueEx(hkResult, CUBEICE_REG_CONTEXT, NULL, &dwType, (LPBYTE)&ctx_flags_, &dwSize);
				
#if !defined(_UNICODE) && !defined(UNICODE)
				dwSize = sizeof(sc_flags_);
				RegQueryValueEx(hkResult, CUBEICE_REG_SHORTCUT, NULL, &dwType, (LPBYTE)&sc_flags_, &dwSize);
				
				// フィルタリング文字列の読み込み．型は REG_MULTI_SZ
				// 変数側の型は std::set<std::string>
				char_type buffer[64 * 1024] = {};
				dwSize = sizeof(buffer);
				if (RegQueryValueEx(hkResult, CUBEICE_REG_FILTER, NULL, &dwType, (LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
					string_type encoded(buffer);
					string_type s = clx::base64::decode(encoded);
					filters_.clear();
					clx::split_if(s, filters_, clx::is_any_of(_T("\r\n")));
				}
#endif
			}
		}
		
		/* ----------------------------------------------------------------- */
		//  save
		/* ----------------------------------------------------------------- */
		void save() {
			comp_.save();
			decomp_.save();
			
			HKEY hkResult;
			DWORD dwDisposition;
			LONG lResult = RegCreateKeyEx(HKEY_CURRENT_USER, root_.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, &dwDisposition);
			if (!lResult) {
				RegSetValueEx(hkResult, CUBEICE_REG_CONTEXT, 0, REG_DWORD, (CONST BYTE*)&ctx_flags_, sizeof(ctx_flags_));
				
#if !defined(_UNICODE) && !defined(UNICODE)
				// ショートカットの処理．
				RegSetValueEx(hkResult, CUBEICE_REG_SHORTCUT, 0, REG_DWORD, (CONST BYTE*)&sc_flags_, sizeof(sc_flags_));
				char buffer[2048] ={};
				GetModuleFileNameA(GetModuleHandle(NULL), buffer, 2048);
				std::basic_string<char> tmp = buffer;
				std::basic_string<char> current = tmp.substr(0, tmp.find_last_of('\\'));
				if ((sc_flags_ & COMPRESS_FLAG)) this->create_shortcut(current + "\\cubeice.exe", "/c:zip", "CubeICE 圧縮.lnk", 1);
				else this->remove_shortcut("CubeICE 圧縮.lnk");
				if ((sc_flags_ & DECOMPRESS_FLAG)) this->create_shortcut(current + "\\cubeice.exe", "/x", "CubeICE 解凍.lnk", 2);
				else this->remove_shortcut("CubeICE 解凍.lnk");
				if ((sc_flags_ & SETTING_FLAG)) this->create_shortcut(current + "\\cubeice-setting.exe", "", "CubeICE 設定.lnk", 0);
				else this->remove_shortcut("CubeICE 設定.lnk");
				
				string_type dest;
				clx::join(filters_, dest, _T("\n"));
				string_type encoded = clx::base64::encode(dest);
				RegSetValueEx(hkResult, CUBEICE_REG_FILTER, 0, REG_SZ, (CONST BYTE*)encoded.c_str(), encoded.length() + 1);
#endif
			}
			this->associate(decomp_.flags());
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  compression
		 *
		 *  圧縮に関する設定．
		 */
		/* ----------------------------------------------------------------- */
		archive_type& compression() { return comp_; }
		const archive_type& compression() const { return comp_; }
		
		/* ----------------------------------------------------------------- */
		/*
		 *  decompression
		 *
		 *  解凍に関する設定．
		 */
		/* ----------------------------------------------------------------- */
		archive_type& decompression() { return decomp_; }
		const archive_type& decompression() const { return decomp_; }
		
		/* ----------------------------------------------------------------- */
		/*
		 *  context_flags
		 *
		 *  コンテキストメニューに表示させるもの．
		 */
		/* ----------------------------------------------------------------- */
		size_type& context_flags() { return ctx_flags_; }
		const size_type& context_flags() const { return ctx_flags_; }
		
		/* ----------------------------------------------------------------- */
		/*
		 *  shortcut_flags
		 *
		 *  ショートカットを表示させるかどうか．
		 */
		/* ----------------------------------------------------------------- */
		size_type& shortcut_flags() { return sc_flags_; }
		const size_type& shortcut_flags() const { return sc_flags_; }
		
		/* ----------------------------------------------------------------- */
		//  filters
		/* ----------------------------------------------------------------- */
		container_type& filters() { return filters_; }
		const container_type& filters() const { return filters_; }
		
	private:
		string_type root_;
		archive_type comp_;
		archive_type decomp_;
		size_type ctx_flags_;
		size_type sc_flags_;
		container_type filters_;
		
		/* ----------------------------------------------------------------- */
		/*
		 *  associate
		 *
		 *  NOTE: レジストリに登録する cubeice_* はインストーラを用いて
		 *  あらかじめ登録しておくこと．
		 */
		/* ----------------------------------------------------------------- */
		void associate(size_type flags) {
			this->associate(_T(".zip"), _T("cubeice_zip"), (flags & ZIP_FLAG) != 0);
			this->associate(_T(".lzh"), _T("cubeice_lzh"), (flags & LZH_FLAG) != 0);
			this->associate(_T(".rar"), _T("cubeice_rar"), (flags & RAR_FLAG) != 0);
			this->associate(_T(".tar"), _T("cubeice_tar"), (flags & TAR_FLAG) != 0);
			this->associate(_T(".gz"),  _T("cubeice_gz"),  (flags & GZ_FLAG) != 0);
			this->associate(_T(".7z"),  _T("cubeice_7z"),  (flags & SEVENZIP_FLAG) != 0);
			this->associate(_T(".arj"), _T("cubeice_arj"), (flags & ARJ_FLAG) != 0);
			this->associate(_T(".bz2"), _T("cubeice_bz2"), (flags & BZ2_FLAG) != 0);
			this->associate(_T(".cab"), _T("cubeice_cab"), (flags & CAB_FLAG) != 0);
			this->associate(_T(".chm"), _T("cubeice_chm"), (flags & CHM_FLAG) != 0);
			this->associate(_T(".cpio"),_T("cubeice_cpio"),(flags & CPIO_FLAG) != 0);
			this->associate(_T(".deb"), _T("cubeice_deb"), (flags & DEB_FLAG) != 0);
			this->associate(_T(".dmg"), _T("cubeice_dmg"), (flags & DMG_FLAG) != 0);
			this->associate(_T(".iso"), _T("cubeice_iso"), (flags & ISO_FLAG) != 0);
			this->associate(_T(".rpm"), _T("cubeice_rpm"), (flags & RPM_FLAG) != 0);
			this->associate(_T(".tbz"), _T("cubeice_tbz"), (flags & TBZ_FLAG) != 0);
			this->associate(_T(".tgz"), _T("cubeice_tgz"), (flags & TGZ_FLAG) != 0);
			this->associate(_T(".wim"), _T("cubeice_wim"), (flags & WIM_FLAG) != 0);
			this->associate(_T(".xar"), _T("cubeice_xar"), (flags & XAR_FLAG) != 0);
			this->associate(_T(".xz"),  _T("cubeice_xz"),  (flags & XZ_FLAG) != 0);
			
			SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_FLUSH,0,0);
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  associate
		 *
		 *  flag が true の場合はレジストリに追加，false の場合で該当の
		 *  キーが存在する場合はそのキーを削除する．
		 *  // 変更 (2010/12/29)
		 *  flag が true の場合、レジストリにもvalue以外のキーが設定されていた場合、そのキーをPREV_ARCHIVERに保存
		 *  flag が false の場合、レジストリのCUBEICE_REG_PREVARCHIVERがあれば、その値を取得し、既定のキーとする
		 */
		/* ----------------------------------------------------------------- */
		void associate(const string_type& key, const string_type& value, bool flag) {
			HKEY subkey;
			if (flag) {
				DWORD disposition = 0;
				LONG status = RegCreateKeyEx(HKEY_CLASSES_ROOT, key.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &subkey, &disposition);
				if (!status) { 
					char_type buffer[32] = {};
					DWORD type = 0;
					DWORD size = sizeof(buffer);
					if (RegQueryValueEx(subkey, _T(""), NULL, &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS && string_type(buffer) != value) {
						// 現在のキーをCUBEICE_REG_PREVARCHIVERに保存
						RegSetValueEx(subkey, CUBEICE_REG_PREVARCHIVER, 0, REG_SZ, (CONST BYTE*)buffer, strlen(buffer) + 1);
					}
					RegSetValueEx(subkey, _T(""), 0, REG_SZ, (CONST BYTE*)value.c_str(), value.size() + 1);
				}
			}
			else {
				LONG status = RegOpenKeyEx(HKEY_CLASSES_ROOT, key.c_str(), 0, KEY_ALL_ACCESS, &subkey);
				if (!status) {
					char_type buffer[32] = {};
					DWORD type = 0;
					DWORD size = sizeof(buffer);
					if (RegQueryValueEx(subkey, _T(""), NULL, &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS && string_type(buffer) == value) {
						char_type buffer[32] = {}; // bufferを再利用するとうまくいかなかったので、新たに作成
						DWORD type = 0;
						DWORD size = sizeof(buffer);
						if (RegQueryValueEx(subkey, CUBEICE_REG_PREVARCHIVER, NULL, &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS) {
							RegSetValueEx(subkey, _T(""), 0, REG_SZ, (CONST BYTE*)buffer, strlen(buffer) + 1);
						} else {
							//OutputDebugString("start reg delete value");
							RegDeleteValue(HKEY_CLASSES_ROOT, key.c_str());
						}
					}
				}
			}
		}
		
#if !defined(_UNICODE) && !defined(UNICODE)
		/* ----------------------------------------------------------------- */
		/*
		 *  create_shortcut
		 *
		 *  ショートカットを作成する．
		 *  see also: http://support.microsoft.com/kb/179904/ja
		 */
		/* ----------------------------------------------------------------- */
		void create_shortcut(const std::basic_string<char>& path, const std::basic_string<char>& args, const std::basic_string<char>& link, int icon) {
			IShellLink *psl = NULL;
			HRESULT hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
			if (FAILED(hres)) goto cleanup;
			
			IPersistFile *pPf = NULL;
			hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&pPf);
			if (FAILED(hres)) goto cleanup;
			
			hres = psl->SetPath(path.c_str());
			if (FAILED(hres)) goto cleanup;
			
			if (!args.empty()) {
				hres = psl->SetArguments(args.c_str());
				if (FAILED(hres)) goto cleanup;
			}
			
			//place the shortcut on the desktop
			LPITEMIDLIST pidl;
			SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl);
			
			char buf[2048] = {};
			SHGetPathFromIDList(pidl, buf);
			lstrcat(buf, "\\");
			lstrcat(buf, link.c_str());
			
			WORD wsz[2048] = {};
			MultiByteToWideChar(CP_ACP, 0, buf, -1, (LPWSTR)wsz, 2048);
			hres = pPf->Save((LPCOLESTR)wsz, TRUE);
			if (FAILED(hres)) goto cleanup;
			
			hres = psl->SetIconLocation(path.c_str(), icon);
			if (FAILED(hres)) goto cleanup;
			
			int id = 0;
			hres = psl->GetIconLocation(buf, 256, &id);
			if (FAILED(hres)) goto cleanup;
			
			pPf->Save((LPCOLESTR)wsz, TRUE);
cleanup:
			if (pPf) pPf->Release();
			if (psl) psl->Release();
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  create_shortcut
		 *
		 *  ショートカットを削除する．
		 */
		/* ----------------------------------------------------------------- */
		void remove_shortcut(const std::basic_string<char>& link) {
			LPITEMIDLIST pidl;
			SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl);
			
			char buf[2048] = {};
			SHGetPathFromIDList(pidl, buf);
			lstrcat(buf, "\\");
			lstrcat(buf, link.c_str());
			
			DeleteFile(buf);
		}
#endif
	};
}

#endif // CUBEICE_USER_SETTING_H
