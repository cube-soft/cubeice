/* ------------------------------------------------------------------------- */
/*
 *  password.h
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
 *  Last-modified: Mon 20 Dec 2010 22:05:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_PASSWORD_H
#define CUBEICE_PASSWORD_H

namespace cubeice {
	/* --------------------------------------------------------------------- */
	/*
	 *  password
	 *
	 *  NOTE: 現状は，全てのパスワードダイアログが一つのパスワード記憶
	 *  領域を共有している．
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<TCHAR>& password() {
		static std::basic_string<TCHAR> pass;
		return pass;
	}
	
	/* --------------------------------------------------------------------- */
	//  password_wndproc
	/* --------------------------------------------------------------------- */
	static BOOL CALLBACK password_wndproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
		switch (msg) {
		case WM_INITDIALOG:
		{
			// アイコン
			HICON app = LoadIcon(GetModuleHandle(NULL), _T("IDI_APP"));
			SendMessage(hWnd, WM_SETICON, 0, LPARAM(app));
			HICON info = LoadIcon(NULL, IDI_INFORMATION);
			HWND pic = GetDlgItem(hWnd, IDC_ICON_PICTUREBOX);
			SendMessage(pic, STM_SETIMAGE, IMAGE_ICON, LPARAM(info));
			
			// パスワード文字を変更
			SendMessage(GetDlgItem(hWnd, IDC_PASSWORD_TEXTBOX), EM_SETPASSWORDCHAR, (WPARAM)_T('*'), 0);
			SendMessage(GetDlgItem(hWnd, IDC_CONFIRM_TEXTBOX), EM_SETPASSWORDCHAR, (WPARAM)_T('*'), 0);
			
			if (lp == 0x02) { // 解凍用
				CheckDlgButton(hWnd, IDC_SHOWPASS_CHECKBOX, BM_SETCHECK);
				EnableWindow(GetDlgItem(hWnd, IDC_CONFIRM_TEXTBOX), FALSE);
				SendMessage(GetDlgItem(hWnd, IDC_PASSWORD_TEXTBOX), EM_SETPASSWORDCHAR, (WPARAM)_T('\0'), 0);
			}
			
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
			case IDOK:
			{
				// NOTE: パスワード表示状態で入力した場合は整合性をチェックしない
				TCHAR buffer[256] = {};
				GetDlgItemText(hWnd, IDC_PASSWORD_TEXTBOX, buffer, 256);
				std::basic_string<TCHAR> pass(buffer);
				if (!IsDlgButtonChecked(hWnd, IDC_SHOWPASS_CHECKBOX)) {
					GetDlgItemText(hWnd, IDC_CONFIRM_TEXTBOX, buffer, 256);
					std::basic_string<TCHAR> confirm(buffer);
					if (pass != confirm) {
						MessageBox(hWnd, _T("パスワードが一致しません。"), _T("エラー"), MB_OK | MB_ICONERROR);
						break;
					}
				}
				cubeice::password() = pass;
				EndDialog(hWnd, IDOK);
				break;
			}
			case IDCANCEL:
				EndDialog(hWnd, IDCANCEL);
				break;
			case IDC_SHOWPASS_CHECKBOX:
			{
				BOOL enabled = IsDlgButtonChecked(hWnd, IDC_SHOWPASS_CHECKBOX);
				EnableWindow(GetDlgItem(hWnd, IDC_CONFIRM_TEXTBOX), !enabled);
				TCHAR secret = enabled ? _T('\0') : _T('*');
				SendMessage(GetDlgItem(hWnd, IDC_PASSWORD_TEXTBOX), EM_SETPASSWORDCHAR, (WPARAM)secret, 0);
				
				// パスワード表示の切り替えが遅れるので強制的に変更する．
				TCHAR buffer[256] = {};
				GetDlgItemText(hWnd, IDC_PASSWORD_TEXTBOX, buffer, 256);
				SetDlgItemText(hWnd, IDC_PASSWORD_TEXTBOX, buffer);
				break;
			}
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
	inline INT_PTR password_dialog(int which) {
		return DialogBoxParam(GetModuleHandle(NULL), _T("IDD_PASSWORD"), NULL, password_wndproc, which);
	}
}

#endif // CUBEICE_PASSWORD_H
