/* ------------------------------------------------------------------------- */
/*
 *  dialog.cpp
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
#include <cstdlib>
#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include "dialog.h"

namespace cubeice {
	namespace detail {
		/* ---------------------------------------------------------------- */
		// dialog_init
		// 
		// Setttingの値を見て、checkBox等の初期状態を代入する。
		// （この時点で既にSettingsの値はレジストリから取得できているようである）
		/* ---------------------------------------------------------------- */
		static void dialog_init(HWND hWnd) {
#if 0
			char buffer[32] = {};
			MessageBox(NULL, itoa(Setting.context_flags(), buffer, 16), "info", MB_OK);
#endif
			// 一般タグの「関連付け」のタグ
			if (Setting.decompression().flags() & ZIP_FLAG) {
				CheckDlgButton(hWnd, IDC_ZIP_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & LZH_FLAG) {
				CheckDlgButton(hWnd, IDC_LZH_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & RAR_FLAG) {
				CheckDlgButton(hWnd, IDC_RAR_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & TAR_FLAG) {
				CheckDlgButton(hWnd, IDC_TAR_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & GZ_FLAG) {
				CheckDlgButton(hWnd, IDC_GZ_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & SEVENZIP_FLAG) {
				CheckDlgButton(hWnd, IDC_SEVENZIP_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & ARJ_FLAG) {
				CheckDlgButton(hWnd, IDC_ARJ_CHCKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & BZ2_FLAG) {
				CheckDlgButton(hWnd, IDC_BZ2_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & CAB_FLAG) {
				CheckDlgButton(hWnd, IDC_CAB_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & CHM_FLAG) {
				CheckDlgButton(hWnd, IDC_CHM_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & CPIO_FLAG) {
				CheckDlgButton(hWnd, IDC_CPIO_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & DEB_FLAG) {
				CheckDlgButton(hWnd, IDC_DEB_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & DMG_FLAG) {
				CheckDlgButton(hWnd, IDC_DMG_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & ISO_FLAG) {
				CheckDlgButton(hWnd, IDC_ISO_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & RPM_FLAG) {
				CheckDlgButton(hWnd, IDC_RPM_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & TBZ_FLAG) {
				CheckDlgButton(hWnd, IDC_TBZ_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & TGZ_FLAG) {
				CheckDlgButton(hWnd, IDC_TGZ_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & WIM_FLAG) {
				CheckDlgButton(hWnd, IDC_WIM_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & XAR_FLAG) {
				CheckDlgButton(hWnd, IDC_XAR_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.decompression().flags() & XZ_FLAG) {
				CheckDlgButton(hWnd, IDC_XZ_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.context_flags() & ARCHIVE_FLAG) {
				CheckDlgButton(hWnd, IDC_ARCHIVE_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.context_flags() & EXPAND_FLAG) {
				CheckDlgButton(hWnd, IDC_EXPAND_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.context_flags() & SETTING_FLAG) {
				CheckDlgButton(hWnd, IDC_SETTING_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.compression().output_condition() & OUTPUT_SPECIFIC) {
				CheckDlgButton(hWnd, IDC_SPECIFIC_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.compression().output_condition() & OUTPUT_SOURCE) {
				CheckDlgButton(hWnd, IDC_SOURCE_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.compression().output_condition() & OUTPUT_RUNTIME) {
				CheckDlgButton(hWnd, IDC_RUNTIME_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.compression().overwrite() & OVERWRITE_NOTIFY) {
				CheckDlgButton(hWnd, IDC_OVERWRITE_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.compression().overwrite() & OVERWRITE_NEWER) {
				CheckDlgButton(hWnd, IDC_NEWER_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.compression().conv_charset()) {
				CheckDlgButton(hWnd, IDC_CHARCODE_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.compression().filter()) {
				CheckDlgButton(hWnd, IDC_FILTER_CHECKBOX, BM_SETCHECK);
			}
			if (Setting.compression().postopen()) {
				CheckDlgButton(hWnd, IDC_POSTOPEN_CHECKBOX, BM_SETCHECK);
			}
		}

		/* ----------------------------------------------------------------- */
		/*
		 *  common_dialogproc
		 *
		 *  各種ボタンを押したときの挙動など，全てのタブページで共通の
		 *  動作を記述する．
		 */
		/* ----------------------------------------------------------------- */
		static BOOL CALLBACK common_dialogproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			switch (msg) {
			case WM_INITDIALOG:
				dialog_init(hWnd);
				break;
			case WM_DESTROY:
			case WM_CLOSE:
				PostQuitMessage(0);
				break;
			case WM_NOTIFY:
			{
				NMHDR* nmhdr = (NMHDR *)lp;
				if (nmhdr->code == PSN_APPLY) {
#if 0
					char buffer[32] = {};
					MessageBox(NULL, itoa(Setting.context_flags(), buffer, 16), "info", MB_OK);
#endif
					Setting.save();
				}
				break;
			}
			default:
				break;
			}
			return FALSE;
		}

		/* ----------------------------------------------------------------- */
		//  check_and_set_flags_for_setting
		//  general_dialogprocの補助関数
		//  「ファイルの関連付け」のチェックボックスのON/OFFに合わせてSetting.decompression().flagsの値を変更
		/* ----------------------------------------------------------------- */
		static void check_and_set_flags_for_setting(HWND hWnd, int nIDButton, unsigned int kindOfFlag) {
			if (IsDlgButtonChecked(hWnd, nIDButton) == BST_CHECKED) {
				Setting.decompression().flags() |= kindOfFlag;
			} else {
				Setting.decompression().flags() &= ~kindOfFlag;
			}
		}
		/* ----------------------------------------------------------------- */
		// check_and_set_contextflag_for_setting
		/* ----------------------------------------------------------------- */
		static void check_and_set_contextflag_for_setting(HWND hWnd, int nIDButton, unsigned int kindOfFlag) {
			if (IsDlgButtonChecked(hWnd, nIDButton) == BST_CHECKED) {
				Setting.context_flags() |= kindOfFlag;
			} else {
				Setting.context_flags() &= ~kindOfFlag;
			}
		}
		/* ----------------------------------------------------------------- */
		//  general_dialogproc
		/* ----------------------------------------------------------------- */
		static BOOL CALLBACK general_dialogproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			switch (msg) {
			case WM_COMMAND:
				switch (LOWORD(wp)) {
				case IDC_ZIP_CHECKBOX: // *.zip
					check_and_set_flags_for_setting(hWnd, IDC_ZIP_CHECKBOX, ZIP_FLAG);
					break;
				case IDC_LZH_CHECKBOX: // *.lzh
					check_and_set_flags_for_setting(hWnd, IDC_LZH_CHECKBOX, LZH_FLAG);
					break;
				case IDC_RAR_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_RAR_CHECKBOX, RAR_FLAG);
					break;
				case IDC_TAR_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_TAR_CHECKBOX, TAR_FLAG);
					break;
				case IDC_GZ_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_GZ_CHECKBOX, GZ_FLAG);
					break;
				case IDC_SEVENZIP_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_SEVENZIP_CHECKBOX, SEVENZIP_FLAG);
					break;
				case IDC_ARJ_CHCKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_ARJ_CHCKBOX, ARJ_FLAG);
					break;
				case IDC_BZ2_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_BZ2_CHECKBOX, BZ2_FLAG);
					break;
				case IDC_CAB_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_CAB_CHECKBOX, CAB_FLAG);
					break;
				case IDC_CHM_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_CHM_CHECKBOX, CHM_FLAG);
					break;
				case IDC_CPIO_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_CPIO_CHECKBOX, CPIO_FLAG);
					break;
				case IDC_DEB_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_DEB_CHECKBOX, DEB_FLAG);
					break;
				case IDC_DMG_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_DMG_CHECKBOX, DMG_FLAG);
					break;
				case IDC_ISO_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_ISO_CHECKBOX, ISO_FLAG);
					break;
				case IDC_RPM_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_RPM_CHECKBOX, RPM_FLAG);
					break;
				case IDC_TBZ_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_TBZ_CHECKBOX, TBZ_FLAG);
					break;
				case IDC_TGZ_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_TGZ_CHECKBOX, TGZ_FLAG);
					break;
				case IDC_WIM_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_WIM_CHECKBOX, WIM_FLAG);
					break;
				case IDC_XAR_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_XAR_CHECKBOX, XAR_FLAG);
					break;
				case IDC_XZ_CHECKBOX:
					check_and_set_flags_for_setting(hWnd, IDC_XZ_CHECKBOX, XZ_FLAG);
					break;
				case IDC_ARCHIVE_CHECKBOX:
					check_and_set_contextflag_for_setting(hWnd, IDC_ARCHIVE_CHECKBOX, ARCHIVE_FLAG);
					break;
				case IDC_EXPAND_CHECKBOX:
					check_and_set_contextflag_for_setting(hWnd, IDC_EXPAND_CHECKBOX, EXPAND_FLAG);
					break;
				case IDC_SETTING_CHECKBOX:
					check_and_set_contextflag_for_setting(hWnd, IDC_SETTING_CHECKBOX, SETTING_FLAG);
					break;
				default:
					return common_dialogproc(hWnd, msg, wp, lp);
				}
			default:
				return common_dialogproc(hWnd, msg, wp, lp);
			}
			return FALSE;
		}
		
		

		/* ----------------------------------------------------------------- */
		//  check_and_set_compress_outputcondition_for_setting
		/* ----------------------------------------------------------------- */
		static void check_and_set_compress_outputcondition_for_setting(HWND hWnd, int nIDButton, unsigned int kindOfFlag) {
			if (IsDlgButtonChecked(hWnd, nIDButton) == BST_CHECKED) {
				Setting.compression().output_condition() |= kindOfFlag;
			} else {
				Setting.compression().output_condition() &= ~kindOfFlag;
			}
		}
		/* ----------------------------------------------------------------- */
		//  check_and_set_compress_overwrite_for_setting
		/* ----------------------------------------------------------------- */
		static void check_and_set_compress_overwrite_for_setting(HWND hWnd, int nIDButton, unsigned int kindOfFlag) {
			if (IsDlgButtonChecked(hWnd, nIDButton) == BST_CHECKED) {
				Setting.compression().overwrite() |= kindOfFlag;
			} else {
				Setting.compression().overwrite() &= ~kindOfFlag;
			}
		}

		/* ----------------------------------------------------------------- */
		//  compress_dialogproc
		/* ----------------------------------------------------------------- */
		static BOOL CALLBACK archive_dialogproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			switch (msg) {
			case WM_COMMAND:
				switch(LOWORD(wp)) {
				case IDC_SPECIFIC_CHECKBOX:
					check_and_set_compress_outputcondition_for_setting(hWnd, IDC_SPECIFIC_CHECKBOX, OUTPUT_SPECIFIC);
					break;
				case IDC_SOURCE_CHECKBOX:
					check_and_set_compress_outputcondition_for_setting(hWnd, IDC_SOURCE_CHECKBOX, OUTPUT_SOURCE);
					break;
				case IDC_RUNTIME_CHECKBOX:
					check_and_set_compress_outputcondition_for_setting(hWnd, IDC_RUNTIME_CHECKBOX, OUTPUT_RUNTIME);
					break;
				case IDC_FILTER_CHECKBOX:
					Setting.compression().filter() = (IsDlgButtonChecked(hWnd, IDC_FILTER_CHECKBOX) == BST_CHECKED);
					break;
				case IDC_OVERWRITE_CHECKBOX:
					check_and_set_compress_overwrite_for_setting(hWnd, IDC_OVERWRITE_CHECKBOX, OVERWRITE_NOTIFY);
					break;
				case IDC_NEWER_CHECKBOX:
					check_and_set_compress_overwrite_for_setting(hWnd, IDC_NEWER_CHECKBOX, OVERWRITE_NEWER);
					break;
				case IDC_POSTOPEN_CHECKBOX:
					Setting.compression().postopen() = (IsDlgButtonChecked(hWnd, IDC_POSTOPEN_CHECKBOX) == BST_CHECKED);
					break;
				default:
					return common_dialogproc(hWnd, msg, wp, lp);
				}
			default:
				return common_dialogproc(hWnd, msg, wp, lp);
			}
			return FALSE;
		}
		
		/* ----------------------------------------------------------------- */
		//  check_and_set_decompress_outputcondition_for_setting
		/* ----------------------------------------------------------------- */
		static void check_and_set_decompress_overwrite_for_setting(HWND hWnd, int nIDButton, unsigned int kindOfFlag) {
			if (IsDlgButtonChecked(hWnd, nIDButton) == BST_CHECKED) {
				Setting.decompression().overwrite() |= kindOfFlag;
			} else {
				Setting.decompression().overwrite() &= ~kindOfFlag;
			}
		}

		/* ----------------------------------------------------------------- */
		//  check_and_set_decompress_outputcondition_for_setting
		/* ----------------------------------------------------------------- */
		static void check_and_set_decompress_outputcondition_for_setting(HWND hWnd, int nIDButton, unsigned int kindOfFlag) {
			if (IsDlgButtonChecked(hWnd, nIDButton) == BST_CHECKED) {
				Setting.decompression().output_condition() |= kindOfFlag;
			} else {
				Setting.decompression().output_condition() &= ~kindOfFlag;
			}
		}
		/* ----------------------------------------------------------------- */
		//  decompress_dialogproc
		/* ----------------------------------------------------------------- */
		static BOOL CALLBACK expand_dialogproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			switch (msg) {
			case WM_COMMAND:
				switch(LOWORD(wp)) {
				case IDC_SPECIFIC_CHECKBOX:
					check_and_set_decompress_outputcondition_for_setting(hWnd, IDC_SPECIFIC_CHECKBOX, OUTPUT_SPECIFIC);
					break;
				case IDC_SOURCE_CHECKBOX:
					check_and_set_decompress_outputcondition_for_setting(hWnd, IDC_SOURCE_CHECKBOX, OUTPUT_SOURCE);
					break;
				case IDC_RUNTIME_CHECKBOX:
					check_and_set_decompress_outputcondition_for_setting(hWnd, IDC_RUNTIME_CHECKBOX, OUTPUT_RUNTIME);
					break;
				case IDC_FILTER_CHECKBOX:
					Setting.decompression().filter() = (IsDlgButtonChecked(hWnd, IDC_FILTER_CHECKBOX) == BST_CHECKED);
					break;
				case IDC_OVERWRITE_CHECKBOX:
					check_and_set_decompress_overwrite_for_setting(hWnd, IDC_OVERWRITE_CHECKBOX, OVERWRITE_NOTIFY);
					break;
				case IDC_NEWER_CHECKBOX:
					check_and_set_decompress_overwrite_for_setting(hWnd, IDC_NEWER_CHECKBOX, OVERWRITE_NEWER);
					break;
				case IDC_MAKE_FOLDER_CHECKBOX:
					Setting.decompression().create_folder() = (IsDlgButtonChecked(hWnd, IDC_MAKE_FOLDER_CHECKBOX) == BST_CHECKED);
					break;
				case IDC_CHARCODE_CHECKBOX:
					Setting.decompression().conv_charset() = (IsDlgButtonChecked(hWnd, IDC_CHARCODE_CHECKBOX) == BST_CHECKED);
					break;
				case IDC_POSTOPEN_CHECKBOX:
					Setting.decompression().postopen() = (IsDlgButtonChecked(hWnd, IDC_POSTOPEN_CHECKBOX) == BST_CHECKED);
					break;
				default:
					return common_dialogproc(hWnd, msg, wp, lp);
				}
			default:
				return common_dialogproc(hWnd, msg, wp, lp);
			}
			return FALSE;
		}
		
		/* ----------------------------------------------------------------- */
		//  filter_dialogproc
		/* ----------------------------------------------------------------- */
		static BOOL CALLBACK filter_dialogproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			switch (msg) {
			case WM_COMMAND:
				break;
			default:
				return common_dialogproc(hWnd, msg, wp, lp);
			}
			return FALSE;
		}

		/* ----------------------------------------------------------------- */
		//  version_dialogproc
		/* ----------------------------------------------------------------- */
		static BOOL CALLBACK version_dialogproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			switch (msg) {
			case WM_INITDIALOG:
			{
				break;
			}
			case WM_COMMAND:
				break;
			default:
				return common_dialogproc(hWnd, msg, wp, lp);
			}
			return FALSE;
		}
	}
	
	/* --------------------------------------------------------------------- */
	//  create_propsheet
	/* --------------------------------------------------------------------- */
	int create_propsheet(HWND parent) {
		static const std::size_t pagenum = 5;
		HPROPSHEETPAGE ref[pagenum] = {};
		
		PROPSHEETPAGE page = {};
		page.dwSize = sizeof (PROPSHEETPAGE);
		page.dwFlags = PSP_DEFAULT | PSP_USEICONID;
		page.hInstance = GetModuleHandle(NULL);
		
		// 「一般」ページ
		page.pszTemplate = _T("GENERAL_TABPAGE");
		page.pfnDlgProc = (DLGPROC)detail::general_dialogproc;
		ref[0] = CreatePropertySheetPage(&page);
		
		// 「圧縮」ページ
		page.pszTemplate = _T("ARCHIVE_TABPAGE");
		page.pfnDlgProc = (DLGPROC)detail::archive_dialogproc;
		ref[1] = CreatePropertySheetPage(&page);
		
		// 「解凍」ページ
		page.pszTemplate = _T("EXPAND_TABPAGE");
		page.pfnDlgProc = (DLGPROC)detail::expand_dialogproc;
		ref[2] = CreatePropertySheetPage(&page);
		
		// 「フィルタリング」ページ
		page.pszTemplate = _T("FILTER_TABPAGE");
		page.pfnDlgProc = (DLGPROC)detail::filter_dialogproc;
		ref[3] = CreatePropertySheetPage(&page);

		// 「バージョン情報」ページ
		page.pszTemplate = _T("VERSION_TABPAGE");
		page.pfnDlgProc = (DLGPROC)detail::version_dialogproc;;
		ref[4] = CreatePropertySheetPage(&page);
		
		PROPSHEETHEADER psh = {};
		psh.dwSize = sizeof (PROPSHEETHEADER);
		psh.dwFlags = PSH_DEFAULT | PSH_USEHICON;
		psh.hwndParent = parent;
		psh.hIcon = LoadIcon(page.hInstance, _T("CUBEICE_ICON"));
		psh.pszCaption = _T("CubeICE 設定");
		psh.nPages = pagenum;
		psh.phpage = ref;
		
		return PropertySheet(&psh);
	}
}