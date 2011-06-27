/* ------------------------------------------------------------------------- */
/*
 *  user-setting.h
 *
 *  Copyright (c) 2010 CubeSoft Inc.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see < http://www.gnu.org/licenses/ >.
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_USER_SETTING_H
#define CUBEICE_USER_SETTING_H

#ifndef CLX_USE_WCHAR
#define CLX_USE_WCHAR
#endif

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
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <babel/babel.h>
#include "guid.h"

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
#define JAR_FLAG            0x00200000

/* ------------------------------------------------------------------------- */
//  コンテキストメニューに関連するフラグ
/* ------------------------------------------------------------------------- */
#define COMPRESS_FLAG       0x0001
#define DECOMPRESS_FLAG     0x0002
#define SETTING_FLAG        0x0004
#define MAIL_FLAG           0x0008

/* ------------------------------------------------------------------------- */
//  解凍のコンテキストメニューに関連するフラグ
/* ------------------------------------------------------------------------- */
#define DECOMP_HERE_FLAG            0x00000010
#define DECOMP_DESKTOP_FLAG         0x00000020
#define DECOMP_RUNTIME_FLAG         0x00000040
#define DECOMP_MYDOCUMENTS_FLAG     0x00000080
#define DECOMP_ALL_FLAG             0x000000f0

/* ------------------------------------------------------------------------- */
//  圧縮のコンテキストメニューに関連するフラグ
/* ------------------------------------------------------------------------- */
#define COMP_ZIP_FLAG               0x00000100
#define COMP_ZIP_PASS_FLAG          0x00000200
#define COMP_SEVENZIP_FLAG          0x00000400
#define COMP_BZIP2_FLAG             0x00000800
#define COMP_GZIP_FLAG              0x00001000
#define COMP_DETAIL_FLAG            0x00002000
#define COMP_ALL_FLAG               0x00003f00

/* ------------------------------------------------------------------------- */
//  圧縮してメール送信のコンテキストメニューに関連するフラグ
/* ------------------------------------------------------------------------- */
#define MAIL_ZIP_FLAG               0x00010000
#define MAIL_ZIP_PASS_FLAG          0x00020000
#define MAIL_SEVENZIP_FLAG          0x00040000
#define MAIL_BZIP2_FLAG             0x00080000
#define MAIL_GZIP_FLAG              0x00100000
#define MAIL_DETAIL_FLAG            0x00200000
#define MAIL_ALL_FLAG               0x003f0000

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
#define DETAIL_TOOLTIP        0x0400  // ツールチップにファイル一覧を表示する
#define DETAIL_REMOVE_SRC     0x0800  // 元ファイルを削除する

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
#define CUBEICE_REG_MAX_FILELIST        _T("MaxFilelist")
#define CUBEICE_REG_CONTEXT             _T("ContextFlags")
//#define CUBEICE_REG_SHORTCUT            _T("ShortcutFlags")
#define CUBEICE_REG_SCCOMPRESS          _T("ScCompressIndex")
#define CUBEICE_REG_FILTER              _T("Filter")
#define CUBEICE_REG_INSTALL             _T("InstallDirectory")
#define CUBEICE_REG_VERSION             _T("Version")
#define CUBEICE_REG_PREVARCHIVER        _T("PrevArchiver")

/* ------------------------------------------------------------------------- */
//  設定ファイルに関する情報
/* ------------------------------------------------------------------------- */
#define CUBEICE_XML_DIR                 "cubeice"
#define CUBEICE_XML_FILE_NAME           "setting.xml"
#define CUBEICE_XML_FILE_PATH           CUBEICE_XML_DIR "\\" CUBEICE_XML_FILE_NAME
#define CUBEICE_CONTEXT_ROOT            "cubeice.general.ctxmenu"
#define CUBEICE_CONTEXT_CUSTOMIZE       "customize"
#define CUBEICE_CONTEXT_CHECK_VALUE     "check"
#define CUBEICE_CONTEXT_ITEM_ID         "<xmlattr>.id"

