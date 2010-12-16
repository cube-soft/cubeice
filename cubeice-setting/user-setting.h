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
#include <string>
#include <windows.h>
#include <winreg.h>

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

/* ------------------------------------------------------------------------- */
//  レジストリに関する情報
/* ------------------------------------------------------------------------- */
#define CUBEICE_REG_ROOT                "Software\\CubeSoft\\CubeICE"
#define CUBEICE_REG_COMPRESS            "Compress"
#define CUBEICE_REG_DECOMPRESS          "Decompress"
#define CUBEICE_REG_FLAGS               "Flags"
#define CUBEICE_REG_DETAILS	            "Details"
#define CUBEICE_REG_OUTPUT_CONDITION    "OutputCondition"
#define CUBEICE_REG_OUTPUT_PATH         "OutputPath"
#define CUBEICE_REG_CONTEXT             "ContextFlags"
#define CUBEICE_REG_FILTER              "Filter"

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
			lResult = RegCreateKeyEx(HKEY_CURRENT_USER, root_.c_str(), 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, &dwDisposition);
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
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		user_setting() :
			root_(CUBEICE_REG_ROOT),
			comp_(string_type(CUBEICE_REG_ROOT) + '\\' + CUBEICE_REG_COMPRESS),
			decomp_(string_type(CUBEICE_REG_ROOT) + '\\' + CUBEICE_REG_DECOMPRESS),
			flags_(0), filter_() {
			this->load();	
		}
		
		explicit user_setting(const string_type& root) :
			root_(root),
			comp_(root + '\\' + CUBEICE_REG_COMPRESS),
			decomp_(root + '\\' + CUBEICE_REG_DECOMPRESS),
			flags_(0), filter_() {
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
				
				dwSize = sizeof(flags_);
				RegQueryValueEx(hkResult, CUBEICE_REG_CONTEXT, NULL, &dwType, (LPBYTE)&flags_, &dwSize);
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
			LONG lResult = RegCreateKeyEx(HKEY_CURRENT_USER, root_.c_str(), 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, &dwDisposition);
			if (!lResult) {
				RegSetValueEx(hkResult, CUBEICE_REG_CONTEXT, 0, REG_DWORD, (CONST BYTE*)&flags_, sizeof(flags_));
			}
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
		size_type& context_flags() { return flags_; }
		const size_type& context_flags() const { return flags_; }
		
		/* ----------------------------------------------------------------- */
		//  filters
		/* ----------------------------------------------------------------- */
		string_type& filter() { return filter_; }
		const string_type& filter() const { return filter_; }
		
	private:
		string_type root_;
		archive_type comp_;
		archive_type decomp_;
		size_type flags_;
		string_type filter_;
	};
}

#endif // CUBEICE_USER_SETTING_H
