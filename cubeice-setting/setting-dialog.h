/* ------------------------------------------------------------------------- */
/*
 *  setting-dialog.h
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
 *  Last-modified: Wed 17 Nov 2010 17:39:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_DIALOG_H
#define CUBEICE_DIALOG_H

#include <cubeice/config.h>
#include <cubeice/user-setting.h>
#include <map>
#include "cubeice-setting.h"
#include "resource.h"

#ifndef CUBE_MAX_PATH
#define CUBE_MAX_PATH 2048
#endif

extern cubeice::user_setting Setting;

namespace cubeice {
	namespace dialog {
		extern int customize(HWND owner, cubeice::user_setting& setting);
	}

	extern INT_PTR create_propsheet(HWND parent, bool install);
	
	typedef std::map<std::size_t, std::size_t> flag_map;
	typedef std::map<std::size_t, std::basic_string<TCHAR> > sctype_map;
	
	/* ----------------------------------------------------------------- */
	//  decompress_map
	/* ----------------------------------------------------------------- */
	inline flag_map& decompress_map() {
		static bool initialized = false;
		static flag_map dest;
		if (!initialized) {
			dest[IDC_ZIP_CHECKBOX] = ZIP_FLAG;
			dest[IDC_LZH_CHECKBOX] = LZH_FLAG;
			dest[IDC_RAR_CHECKBOX] = RAR_FLAG;
			dest[IDC_TAR_CHECKBOX] = TAR_FLAG;
			dest[IDC_GZ_CHECKBOX] = GZ_FLAG;
			dest[IDC_SEVENZIP_CHECKBOX] = SEVENZIP_FLAG;
			dest[IDC_ARJ_CHCKBOX] = ARJ_FLAG;
			dest[IDC_BZ2_CHECKBOX] = BZ2_FLAG;
			dest[IDC_CAB_CHECKBOX] = CAB_FLAG;
			dest[IDC_CHM_CHECKBOX] = CHM_FLAG;
			dest[IDC_CPIO_CHECKBOX] = CPIO_FLAG;
			dest[IDC_DEB_CHECKBOX] = DEB_FLAG;
			dest[IDC_DMG_CHECKBOX] = DMG_FLAG;
			dest[IDC_ISO_CHECKBOX] = ISO_FLAG;
			dest[IDC_RPM_CHECKBOX] = RPM_FLAG;
			dest[IDC_TBZ_CHECKBOX] = TBZ_FLAG;
			dest[IDC_TGZ_CHECKBOX] = TGZ_FLAG;
			dest[IDC_WIM_CHECKBOX] = WIM_FLAG;
			dest[IDC_XAR_CHECKBOX] = XAR_FLAG;
			dest[IDC_XZ_CHECKBOX] = XZ_FLAG;
			dest[IDC_JAR_CHECKBOX] = JAR_FLAG;
			initialized = true;
		}
		return dest;
	}
	
	/* ----------------------------------------------------------------- */
	//  context_map
	/* ----------------------------------------------------------------- */
	inline flag_map& context_map() {
		static bool initialized = false;
		static flag_map dest;
		if (!initialized) {
			dest[IDC_CT_COMPRESS_CHECKBOX] = COMPRESS_FLAG;
			dest[IDC_CT_DECOMPRESS_CHECKBOX] = DECOMPRESS_FLAG;
			dest[IDC_CT_MAIL_CHECKBOX] = MAIL_FLAG;
			
			// 圧縮メニューのサブメニュー
			dest[IDC_COMP_ZIP_CHECKBOX] = COMP_ZIP_FLAG;
			dest[IDC_COMP_ZIP_PASS_CHECKBOX] = COMP_ZIP_PASS_FLAG;
			dest[IDC_COMP_SEVENZIP_CHECKBOX] = COMP_SEVENZIP_FLAG;
			dest[IDC_COMP_BZIP2_CHECKBOX] = COMP_BZIP2_FLAG;
			dest[IDC_COMP_GZIP_CHECKBOX] = COMP_GZIP_FLAG;
			dest[IDC_COMP_EXE_CHECKBOX] = COMP_EXE_FLAG;
			dest[IDC_COMP_DETAIL_CHECKBOX] = COMP_DETAIL_FLAG;
			
			// 解凍メニューのサブメニュー
			dest[IDC_DECOMP_HERE_CHECKBOX] = DECOMP_HERE_FLAG;
			dest[IDC_DECOMP_DESKTOP_CHECKBOX] = DECOMP_DESKTOP_FLAG;
			dest[IDC_DECOMP_RUNTIME_CHECKBOX] = DECOMP_RUNTIME_FLAG;
			dest[IDC_DECOMP_MYDOCUMENTS_CHECKBOX] = DECOMP_MYDOCUMENTS_FLAG;
			
			// 圧縮してメール送信のサブメニュー
			dest[IDC_MAIL_ZIP_CHECKBOX] = MAIL_ZIP_FLAG;
			dest[IDC_MAIL_ZIP_PASS_CHECKBOX] = MAIL_ZIP_PASS_FLAG;
			dest[IDC_MAIL_SEVENZIP_CHECKBOX] = MAIL_SEVENZIP_FLAG;
			dest[IDC_MAIL_BZIP2_CHECKBOX] = MAIL_BZIP2_FLAG;
			dest[IDC_MAIL_GZIP_CHECKBOX] = MAIL_GZIP_FLAG;
			dest[IDC_MAIL_EXE_CHECKBOX] = MAIL_EXE_FLAG;
			dest[IDC_MAIL_DETAIL_CHECKBOX] = MAIL_DETAIL_FLAG;
			
			initialized = true;
		}
		return dest;
	}
	
	/* ----------------------------------------------------------------- */
	//  context_root_map
	/* ----------------------------------------------------------------- */
	inline flag_map& context_root_map() {
		static bool initialized = false;
		static flag_map dest;
		if (!initialized) {
			dest[IDC_CT_COMPRESS_CHECKBOX] = COMPRESS_FLAG;
			dest[IDC_CT_DECOMPRESS_CHECKBOX] = DECOMPRESS_FLAG;
			dest[IDC_CT_MAIL_CHECKBOX] = MAIL_FLAG;
			
			initialized = true;
		}
		return dest;
	}

	/* ----------------------------------------------------------------- */
	//  context_compress_map
	/* ----------------------------------------------------------------- */
	inline flag_map& context_compress_map() {
		static bool initialized = false;
		static flag_map dest;
		if (!initialized) {
			dest[IDC_COMP_ZIP_CHECKBOX] = COMP_ZIP_FLAG;
			dest[IDC_COMP_ZIP_PASS_CHECKBOX] = COMP_ZIP_PASS_FLAG;
			dest[IDC_COMP_SEVENZIP_CHECKBOX] = COMP_SEVENZIP_FLAG;
			dest[IDC_COMP_BZIP2_CHECKBOX] = COMP_BZIP2_FLAG;
			dest[IDC_COMP_GZIP_CHECKBOX] = COMP_GZIP_FLAG;
			dest[IDC_COMP_EXE_CHECKBOX] = COMP_EXE_FLAG;
			dest[IDC_COMP_DETAIL_CHECKBOX] = COMP_DETAIL_FLAG;
		}
		return dest;
	}
	
	/* ----------------------------------------------------------------- */
	//  context_decompress_map
	/* ----------------------------------------------------------------- */
	inline flag_map& context_decompress_map() {
		static bool initialized = false;
		static flag_map dest;
		if (!initialized) {
			dest[IDC_DECOMP_HERE_CHECKBOX] = DECOMP_HERE_FLAG;
			dest[IDC_DECOMP_DESKTOP_CHECKBOX] = DECOMP_DESKTOP_FLAG;
			dest[IDC_DECOMP_RUNTIME_CHECKBOX] = DECOMP_RUNTIME_FLAG;
			dest[IDC_DECOMP_MYDOCUMENTS_CHECKBOX] = DECOMP_MYDOCUMENTS_FLAG;
		}
		return dest;
	}
	
	/* ----------------------------------------------------------------- */
	//  context_mail_map
	/* ----------------------------------------------------------------- */
	inline flag_map& context_mail_map() {
		static bool initialized = false;
		static flag_map dest;
		if (!initialized) {
			dest[IDC_MAIL_ZIP_CHECKBOX] = MAIL_ZIP_FLAG;
			dest[IDC_MAIL_ZIP_PASS_CHECKBOX] = MAIL_ZIP_PASS_FLAG;
			dest[IDC_MAIL_SEVENZIP_CHECKBOX] = MAIL_SEVENZIP_FLAG;
			dest[IDC_MAIL_BZIP2_CHECKBOX] = MAIL_BZIP2_FLAG;
			dest[IDC_MAIL_GZIP_CHECKBOX] = MAIL_GZIP_FLAG;
			dest[IDC_MAIL_EXE_CHECKBOX] = MAIL_EXE_FLAG;
			dest[IDC_MAIL_DETAIL_CHECKBOX] = MAIL_DETAIL_FLAG;
		}
		return dest;
	}
	
	/* ----------------------------------------------------------------- */
	//  shortcut_map
	/* ----------------------------------------------------------------- */
	inline flag_map& shortcut_map() {
		static bool initialized = false;
		static flag_map dest;
		if (!initialized) {
			dest[IDC_SC_COMPRESS_CHECKBOX] = COMPRESS_FLAG;
			dest[IDC_SC_DECOMPRESS_CHECKBOX] = DECOMPRESS_FLAG;
			dest[IDC_SC_SETTING_CHECKBOX] = SETTING_FLAG;
			initialized = true;
		}
		return dest;
	}
	
	/* ----------------------------------------------------------------- */
	//  detail_map
	/* ----------------------------------------------------------------- */
	inline flag_map& detail_map() {
		static bool initialized = false;
		static flag_map dest;
		if (!initialized) {
			dest[IDC_OVERWRITE_CHECKBOX] = DETAIL_OVERWRITE;
			dest[IDC_NEWER_CHECKBOX] = DETAIL_IGNORE_NEWER;
			dest[IDC_POSTOPEN_CHECKBOX] = DETAIL_OPEN;
			dest[IDC_CREATE_FOLDER_CHECKBOX] = DETAIL_CREATE_FOLDER;
			dest[IDC_SINGLE_FILE_CHECKBOX] = DETAIL_SINGLE_FILE;
			dest[IDC_SINGLE_FOLDER_CHECKBOX] = DETAIL_SINGLE_FOLDER;
			dest[IDC_SKIP_DESKTOP_CHECKBOX] = DETAIL_SKIP_DESKTOP;
			dest[IDC_CHARCODE_CHECKBOX] = DETAIL_CHARCODE;
			dest[IDC_FILTER_CHECKBOX] = DETAIL_FILTER;
			dest[IDC_REPORT_CHECKBOX] = DETAIL_REPORT;
			dest[IDC_MAIL_REMOVE_CHECKBOX] = DETAIL_MAIL_REMOVE;
			dest[IDC_TOOLTIP_CHECKBOX] = DETAIL_TOOLTIP;
			dest[IDC_REMOVE_SRC_CHECKBOX] = DETAIL_REMOVE_SRC;
			initialized = true;
		}
		return dest;
	}
	
	/* ----------------------------------------------------------------- */
	//  output_map
	/* ----------------------------------------------------------------- */
	inline flag_map& output_map() {
		static bool initialized = false;
		static flag_map dest;
		if (!initialized) {
			dest[IDC_SPECIFIC_RADIO] = OUTPUT_SPECIFIC;
			dest[IDC_SOURCE_RADIO] = OUTPUT_SOURCE;
			dest[IDC_RUNTIME_RADIO] = OUTPUT_RUNTIME;
			initialized = true;
		}
		return dest;
	}
}

#endif // CUBEICE_DIALOG_H