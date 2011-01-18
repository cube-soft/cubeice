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

#define CLX_USE_WCHAR

#include <tchar.h>
#include <cstdlib>
#include <set>
#include <string>
#include <windows.h>
#include <winreg.h>
#include <shlobj.h>
#include <shlwapi.h>
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
#define MAIL_FLAG           0x0008

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
#define DETAIL_SINGLE_FOLDER  0x0100  // 単一フォルダの場合は作成しない
#define DETAIL_CHARCODE       0x0010  // 文字コードを変換する
#define DETAIL_FILTER         0x0020  // フィルタリングを行う
#define DETAIL_SKIP_DESKTOP   0x0040  // デスクトップの場合は開かない
#define DETAIL_MAIL_REMOVE    0x0200  // メール添付の際にファイルを残さない
#define DETAIL_REPORT         0x0080  // エラーレポートを表示する

/* ------------------------------------------------------------------------- */
//  レジストリに関する情報
/* ------------------------------------------------------------------------- */
#define CUBEICE_REG_ROOT                _T("Software\\CubeSoft\\CubeICE")
#define CUBEICE_REG_COMPRESS            _T("Compress")
#define CUBEICE_REG_DECOMPRESS          _T("Decompress")
//#define CUBEICE_REG_FLAGS               _T("Flags")
#define CUBEICE_REG_DETAILS	            _T("Details")
#define CUBEICE_REG_OUTPUT_CONDITION    _T("OutputCondition")
#define CUBEICE_REG_OUTPUT_PATH         _T("OutputPath")
#define CUBEICE_REG_CONTEXT             _T("ContextFlags")
//#define CUBEICE_REG_SHORTCUT            _T("ShortcutFlags")
#define CUBEICE_REG_SCCOMPRESS          _T("ScCompressIndex")
#define CUBEICE_REG_FILTER              _T("Filter")
#define CUBEICE_REG_INSTALL             _T("InstallDirectory")
#define CUBEICE_REG_VERSION             _T("Version")
#define CUBEICE_REG_PREVARCHIVER        _T("PrevArchiver")

/* ------------------------------------------------------------------------- */
//  ショートカットに関する情報
/* ------------------------------------------------------------------------- */
#define CUBEICE_SC_COMPRESS             _T("CubeICE 圧縮.lnk")
#define CUBEICE_SC_DECOMPRESS           _T("CubeICE 解凍.lnk")
#define CUBEICE_SC_SETTING              _T("CubeICE 設定.lnk")

namespace cubeice {
	typedef std::map<std::basic_string<TCHAR>, std::pair<std::basic_string<TCHAR>, std::size_t> > ext_map;
	typedef std::map<std::size_t, std::pair<std::basic_string<TCHAR>, std::basic_string<TCHAR> > > param_map;
	
	/* --------------------------------------------------------------------- */
	//  extensions
	/* --------------------------------------------------------------------- */
	inline ext_map& extensions() {
		static bool initialized = false;
		static ext_map exts;
		if (!initialized) {
			exts[_T(".zip")] = std::make_pair(_T("cubeice_zip"), ZIP_FLAG);
			exts[_T(".lzh")] = std::make_pair(_T("cubeice_lzh"), LZH_FLAG);
			exts[_T(".rar")] = std::make_pair(_T("cubeice_rar"), RAR_FLAG);
			exts[_T(".tar")] = std::make_pair(_T("cubeice_tar"), TAR_FLAG);
			exts[_T(".gz")]  = std::make_pair(_T("cubeice_gz"),  GZ_FLAG);
			exts[_T(".7z")]  = std::make_pair(_T("cubeice_7z"),  SEVENZIP_FLAG);
			exts[_T(".arj")] = std::make_pair(_T("cubeice_arj"), ARJ_FLAG);
			exts[_T(".bz2")] = std::make_pair(_T("cubeice_bz2"), BZ2_FLAG);
			exts[_T(".cab")] = std::make_pair(_T("cubeice_cab"), CAB_FLAG);
			exts[_T(".chm")] = std::make_pair(_T("cubeice_chm"), CHM_FLAG);
			exts[_T(".cpio")]= std::make_pair(_T("cubeice_cpio"),CPIO_FLAG);
			exts[_T(".deb")] = std::make_pair(_T("cubeice_deb"), DEB_FLAG);
			exts[_T(".dmg")] = std::make_pair(_T("cubeice_dmg"), DMG_FLAG);
			exts[_T(".iso")] = std::make_pair(_T("cubeice_iso"), ISO_FLAG);
			exts[_T(".rpm")] = std::make_pair(_T("cubeice_rpm"), RPM_FLAG);
			exts[_T(".tbz")] = std::make_pair(_T("cubeice_tbz"), TBZ_FLAG);
			exts[_T(".tgz")] = std::make_pair(_T("cubeice_tgz"), TGZ_FLAG);
			exts[_T(".wim")] = std::make_pair(_T("cubeice_wim"), WIM_FLAG);
			exts[_T(".xar")] = std::make_pair(_T("cubeice_xar"), XAR_FLAG);
			exts[_T(".xz")]  = std::make_pair( _T("cubeice_xz"),  XZ_FLAG);
			initialized = true;
		}
		return exts;
	}
	
