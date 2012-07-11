/* ------------------------------------------------------------------------- */
/*
 *  dialog.h
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
#ifndef CUBE_DIALOG_H
#define CUBE_DIALOG_H

#include <vector>
#include <set>
#include <string>
#include <tchar.h>
#include "resource.h"

#ifndef NO_WIN32_LEAN_AND_MEAN
#define NO_WIN32_LEAN_AND_MEAN
#include <shlobj.h>
#endif // NO_WIN32_LEAN_AND_MEAN

#define CUBE_MAX_PATH 2048

#include <cubeice/config.h>
#include <cubeice/user-setting.h>
#include "progressbar.h"
#include "file-dialog.h"

namespace cubeice {
	extern std::basic_string<TCHAR>& password();
	
	namespace dialog {
		namespace detail {
			/* ------------------------------------------------------------- */
			//  dialog_init
			/* ------------------------------------------------------------- */
			class dialog_init {
			public:
				dialog_init() {
					INITCOMMONCONTROLSEX ic = { sizeof(INITCOMMONCONTROLSEX) };
					ic.dwICC = ICC_PROGRESS_CLASS;
					InitCommonControlsEx(&ic);
				}
			};

			static dialog_init dialog_init_;
		}

		extern int password(HWND owner, int which);
		extern int overwrite(HWND owner, const std::basic_string<TCHAR>& message);
		extern int report(HWND owner, const std::basic_string<TCHAR>& message);
		extern int runtime_setting(HWND owner, CubeICE::RuntimeSetting& setting);
	}
	
	/* --------------------------------------------------------------------- */
	//  ダイアログ用データ
	/* --------------------------------------------------------------------- */
	namespace dialog_data {
		typedef std::vector<std::basic_string<TCHAR> > param_list;
		
		/* ----------------------------------------------------------------- */
		//  compress_types
		/* ----------------------------------------------------------------- */
		inline param_list& compress_types(const bool &update) {
			static param_list v_;
			static param_list v2_;

			static bool initialized_ = false;
			if (!initialized_) {
				v_.clear();
				v_.push_back(_T("zip"));
				v_.push_back(_T("7z"));
				v_.push_back(_T("tar"));
				v_.push_back(_T("gzip"));
				v_.push_back(_T("bzip2"));
				v_.push_back(_T("tgz"));
				v_.push_back(_T("tbz"));
				v_.push_back(_T("exe"));

				v2_.clear();
				v2_.push_back(_T("zip"));
				v2_.push_back(_T("7z"));
			}
			return update ? v2_ : v_;
		}
		
		/* ----------------------------------------------------------------- */
		//  compress_levels
		/* ----------------------------------------------------------------- */
		inline param_list& compress_levels() {
			static param_list v_;
			static bool initialized_ = false;
			if (!initialized_) {
				v_.clear();
				v_.push_back(_T("無圧縮"));
				v_.push_back(_T("最速"));
				v_.push_back(_T("高速"));
				v_.push_back(_T("標準"));
				v_.push_back(_T("最高"));
				v_.push_back(_T("超圧縮"));
			}
			return v_;
		}
		
		/* ----------------------------------------------------------------- */
		//  zip_methods
		/* ----------------------------------------------------------------- */
		inline param_list& zip_methods() {
			static param_list v_;
			static bool initialized_ = false;
			if (!initialized_) {
				v_.clear();
				v_.push_back(_T("Deflate"));
				v_.push_back(_T("Deflate64"));
				v_.push_back(_T("LZMA"));
				v_.push_back(_T("PPMd"));
				v_.push_back(_T("BZip2"));
			}
			return v_;
		}
		
		/* ----------------------------------------------------------------- */
		//  sevenzip_methods
		/* ----------------------------------------------------------------- */
		inline param_list& sevenzip_methods() {
			static param_list v_;
			static bool initialized_ = false;
			if (!initialized_) {
				v_.clear();
				v_.push_back(_T("LZMA"));
				v_.push_back(_T("LZMA2"));
				v_.push_back(_T("PPMd"));
				v_.push_back(_T("BZip2"));
			}
			return v_;
		}
		
		/* ----------------------------------------------------------------- */
		//  encode_methods
		/* ----------------------------------------------------------------- */
		inline param_list& encode_methods() {
			static param_list v_;
			static bool initialized_ = false;
			if (!initialized_) {
				v_.clear();
				v_.push_back(_T("ZipCrypto"));
				v_.push_back(_T("AES256"));
			}
			return v_;
		}
	}
}

#endif // CUBE_DIALOG_H