/* ------------------------------------------------------------------------- */
//  ショートカットに関する情報
/* ------------------------------------------------------------------------- */
#define CUBEICE_SC_COMPRESS             _T("CubeICE 圧縮.lnk")
#define CUBEICE_SC_DECOMPRESS           _T("CubeICE 解凍.lnk")
#define CUBEICE_SC_SETTING              _T("CubeICE 設定.lnk")

namespace cubeice {
	namespace detail {
		typedef std::map<std::basic_string<TCHAR>, std::pair<std::basic_string<TCHAR>, std::size_t> > ext_map;
		typedef std::map<std::size_t, std::pair<std::basic_string<TCHAR>, std::basic_string<TCHAR> > > param_map;
		
		/* ----------------------------------------------------------------- */
		//  extensions
		/* ----------------------------------------------------------------- */
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
				exts[_T(".jar")]  = std::make_pair( _T("cubeice_jar"),  JAR_FLAG);
				initialized = true;
			}
			return exts;
		}
		
		/* ----------------------------------------------------------------- */
		//  compress_parameters
		/* ----------------------------------------------------------------- */
		inline param_map& compress_parameters() {
			static bool initialized = false;
			static param_map params;
			if (!initialized) {
				params[0] = std::make_pair(_T("zip"), _T("/c:zip"));
				params[1] = std::make_pair(_T("zip (パスワード)"), _T("/c:zip /p"));
				params[2] = std::make_pair(_T("7z"), _T("/c:7z"));
				params[3] = std::make_pair(_T("gzip"), _T("/c:gzip"));
				params[4] = std::make_pair(_T("bzip2"), _T("/c:bzip2"));
				params[5] = std::make_pair(_T("詳細設定"), _T("/c:detail"));
				initialized = true;
			}
			return params;
		}
	}
	
	/* --------------------------------------------------------------------- */
	/*
	 *  RegDeleteKeyNT
	 *
	 *  see also: http://support.microsoft.com/kb/142491/ja
	 */
	/* --------------------------------------------------------------------- */
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
			root_(root), flags_(0), details_(0), max_filelist_(0), output_condition_(0), output_path_() {
			//this->load();
		}
		
		/* ----------------------------------------------------------------- */
		//  is_associated
		/* ----------------------------------------------------------------- */
		bool is_associated(const string_type& key, const string_type& value) {
			HKEY subkey;
			LONG status = RegOpenKeyEx(HKEY_CLASSES_ROOT, key.c_str(), 0, KEY_READ, &subkey);
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
				
				dwSize = sizeof(max_filelist_);
				RegQueryValueEx(hkResult, CUBEICE_REG_MAX_FILELIST, NULL, &dwType, (LPBYTE)&max_filelist_, &dwSize);
				
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
			const detail::ext_map& exts = detail::extensions();
			for (detail::ext_map::const_iterator pos = exts.begin(); pos != exts.end(); pos++) {
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
				
				DWORD value = static_cast<DWORD>(details_);
				RegSetValueEx(hkResult, CUBEICE_REG_DETAILS, 0, REG_DWORD, (CONST BYTE*)&value, sizeof(value));
				
				value = static_cast<DWORD>(max_filelist_);
				RegSetValueEx(hkResult, CUBEICE_REG_MAX_FILELIST, 0, REG_DWORD, (CONST BYTE*)&value, sizeof(value));
				
				value = static_cast<DWORD>(output_condition_);
				RegSetValueEx(hkResult, CUBEICE_REG_OUTPUT_CONDITION, 0, REG_DWORD, (CONST BYTE*)&value, sizeof(value));
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
		//  max_filelist
		/* ----------------------------------------------------------------- */
		size_type& max_filelist() { return max_filelist_; }
		const size_type& max_filelist() const { return max_filelist_; }
		
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
		size_type max_filelist_;
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

		struct SUBMENU {
			int						id;
			string_type				str;
			std::vector<SUBMENU>	children;

			SUBMENU(int id_, string_type str_) : id(id_), str(str_)
			{
			}
		};
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		user_setting() :
			root_(CUBEICE_REG_ROOT), install_(_T("")), version_(_T("")),
			comp_(string_type(CUBEICE_REG_ROOT) + _T('\\') + CUBEICE_REG_COMPRESS),
			decomp_(string_type(CUBEICE_REG_ROOT) + _T('\\') + CUBEICE_REG_DECOMPRESS),
			ctx_flags_(0x03), sc_flags_(0), sc_index_(0), filters_(), update_(true) {
			comp_.output_condition() = 0x02;
			comp_.details() = 0x281;
			comp_.max_filelist() = 5;
			decomp_.output_condition() = 0x01;
			decomp_.details() = 0x5ed;
			babel::init_babel();
			this->load();
		}
		
		explicit user_setting(const string_type& root) :
			root_(root), install_(_T("")), version_(_T("")),
			comp_(root + _T('\\') + CUBEICE_REG_COMPRESS),
			decomp_(root + _T('\\') + CUBEICE_REG_DECOMPRESS),
			ctx_flags_(0x03), sc_flags_(0), sc_index_(0), filters_(), update_(true) {
			comp_.output_condition() = 0x02;
			comp_.details() = 0x281;
			comp_.max_filelist() = 5;
			decomp_.output_condition() = 0x01;
			decomp_.details() = 0x5ed;
			babel::init_babel();
			this->load();
		}
		
		/* ----------------------------------------------------------------- */
		//  load
		/* ----------------------------------------------------------------- */
		void load() {
			comp_.load();
			decomp_.load();
			
			HKEY hkResult;
			LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, root_.c_str(), 0, KEY_READ, &hkResult);
			if (!lResult) {
				DWORD dwType;
				DWORD dwSize = 0;
				
				char_type install[2 * 1024] = {};
				dwSize = sizeof(install);
				if (RegQueryValueEx(hkResult, CUBEICE_REG_INSTALL, NULL, &dwType, (LPBYTE)install, &dwSize) == ERROR_SUCCESS) {
					install_ = install;
				}
				
				char_type version[32] = {};
				dwSize = sizeof(version);
				if (RegQueryValueEx(hkResult, CUBEICE_REG_VERSION, NULL, &dwType, (LPBYTE)version, &dwSize) == ERROR_SUCCESS) {
					version_ = version;
				}
			}
			
			lResult = RegOpenKeyEx(HKEY_CURRENT_USER, root_.c_str(), 0, KEY_ALL_ACCESS, &hkResult);
			if (!lResult) {
				DWORD dwType;
				DWORD dwSize;
				
				char							xmlpath[2*MAX_PATH];

				ctx_flags_ = 0;
				SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, xmlpath);
				PathAppendA(xmlpath, CUBEICE_XML_FILE_PATH);

				ctx_customize_ = false;
				try {
					using namespace boost::property_tree;
					ptree		root;
					ptree		ctx_root;

					xml_parser::read_xml(xmlpath, root);
					ctx_root = root.get_child(CUBEICE_CONTEXT_ROOT);

					if(ctx_root.get<std::string>(CUBEICE_CONTEXT_CUSTOMIZE) == "yes") {
						ctx_customize_ = true;

						context_read(ctx_root, ctx_submenu_);
					} else {
						ctx_flags_ = ctx_root.get(CUBEICE_CONTEXT_CHECK_VALUE, (size_type)0);
					}
				} catch( const boost::property_tree::ptree_error & ) {
					dwSize = sizeof(ctx_flags_);
					RegQueryValueEx(hkResult, CUBEICE_REG_CONTEXT, NULL, &dwType, (LPBYTE)&ctx_flags_, &dwSize);
				}

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
			
			update_ = false;
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
				boost::property_tree::ptree		root;
				boost::property_tree::ptree		&ctx_root = root.put(CUBEICE_CONTEXT_ROOT, "");
				char							xmlpath[2*MAX_PATH];

				if(ctx_customize_) {
					ctx_root.put(CUBEICE_CONTEXT_CUSTOMIZE, "yes");
					context_write(ctx_root, ctx_submenu_);
				} else {
					ctx_root.put(CUBEICE_CONTEXT_CUSTOMIZE, "no");
					ctx_root.put(CUBEICE_CONTEXT_CHECK_VALUE, clx::lexical_cast<std::string>(ctx_flags_));
				}
				SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, xmlpath);
				PathAppendA(xmlpath, CUBEICE_XML_DIR);
				CreateDirectoryA(xmlpath, NULL);
				PathAppendA(xmlpath, CUBEICE_XML_FILE_NAME);
				boost::property_tree::xml_parser::write_xml(xmlpath, root);

				// ショートカットの処理．
				DWORD value = static_cast<DWORD>(sc_index_);
				RegSetValueEx(hkResult, CUBEICE_REG_SCCOMPRESS, 0, REG_DWORD, (CONST BYTE*)&value, sizeof(value));
				
				TCHAR buffer[2048] ={};
				GetModuleFileName(GetModuleHandle(NULL), buffer, 2048);
				std::basic_string<TCHAR> tmp = buffer;
				std::basic_string<TCHAR> current = tmp.substr(0, tmp.find_last_of(_T('\\')));
				const detail::param_map& v = detail::compress_parameters();
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
				if (update_) {
					std::basic_string<TCHAR> value = _T("\"");
					value += install_;
					value += _T("\\cubeice-checker.exe\"");
					RegSetValueEx(hkResult, _T("cubeice-checker"), 0, REG_SZ, (CONST BYTE*)value.c_str(), (value.length() + 1) * sizeof(char_type));
				}
				else RegDeleteValue(hkResult, _T("cubeice-checker"));
			}
			
			this->associate(decomp_.flags(), decomp_.details());
		}
		
		/* ----------------------------------------------------------------- */
		//  install_path
		/* ----------------------------------------------------------------- */
		const string_type& install_path() const { return install_; }
		
		/* ----------------------------------------------------------------- */
		//  version
		/* ----------------------------------------------------------------- */
		const string_type& version() const { return version_; }
		
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
		 *  context_customize
		 *
		 *  コンテキストメニューがカスタマイズされたかどうか．
		 */
		/* ----------------------------------------------------------------- */
		bool& context_customize() { return ctx_customize_; }
		const bool& context_customize() const { return ctx_customize_; }

		/* ----------------------------------------------------------------- */
		/*
		 *  context_flags
		 *
		 *  コンテキストメニューに表示させるもの．
		 */
		/* ----------------------------------------------------------------- */
		std::vector<SUBMENU>& context_submenu() { return ctx_submenu_; }
		const std::vector<SUBMENU>& context_submenu() const { return ctx_submenu_; }

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
		string_type install_;
		string_type version_;
		archive_type comp_;
		archive_type decomp_;
		size_type ctx_flags_;
		size_type sc_flags_;
		size_type sc_index_;
		container_type filters_;
		bool update_;
		bool ctx_customize_;
		std::vector<SUBMENU> ctx_submenu_;
		
		/* ----------------------------------------------------------------- */
		/*
		 *  associate
		 *
		 *  NOTE: レジストリに登録する cubeice_* はインストーラを用いて
		 *  あらかじめ登録しておくこと．
		 *  SPLIT_ADMIN_OPERATIONS が定義されている場合は，管理者権限の
		 *  必要な処理だけ別の exe で実行する．
		 */
		/* ----------------------------------------------------------------- */
		void associate(size_type flags, size_type details) {
#define SPLIT_ADMIN_OPERATIONS
#ifdef  SPLIT_ADMIN_OPERATIONS
			CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
			
			TCHAR buffer[32] = {};
			::_itot_s(flags, buffer, sizeof(buffer), 10);
			
			SHELLEXECUTEINFO sei = {};
			sei.cbSize = sizeof(SHELLEXECUTEINFO);
			//sei.lpVerb = _T("runas");
			sei.lpParameters = buffer;
			sei.lpFile = _T("cubeice-associate.exe");
			ShellExecuteEx(&sei);
			
			CoUninitialize();
#else
			const detail::ext_map& exts = detail::extensions();
			for (detail::ext_map::const_iterator pos = exts.begin(); pos != exts.end(); pos++) {
				this->associate(pos->first, pos->second.first, ((flags & pos->second.second) != 0), ((details & DETAIL_TOOLTIP) != 0));
			}
			SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_FLUSH,0,0);
