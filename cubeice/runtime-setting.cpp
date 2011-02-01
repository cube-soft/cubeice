/* ------------------------------------------------------------------------- */
/*
 *  runtime-setting.cpp
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
#include "dialog.h"

namespace cubeice {
	namespace dialog {
		namespace compress {
			namespace detail {
				/* --------------------------------------------------------- */
				//  extension
				/* --------------------------------------------------------- */
				std::basic_string<TCHAR> extension(const std::basic_string<TCHAR>& filetype) {
					std::basic_string<TCHAR> ext = _T(".");
					if (filetype == _T("gzip")) ext += _T("gz");
					else if (filetype == _T("bzip2")) ext += _T("bz2");
					else ext += filetype;
					if (filetype == _T("gzip") || filetype == _T("bzip2")) ext = _T(".tar") + ext;
					return ext;
				}
			}
			/* ----------------------------------------------------------------- */
			/*
			 *  runtime_setting_initdialog
			 *
			 *  WM_INITDIALOG メッセージを受信した際の処理．
			 */
			/* ----------------------------------------------------------------- */
			static INT_PTR runtime_setting_initdialog(HWND hWnd, cubeice::runtime_setting& setting) {
				// アイコン
				HICON app = LoadIcon(GetModuleHandle(NULL), _T("IDI_APP"));
				SendMessage(hWnd, WM_SETICON, 0, LPARAM(app));
				
				// 画面中央に表示
				RECT rect = {};
				GetWindowRect(hWnd, (LPRECT)&rect);
				int x = (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2;
				int y = (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2;
				SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
				
				// 出力先
				SetWindowText(GetDlgItem(hWnd, IDC_OUTPUT_TEXTBOX), setting.path().c_str());
				
				// 圧縮形式
				const cubeice::dialog_data::param_list& types = cubeice::dialog_data::compress_types();
				HWND combo = GetDlgItem(hWnd, IDC_COMPTYPE_COMBOBOX);
				std::size_t index = 0; // 初期値は「zip」
				for (std::size_t i = 0; i < types.size(); ++i) {
					SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)types.at(i).c_str());
					if (setting.type() == types.at(i)) index = i;
				}
				SendMessage(combo, CB_SETCURSEL, index, 0);
				setting.type() = types.at(index);
				
				// 圧縮レベル
				const cubeice::dialog_data::param_list& levels = cubeice::dialog_data::compress_levels();
				combo = GetDlgItem(hWnd, IDC_COMPLEVEL_COMBOBOX);
				index = 3; // 初期値は「標準」
				for (std::size_t i = 0; i < levels.size(); ++i) {
					SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)levels.at(i).c_str());
					if (setting.level() == 0) {
						if (setting.level() == i) index = i;
					}
					else if (setting.level() / 2 + 1 == i) index = i;
				}
				SendMessage(combo, CB_SETCURSEL, index, 0);
				setting.level() = (index == 0) ? index : index * 2 - 1;
				
				// 圧縮メソッド
				combo = GetDlgItem(hWnd, IDC_COMPMETHOD_COMBOBOX);
				index = 0;
				if (setting.type() != _T("zip") && setting.type() != _T("7z")) EnableWindow(combo, FALSE);
				else {
					const cubeice::dialog_data::param_list& methods = (setting.type() == _T("zip")) ?
						cubeice::dialog_data::zip_methods() : cubeice::dialog_data::sevenzip_methods();
					for (std::size_t i = 0; i < methods.size(); ++i) {
						SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)methods.at(i).c_str());
						if (setting.method() == methods.at(i)) index = i;
					}
					SendMessage(combo, CB_SETCURSEL, index, 0);
					setting.method() = methods.at(index);
				}
				
				// CPU 数
				SYSTEM_INFO info = {};
				GetSystemInfo(&info);
				HWND spin = GetDlgItem(hWnd, IDC_THREAD_UPDOWN);
				SendMessage(spin, UDM_SETRANGE, 0, MAKELONG(info.dwNumberOfProcessors, 1));
				SendMessage(spin, UDM_SETPOS, 0, MAKELONG(setting.thread_size(), 0));
				std::basic_string<TCHAR> cpu = _T("/ ") + clx::lexical_cast<std::basic_string<TCHAR> >(info.dwNumberOfProcessors);
				SetWindowText(GetDlgItem(hWnd, IDC_THREAD_MAX_LABEL), cpu.c_str());
				
				// 暗号化するかどうか
				if (setting.enable_password()) CheckDlgButton(hWnd, IDC_PASSWORD_CHECKBOX, BM_SETCHECK);
				else {
					EnableWindow(GetDlgItem(hWnd, IDC_PASSWORD_TEXTBOX), FALSE);
					EnableWindow(GetDlgItem(hWnd, IDC_CONFIRM_TEXTBOX), FALSE);
					EnableWindow(GetDlgItem(hWnd, IDC_SHOWPASS_CHECKBOX), FALSE);
					EnableWindow(GetDlgItem(hWnd, IDC_ENMETHOD_COMBOBOX), FALSE);
				}
				
				// パスワードを表示するかどうか
				SendMessage(GetDlgItem(hWnd, IDC_CONFIRM_TEXTBOX), EM_SETPASSWORDCHAR, (WPARAM)_T('*'), 0);
				if (setting.show_password()) {
					CheckDlgButton(hWnd, IDC_SHOWPASS_CHECKBOX, BM_SETCHECK);
					SendMessage(GetDlgItem(hWnd, IDC_PASSWORD_TEXTBOX), EM_SETPASSWORDCHAR, (WPARAM)_T('\0'), 0);
					EnableWindow(GetDlgItem(hWnd, IDC_CONFIRM_TEXTBOX), FALSE);
				}
				else SendMessage(GetDlgItem(hWnd, IDC_PASSWORD_TEXTBOX), EM_SETPASSWORDCHAR, (WPARAM)_T('*'), 0);
				
				// 暗号化メソッド
				const cubeice::dialog_data::param_list& encodings = cubeice::dialog_data::encode_methods();
				combo = GetDlgItem(hWnd, IDC_ENMETHOD_COMBOBOX);
				index = 0;
				for (std::size_t i = 0; i < encodings.size(); ++i) {
					SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)encodings.at(i).c_str());
					if (setting.encoding() == encodings.at(i)) index = i;
				}
				SendMessage(combo, CB_SETCURSEL, index, 0);
				if (setting.type() != _T("zip")) EnableWindow(combo, FALSE);
				
				return TRUE;
			}
			
			/* ----------------------------------------------------------------- */
			/*
			 *  runtime_setting_termdialog
			 *
			 *  ダイアログの状態を setting に保存する．
			 */
			/* ----------------------------------------------------------------- */
			static INT_PTR runtime_setting_termdialog(HWND hWnd, cubeice::runtime_setting& setting) {
				// 出力先
				TCHAR path[2048] = {};
				GetDlgItemText(hWnd, IDC_OUTPUT_TEXTBOX, path, 2048);
				setting.path() = path;
				
				// 圧縮形式
				const cubeice::dialog_data::param_list& types = cubeice::dialog_data::compress_types();
				std::size_t index = SendMessage(GetDlgItem(hWnd, IDC_COMPTYPE_COMBOBOX), CB_GETCURSEL, 0, 0);
				setting.type() = types.at(index);
				
				// 圧縮レベル
				index = SendMessage(GetDlgItem(hWnd, IDC_COMPLEVEL_COMBOBOX), CB_GETCURSEL, 0, 0);
				if (index == 0) setting.level() = index;
				else setting.level() = index * 2 - 1;
				
				// 圧縮メソッド
				HWND combo = GetDlgItem(hWnd, IDC_COMPMETHOD_COMBOBOX);
				if (IsWindowEnabled(combo)) {
					TCHAR buffer[32] = {};
					index = SendMessage(combo, CB_GETCURSEL, 0, 0);
					SendMessage(combo, CB_GETLBTEXT, index, (LPARAM)buffer);
					setting.method() = buffer;
				}
				else setting.method().erase();
				
				// CPU スレッド数
				setting.thread_size() = SendMessage(GetDlgItem(hWnd, IDC_THREAD_UPDOWN), UDM_GETPOS, 0, 0);
				
				// パスワード
				if (IsDlgButtonChecked(hWnd, IDC_PASSWORD_CHECKBOX)) {
					TCHAR pass[128] = {};
					GetDlgItemText(hWnd, IDC_PASSWORD_TEXTBOX, pass, 2048);
					setting.enable_password() = true;
					setting.password() = pass;
				}
				else {
					setting.enable_password() = false;
					setting.password().erase();
				}
				
				// 設定の保存のために，使用していない場合でも状態を取得する
				setting.show_password() = (IsDlgButtonChecked(hWnd, IDC_SHOWPASS_CHECKBOX) == TRUE);
				const cubeice::dialog_data::param_list& encodings = cubeice::dialog_data::encode_methods();
				index = SendMessage(GetDlgItem(hWnd, IDC_ENMETHOD_COMBOBOX), CB_GETCURSEL, 0, 0);
				setting.encoding() = encodings.at(index);
				
				return TRUE;
			}
			
			/* ----------------------------------------------------------------- */
			//  is_valid_password
			/* ----------------------------------------------------------------- */
			static bool is_valid_password(HWND hWnd) {
				TCHAR buffer[256] = {};
				GetDlgItemText(hWnd, IDC_PASSWORD_TEXTBOX, buffer, 256);
				std::basic_string<TCHAR> pass(buffer);
				if (pass.empty()) {
					MessageBox(hWnd, _T("パスワードを入力して下さい。"), _T("エラー"), MB_OK | MB_ICONERROR);
					return false;
				}
				else if (!IsDlgButtonChecked(hWnd, IDC_SHOWPASS_CHECKBOX)) {
					if (GetDlgItemText(hWnd, IDC_CONFIRM_TEXTBOX, buffer, 256)) {
						std::basic_string<TCHAR> confirm(buffer);
						if (pass != confirm) {
							MessageBox(hWnd, _T("パスワードが一致しません。"), _T("エラー"), MB_OK | MB_ICONERROR);
							return false;
						}
					}
				}
				else if (GetDlgItem(hWnd, IDC_CONFIRM_TEXTBOX) != NULL) { // 圧縮用パスワードダイアログ
					for (std::basic_string<TCHAR>::const_iterator pos = pass.begin(); pos != pass.end(); ++pos) {
						if (*pos > 0x007f) {
							if (MessageBox(hWnd, _T("パスワードに半角英数以外の文字を使用すると、環境によっては解凍できなくなる場合があります。"), _T("パスワードの確認"), MB_OKCANCEL | MB_ICONWARNING) == IDOK) return true;
							else return false;
						}
					}
				}
				
				return true;
			}
			
			/* ----------------------------------------------------------------- */
			/*
			 *  runtime_setting_command
			 *
			 *  WM_COMMAND メッセージを受信した際の処理．
			 */
			/* ----------------------------------------------------------------- */
			static INT_PTR runtime_setting_command(HWND hWnd, WPARAM wp, LPARAM lp, cubeice::runtime_setting& setting) {
				switch (LOWORD(wp)) {
				case IDCANCEL:
					EndDialog(hWnd, IDCANCEL);
					break;
				case IDOK:
				{
					if (IsDlgButtonChecked(hWnd, IDC_PASSWORD_CHECKBOX)) {
						if (!is_valid_password(hWnd)) break;
						TCHAR buffer[256] = {};
						GetDlgItemText(hWnd, IDC_PASSWORD_TEXTBOX, buffer, 256);
						setting.password() = buffer;
					}
					
					runtime_setting_termdialog(hWnd, setting);
					EndDialog(hWnd, IDOK);
					break;
				}
				case IDC_OUTPUT_BUTTON: // 出力先を選択
				{
					const TCHAR filter[] = _T("All files(*.*)\0*.*\0\0");
					std::basic_string<TCHAR> path = cubeice::dialog::savefile(filter, setting.path().c_str());
					if (!path.empty()) setting.path() = path;
					SetWindowText(GetDlgItem(hWnd, IDC_OUTPUT_TEXTBOX), setting.path().c_str());
					break;
				}
				case IDC_COMPTYPE_COMBOBOX:
				{
					const cubeice::dialog_data::param_list& types = cubeice::dialog_data::compress_types();
					std::size_t index = SendMessage(GetDlgItem(hWnd, IDC_COMPTYPE_COMBOBOX), CB_GETCURSEL, 0, 0);
					if (types.at(index) == _T("zip") || types.at(index) == _T("7z")) {
						HWND combo = GetDlgItem(hWnd, IDC_COMPMETHOD_COMBOBOX);
						std::size_t cursel = SendMessage(combo, CB_GETCURSEL, 0, 0);
						std::size_t count = SendMessage(combo, CB_GETCOUNT, 0, 0);
						
						// いったん全ての圧縮メソッドを削除して再設定
						while(SendMessage(combo, CB_GETCOUNT, 0, 0) != 0) SendMessage(combo, CB_DELETESTRING, 0, 0);
						const cubeice::dialog_data::param_list& methods = (types.at(index) == _T("zip")) ?
							cubeice::dialog_data::zip_methods() : cubeice::dialog_data::sevenzip_methods();
						for (std::size_t i = 0; i < methods.size(); ++i) {
							SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)methods.at(i).c_str());
						}
						EnableWindow(GetDlgItem(hWnd, IDC_COMPMETHOD_COMBOBOX), TRUE);
						
						// TODO: 圧縮形式が変わったときの圧縮メソッドのカーソルはどうするか？
						if (SendMessage(combo, CB_GETCOUNT, 0, 0) != count) cursel = 0;
						SendMessage(combo, CB_SETCURSEL, cursel, 0);
					}
					else EnableWindow(GetDlgItem(hWnd, IDC_COMPMETHOD_COMBOBOX), FALSE);
					
					// 出力パスの設定
					std::basic_string<TCHAR> path = setting.path().substr(0, setting.path().find_last_of(_T('.')));
					if (path.find(_T(".tar")) != std::basic_string<TCHAR>::npos) path.erase(path.find_last_of(_T('.')));
					setting.path() = path + detail::extension(types.at(index));
					SetWindowText(GetDlgItem(hWnd, IDC_OUTPUT_TEXTBOX), setting.path().c_str());
					
					// 暗号化メソッドの有効/無効
					BOOL enabled = (IsDlgButtonChecked(hWnd, IDC_PASSWORD_CHECKBOX) && types.at(index) == _T("zip")) ? TRUE : FALSE;
					EnableWindow(GetDlgItem(hWnd, IDC_ENMETHOD_COMBOBOX), enabled);
					break;
				}
				case IDC_PASSWORD_CHECKBOX: // パスワードを設定する
				{
					BOOL enabled = IsDlgButtonChecked(hWnd, IDC_PASSWORD_CHECKBOX);
					EnableWindow(GetDlgItem(hWnd, IDC_PASSWORD_TEXTBOX), enabled);
					EnableWindow(GetDlgItem(hWnd, IDC_CONFIRM_TEXTBOX), enabled);
					EnableWindow(GetDlgItem(hWnd, IDC_SHOWPASS_CHECKBOX), enabled);
					
					// 暗号化メソッドは zip 以外は無効
					const cubeice::dialog_data::param_list& types = cubeice::dialog_data::compress_types();
					std::size_t index = SendMessage(GetDlgItem(hWnd, IDC_COMPTYPE_COMBOBOX), CB_GETCURSEL, 0, 0);
					if (types.at(index) != _T("zip")) enabled = FALSE;
					EnableWindow(GetDlgItem(hWnd, IDC_ENMETHOD_COMBOBOX), enabled);
					break;
				}
				case IDC_SHOWPASS_CHECKBOX: // パスワードを表示する
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
				return FALSE;
			}
			
			/* ----------------------------------------------------------------- */
			//  runtime_setting_wndproc
			/* ----------------------------------------------------------------- */
			static INT_PTR CALLBACK runtime_setting_wndproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
				static cubeice::runtime_setting* setting = NULL;
				switch (msg) {
				case WM_INITDIALOG:
					setting = reinterpret_cast<cubeice::runtime_setting*>(lp);
					return runtime_setting_initdialog(hWnd, *setting);
				case WM_COMMAND:
					return runtime_setting_command(hWnd, wp, lp, *setting);
				default:
					break;
				}
				
				return FALSE;
			}
		}
		
		/* ----------------------------------------------------------------- */
		//  runtime_setting
		/* ----------------------------------------------------------------- */
		int runtime_setting(cubeice::runtime_setting& setting) {
			return DialogBoxParam(GetModuleHandle(NULL), _T("IDD_RUNTIME_COMPRESS"), NULL, compress::runtime_setting_wndproc, (LPARAM)(&setting));
		}
	}
}
