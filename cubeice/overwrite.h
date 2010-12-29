/* ------------------------------------------------------------------------- */
/*
 *  overwrite.h
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
 *  Last-modified: Tue 28 Dec 2010 22:04:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_OVERWRITE_H
#define CUBEICE_OVERWRITE_H

namespace cubeice {
	/* --------------------------------------------------------------------- */
	/*
	 *  overwrite
	 *
	 *  NOTE: 現状は，全てのイアログが一つのテキスト領域を共有している．
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<TCHAR>& overwrite() {
		static std::basic_string<TCHAR> text;
		return text;
	}
	
	/* --------------------------------------------------------------------- */
	//  overwrite_wndproc
	/* --------------------------------------------------------------------- */
	static BOOL CALLBACK overwrite_wndproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
		switch (msg) {
		case WM_INITDIALOG:
		{
			// アイコン
			HICON app = LoadIcon(GetModuleHandle(NULL), _T("IDI_APP"));
			SendMessage(hWnd, WM_SETICON, 0, LPARAM(app));
			HICON info = LoadIcon(NULL, IDI_WARNING);
			HWND pic = GetDlgItem(hWnd, IDC_ICON_PICTUREBOX);
			SendMessage(pic, STM_SETIMAGE, IMAGE_ICON, LPARAM(info));
			
			// テキスト
			SetWindowText(GetDlgItem(hWnd, IDC_INFO_LABEL), overwrite().c_str());
			
			// 画面中央に表示
			RECT rect = {};
			GetWindowRect(hWnd, (LPRECT)&rect);
			int x = (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2;
			int y = (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2;
			SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
			return TRUE;
		}
		case WM_COMMAND:
			switch (LOWORD(wp)) {
			case IDYES:
			{
				int to_all = IsDlgButtonChecked(hWnd, IDC_ALL_CHECKBOX) ? ID_TO_ALL : 0;
				EndDialog(hWnd, IDOK | to_all);
				break;
			}
			case IDNO:
			{
				int to_all = IsDlgButtonChecked(hWnd, IDC_ALL_CHECKBOX) ? ID_TO_ALL : 0;
				EndDialog(hWnd, IDNO | to_all);
				break;
			}
			case IDCANCEL:
				EndDialog(hWnd, IDCANCEL);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		
		return FALSE;
	}
	
	/* --------------------------------------------------------------------- */
	//  password_dialog
	/* --------------------------------------------------------------------- */
	inline INT_PTR overwrite_dialog() {
		return DialogBox(GetModuleHandle(NULL), _T("IDD_OVERWRITE"), NULL, overwrite_wndproc);
	}
}

#endif // CUBEICE_OVERWRITE_H