#endif // SPLIT_ADMIN_OPERATIONS
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
		void associate(const string_type& key, const string_type& value, bool flag, bool tooltip) {
			static const char_type* clsid_tooltip = _T("{00021500-0000-0000-C000-000000000046}");
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
					char_type buffer[32] = {};
					DWORD type = 0;
					DWORD size = sizeof(buffer);
					if (RegQueryValueEx(subkey, _T(""), NULL, &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS && string_type(buffer) != value) {
						RegSetValueEx(subkey, CUBEICE_REG_PREVARCHIVER, 0, REG_SZ, (CONST BYTE*)buffer, (_tcslen(buffer) + 1) * sizeof(char_type));
					}
					RegSetValueEx(subkey, _T(""), 0, REG_SZ, (CONST BYTE*)value.c_str(), (value.size() + 1) * sizeof(char_type));
				}
				
				disposition = 0;
				status = RegCreateKeyEx(HKEY_CLASSES_ROOT, ( key + _T( "\\shellex\\") + clsid_tooltip ).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &subkey, &disposition);
				if (!status) {
					RegSetValueEx(subkey, _T(""), 0, REG_SZ, (CONST BYTE*)CLSID_CubeICE_STR, sizeof( CLSID_CubeICE_STR ));
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
						else {
							RegCloseKey(subkey);
							RegDeleteKeyNT(HKEY_CLASSES_ROOT, key.c_str());
						}
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
			
			return PathFileExists(buf) != FALSE;
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

		/* ----------------------------------------------------------------- */
		/*
		 *  context_read
		 *
		 *  コンテキストメニューの構造を読み込む．
		 */
		/* ----------------------------------------------------------------- */
		void context_read(const boost::property_tree::ptree &pt, std::vector<SUBMENU> &parent)
		{
			using namespace boost;
			using namespace boost::property_tree;

			BOOST_FOREACH(const ptree::value_type &v, pt) {
				if(v.first == "item") {
					optional<int>			attr_id		= v.second.get_optional<int>("<xmlattr>.id");
					optional<std::string>	attr_name	= v.second.get_optional<std::string>("<xmlattr>.name");
					if(attr_id) {
#ifdef	UNICODE
						parent.push_back(SUBMENU( *attr_id, attr_name ? babel::utf8_to_unicode(*attr_name) : L"" ));
#else
						parent.push_back(SUBMENU( *attr_id, attr_name ? babel::utf8_to_sjis(*attr_name) : "" ));
#endif
						if(v.second.size())
							context_read(v.second, parent.back().children);
					}
				}
			}
			return;
		}

		/* ----------------------------------------------------------------- */
		/*
		 *  context_write
		 *
		 *  コンテキストメニューの構造を書き込む．
		 */
		/* ----------------------------------------------------------------- */
		void context_write(boost::property_tree::ptree &pt, const std::vector<SUBMENU> &submenu)
		{
			using namespace boost;
			using namespace boost::property_tree;

			BOOST_FOREACH(const SUBMENU &s, submenu) {
				ptree	&item = pt.add("item", "");
				ptree	&attr = item.put("<xmlattr>", "");
				attr.put("id", clx::lexical_cast<std::string>(s.id));
				if(s.str!=_T("")) {
#ifdef	UNICODE
					attr.put("name", babel::unicode_to_utf8(s.str));
#else
					attr.put("name", babel::sjis_to_utf8(s.str));
#endif
				}
				context_write(item, s.children);
			}
		}
	};
}

#endif // CUBEICE_USER_SETTING_H
