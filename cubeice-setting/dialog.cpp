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
			case WM_DESTROY:
			case WM_CLOSE:
				PostQuitMessage(0);
				break;
			case WM_NOTIFY:
			{
				NMHDR* nmhdr = (NMHDR *)lp;
				if (nmhdr->code == PSN_APPLY) {
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
		//  general_dialogproc
		/* ----------------------------------------------------------------- */
		static BOOL CALLBACK general_dialogproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			switch (msg) {
			case WM_COMMAND:
				break;
			default:
				switch (LOWORD(wp)) {
				case IDC_ZIP_CHECKBOX: // *.zip
					// チェックボックスのチェックの有無によって変える．
					Setting.decompression().flags() |= ZIP_FLAG;
					// Setting.decompression().flags() &= ~ZIP_FLAG;
					break;
				default:
					return common_dialogproc(hWnd, msg, wp, lp);
				}
			}
			return FALSE;
		}

		/* ----------------------------------------------------------------- */
		//  compress_dialogproc
		/* ----------------------------------------------------------------- */
		static BOOL CALLBACK archive_dialogproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			switch (msg) {
			case WM_COMMAND:
				break;
			default:
				return common_dialogproc(hWnd, msg, wp, lp);
			}
			return FALSE;
		}
		
		/* ----------------------------------------------------------------- */
		//  decompress_dialogproc
		/* ----------------------------------------------------------------- */
		static BOOL CALLBACK expand_dialogproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			switch (msg) {
			case WM_COMMAND:
				break;
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