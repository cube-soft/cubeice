/* ------------------------------------------------------------------------- */
/*
 *  dialog.h
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

#include <map>
#include <windows.h>
#include "resource.h"
#include "user-setting.h"

extern cubeice::user_setting Setting;

/* ------------------------------------------------------------------------- */
//  functions declarations
/* ------------------------------------------------------------------------- */
namespace cubeice {
	extern int create_propsheet(HWND parent);
	
	typedef std::map<std::size_t, std::size_t> flag_map;
	/* ----------------------------------------------------------------- */
	//  compress_map
	/* ----------------------------------------------------------------- */
	inline flag_map& compress_map() {
		static bool initialized = false;
		static flag_map dest;
		if (!initialized) {
			dest[IDC_ADD_ZIP_CHECKBOX] = ZIP_FLAG;
			dest[IDC_ADD_SEVENZIP_CHECKBOX] = SEVENZIP_FLAG;
			dest[IDC_ADD_BZ2_CHECKBOX] = BZ2_FLAG;
			dest[IDC_ADD_GZ_CHECKBOX] = GZ_FLAG;
			dest[IDC_ADD_EXE_CHECKBOX] = EXE_FLAG;
			initialized = true;
		}
		return dest;
	}
	
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
			dest[IDC_COMPRESS_CHECKBOX] = COMPRESS_FLAG;
			dest[IDC_DECOMPRESS_CHECKBOX] = DECOMPRESS_FLAG;
			dest[IDC_SETTING_CHECKBOX] = SETTING_FLAG;
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
			dest[IDC_CHARCODE_CHECKBOX] = DETAIL_CHARCODE;
			dest[IDC_FILTER_CHECKBOX] = DETAIL_FILTER;
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
			dest[IDC_SPECIFIC_CHECKBOX] = OUTPUT_SPECIFIC;
			dest[IDC_SOURCE_CHECKBOX] = OUTPUT_SOURCE;
			dest[IDC_RUNTIME_CHECKBOX] = OUTPUT_RUNTIME;
		}
		return dest;
	}
}

#endif // CUBEICE_DIALOG_H