	/* --------------------------------------------------------------------- */
	//  compress_parameters
	/* --------------------------------------------------------------------- */
	inline param_map& compress_parameters() {
		static bool initialized = false;
		static param_map params;
		if (!initialized) {
			params[0] = std::make_pair(_T("zip"), _T("/c:zip"));
			params[1] = std::make_pair(_T("zip (パスワード)"), _T("/c:zip /p"));
			params[2] = std::make_pair(_T("7z"), _T("/c:7z"));
			params[3] = std::make_pair(_T("gzip"), _T("/c:gzip"));
			params[4] = std::make_pair(_T("bzip2"), _T("/c:bzip2"));
			initialized = true;
		}
		return params;
	}
	
	/* --------------------------------------------------------------------- */
	//  is_dotnet_installed
	/* --------------------------------------------------------------------- */
	inline bool is_dotnet_installed() {
		std::basic_string<TCHAR> subkey = _T("Software\\Microsoft\\NET Framework Setup\\NDP\\v2.0.50727");
		HKEY hkResult;
		LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey.c_str(), 0, KEY_READ, &hkResult);
		if (lResult) return false;
		
		int value = 0;
		DWORD dwType = 0;
		DWORD dwSize = sizeof(value);
		RegQueryValueEx(hkResult, _T("Install"), NULL, &dwType, (LPBYTE)&value, &dwSize);
		return (value == 1);
	}
	
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
		//  is_associated
		/* ----------------------------------------------------------------- */
		bool is_associated(const string_type& key, const string_type& value) {
			HKEY subkey;
			DWORD disposition = 0;
			LONG status = RegCreateKeyEx(HKEY_CLASSES_ROOT, key.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &subkey, &disposition);
			if (!status) { 
				char_type buffer[32] = {};
				DWORD type = 0;
				DWORD size = sizeof(buffer);
				if (RegQueryValueEx(subkey, _T(""), NULL, &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS && string_type(buffer) == value) {
					return true;
				}
			}
			return false;
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

			// Flagsはレジストリに置かず，それぞれの拡張子のキーの既定値が cubeice_XXX かどうかで判断
			flags_ = 0;
			const ext_map& exts = extensions();
			for (ext_map::const_iterator pos = exts.begin(); pos != exts.end(); pos++) {
				if (this->is_associated(pos->first, pos->second.first)) flags_ |= pos->second.second;
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
				//RegSetValueEx(hkResult, CUBEICE_REG_FLAGS, 0, REG_DWORD, (CONST BYTE*)&flags_, sizeof(flags_));
				RegSetValueEx(hkResult, CUBEICE_REG_DETAILS, 0, REG_DWORD, (CONST BYTE*)&details_, sizeof(details_));
				RegSetValueEx(hkResult, CUBEICE_REG_OUTPUT_CONDITION, 0, REG_DWORD, (CONST BYTE*)&output_condition_, sizeof(output_condition_));
				RegSetValueEx(hkResult, CUBEICE_REG_OUTPUT_PATH, 0, REG_SZ, (CONST BYTE*)output_path_.c_str(), (output_path_.length() + 1) * sizeof(char_type));
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
			ctx_flags_(0), sc_flags_(0), sc_index_(0), filters_(), update_(false) {
			this->load();	
		}
		
		explicit user_setting(const string_type& root) :
			root_(root),
			comp_(root + _T('\\') + CUBEICE_REG_COMPRESS),
			decomp_(root + _T('\\') + CUBEICE_REG_DECOMPRESS),
			ctx_flags_(0), sc_flags_(0), sc_index_(0), filters_(), update_(false) {
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
				
				sc_flags_ = 0;
				//dwSize = sizeof(sc_flags_);
				//RegQueryValueEx(hkResult, CUBEICE_REG_SHORTCUT, NULL, &dwType, (LPBYTE)&sc_flags_, &dwSize);
				if (shortcut_exist(CUBEICE_SC_COMPRESS)) sc_flags_ |= COMPRESS_FLAG;
				if (shortcut_exist(CUBEICE_SC_DECOMPRESS)) sc_flags_ |= DECOMPRESS_FLAG;
				if (shortcut_exist(CUBEICE_SC_SETTING)) sc_flags_ |= SETTING_FLAG;
				
				dwSize = sizeof(sc_index_);
				RegQueryValueEx(hkResult, CUBEICE_REG_SCCOMPRESS, NULL, &dwType, (LPBYTE)&sc_index_, &dwSize);
				
				// 変数側の型は std::set<std::string>
				char_type buffer[8 * 1024] = {};
				dwSize = sizeof(buffer);
				if (RegQueryValueEx(hkResult, CUBEICE_REG_FILTER, NULL, &dwType, (LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
					string_type s(buffer);
					filters_.clear();
					clx::split_if(s, filters_, clx::is_any_of(_T("<>")));
				}
			}
			
			lResult = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_ALL_ACCESS, &hkResult);
			if (!lResult) {
				char_type buffer[1024] = {};
				DWORD dwType;
				DWORD dwSize = sizeof(buffer);
				if (RegQueryValueEx(hkResult, _T("cubeice-checker"), NULL, &dwType, (LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) update_ = true;
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
				
				// ショートカットの処理．
				//RegSetValueEx(hkResult, CUBEICE_REG_SHORTCUT, 0, REG_DWORD, (CONST BYTE*)&sc_flags_, sizeof(sc_flags_));
				RegSetValueEx(hkResult, CUBEICE_REG_SCCOMPRESS, 0, REG_DWORD, (CONST BYTE*)&sc_index_, sizeof(sc_flags_));
				
				TCHAR buffer[2048] ={};
				GetModuleFileName(GetModuleHandle(NULL), buffer, 2048);
				std::basic_string<TCHAR> tmp = buffer;
				std::basic_string<TCHAR> current = tmp.substr(0, tmp.find_last_of(_T('\\')));
				const param_map& v = compress_parameters();
				std::basic_string<TCHAR> params = (v.find(sc_index_) != v.end()) ? v.find(sc_index_)->second.second : _T("/c:zip");
				if ((sc_flags_ & COMPRESS_FLAG)) this->create_shortcut(current + _T("\\cubeice.exe"), params, CUBEICE_SC_COMPRESS, 1);
				else this->remove_shortcut(CUBEICE_SC_COMPRESS);
				if ((sc_flags_ & DECOMPRESS_FLAG)) this->create_shortcut(current + _T("\\cubeice.exe"), _T("/x"), CUBEICE_SC_DECOMPRESS, 2);
				else this->remove_shortcut(CUBEICE_SC_DECOMPRESS);
				if ((sc_flags_ & SETTING_FLAG)) this->create_shortcut(current + _T("\\cubeice-setting.exe"), _T(""), CUBEICE_SC_SETTING, 0);
				else this->remove_shortcut(CUBEICE_SC_SETTING);
				
				string_type dest;
				clx::join(filters_, dest, _T("<>"));
				RegSetValueEx(hkResult, CUBEICE_REG_FILTER, 0, REG_SZ, (CONST BYTE*)dest.c_str(), (dest.length() + 1) * sizeof(char_type));
			}
			
			lResult = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_ALL_ACCESS, &hkResult);
			if (!lResult) {
				if (update_ && is_dotnet_installed()) {
					TCHAR buffer[1024] = {};
					GetCurrentDirectory(1024, buffer);
					std::basic_string<TCHAR> value = _T("\"");
					value += buffer;
					value += _T("\\cubeice-checker.exe\"");
					RegSetValueEx(hkResult, _T("cubeice-checker"), 0, REG_SZ, (CONST BYTE*)value.c_str(), (value.length() + 1) * sizeof(char_type));
				}
				else RegDeleteValue(hkResult, _T("cubeice-checker"));
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
		//  shortcut_compress_index
		/* ----------------------------------------------------------------- */
		size_type& shortcut_compress_index() { return sc_index_; }
		const size_type& shortcut_compress_index() const { return sc_index_; }
		
		/* ----------------------------------------------------------------- */
		//  filters
		/* ----------------------------------------------------------------- */
		container_type& filters() { return filters_; }
		const container_type& filters() const { return filters_; }
		
		/* ----------------------------------------------------------------- */
		//  update
		/* ----------------------------------------------------------------- */
		bool& update() { return update_; }
		const bool& update() const { return update_; }
		
	private:
		string_type root_;
		archive_type comp_;
		archive_type decomp_;
		size_type ctx_flags_;
		size_type sc_flags_;
		size_type sc_index_;
		container_type filters_;
		bool update_;
		
		/* ----------------------------------------------------------------- */
		/*
		 *  associate
		 *
		 *  NOTE: レジストリに登録する cubeice_* はインストーラを用いて
		 *  あらかじめ登録しておくこと．
		 */
		/* ----------------------------------------------------------------- */
		void associate(size_type flags) {
			const ext_map& exts = extensions();
			for (ext_map::const_iterator pos = exts.begin(); pos != exts.end(); pos++) {
				this->associate(pos->first, pos->second.first, ((flags & pos->second.second) != 0));
			}
			SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_FLUSH,0,0);
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  associate
		 *
		 *  flag が true の場合はレジストリに追加，false の場合で該当の
		 *  キーが存在する場合はそのキーを削除する．
		 *
		 *  2010/12/29 変更:
		 *   - flag が true の場合、レジストリにも value 以外のキーが設定
		 *     されていた場合、そのキーを PREV_ARCHIVER に保存
		 *   - flag が false の場合、レジストリの CUBEICE_REG_PREVARCHIVER
		 *     があれば、その値を取得し、既定のキーとする
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
						RegSetValueEx(subkey, CUBEICE_REG_PREVARCHIVER, 0, REG_SZ, (CONST BYTE*)buffer, (_tcslen(buffer) + 1) * sizeof(char_type));
					}
					RegSetValueEx(subkey, _T(""), 0, REG_SZ, (CONST BYTE*)value.c_str(), (value.size() + 1) * sizeof(char_type));
				}
			}
			else {
				LONG status = RegOpenKeyEx(HKEY_CLASSES_ROOT, key.c_str(), 0, KEY_ALL_ACCESS, &subkey);
				if (!status) {
					char_type buffer[32] = {};
					DWORD type = 0;
					DWORD size = sizeof(buffer);
					if (RegQueryValueEx(subkey, _T(""), NULL, &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS && string_type(buffer) == value) {
						char_type prev[32] = {};
						type = 0;
						size = sizeof(prev);
						if (RegQueryValueEx(subkey, CUBEICE_REG_PREVARCHIVER, NULL, &type, (LPBYTE)prev, &size) == ERROR_SUCCESS) {
							RegSetValueEx(subkey, _T(""), 0, REG_SZ, (CONST BYTE*)prev, (_tcslen(prev) + 1) * sizeof(char_type));
						}
						else RegDeleteKey(HKEY_CLASSES_ROOT, key.c_str());
					}
				}
			}
		}
		
		/* ----------------------------------------------------------------- */
		//  shortcut_exist
		/* ----------------------------------------------------------------- */
		bool shortcut_exist(const std::basic_string<TCHAR>& link) {
			LPITEMIDLIST pidl;
			SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl);
			
			TCHAR buf[2048] = {};
			SHGetPathFromIDList(pidl, buf);
			lstrcat(buf, _T("\\"));
			lstrcat(buf, link.c_str());
			
			return PathFileExists(buf) == TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  create_shortcut
		 *
		 *  ショートカットを作成する．
		 *  see also: http://support.microsoft.com/kb/179904/ja
		 */
		/* ----------------------------------------------------------------- */
		void create_shortcut(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& args, const std::basic_string<TCHAR>& link, int icon) {
			HRESULT hres = CoInitialize(NULL);
			if (FAILED(hres)) return;
			
			IShellLink *psl = NULL;
			hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
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
			
			TCHAR buf[2048] = {};
			SHGetPathFromIDList(pidl, buf);
			lstrcat(buf, _T("\\"));
			lstrcat(buf, link.c_str());
			
#ifdef	UNICODE
			WORD *wsz = reinterpret_cast<WORD*>(buf);
#else	// UNICODE
			WORD wsz[2048] = {};
			MultiByteToWideChar(CP_ACP, 0, buf, -1, (LPWSTR)wsz, 2048);
#endif	// UNICODE
			hres = psl->SetIconLocation(path.c_str(), icon);
			if (FAILED(hres)) goto cleanup;
			
			pPf->Save((LPCOLESTR)wsz, TRUE);
cleanup:
			if (pPf) pPf->Release();
			if (psl) psl->Release();
			CoUninitialize();
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  create_shortcut
		 *
		 *  ショートカットを削除する．
		 */
		/* ----------------------------------------------------------------- */
		void remove_shortcut(const std::basic_string<TCHAR>& link) {
			LPITEMIDLIST pidl;
			SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl);
			
			TCHAR buf[2048] = {};
			SHGetPathFromIDList(pidl, buf);
			lstrcat(buf, _T("\\"));
			lstrcat(buf, link.c_str());
			
			DeleteFile(buf);
		}
	};
}

#endif // CUBEICE_USER_SETTING_H
