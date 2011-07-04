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
#ifndef NO_WIN32_LEAN_AND_MEAN
#define NO_WIN32_LEAN_AND_MEAN
#include <shlobj.h>
#endif // NO_WIN32_LEAN_AND_MEAN

#include <cstdlib>
#include <algorithm>
#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include "dialog.h"
#include "clx/split.h"
#include "user-setting-converter.h"

namespace cubeice {
	/* ----------------------------------------------------------------- */
	//  browse_proc
	/* ----------------------------------------------------------------- */
	static int CALLBACK browse_proc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
		if(uMsg == BFFM_INITIALIZED){
			SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
		}
		return 0;
	}
	
	/* ----------------------------------------------------------------- */
	/*
	 *  browsefolder
	 *
	 *  see also:
	 *  http://msdn.microsoft.com/en-us/library/bb762115(VS.85).aspx
	 *  http://msdn.microsoft.com/en-us/library/bb773205(VS.85).aspx
	 */
	/* ----------------------------------------------------------------- */
	static std::basic_string<TCHAR> browsefolder(const TCHAR* description) {
		typedef TCHAR char_type;
		char_type path[CUBE_MAX_PATH] = {};
		GetCurrentDirectory(CUBE_MAX_PATH, path);
		
		BROWSEINFO info = {};
		info.pszDisplayName = path;
		info.lpszTitle = description;
		info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
		info.lpfn = &browse_proc;
		info.lParam = (LPARAM)path;
		LPITEMIDLIST dest = SHBrowseForFolder(&info);
		
		SHGetPathFromIDList(dest, path);
		CoTaskMemFree(dest);
		
		return path;
	}
	
	namespace detail {
		/* ----------------------------------------------------------------- */
		/*
		 *  change_flag
		 *
		 *  チェックボックスの変化があったときに flags の値を更新する
		 *  ための補助関数．
		 */
		/* ----------------------------------------------------------------- */
		static void change_flag(std::size_t& dest, HWND handle, int id, std::size_t kind) {
			if (IsDlgButtonChecked(handle, id) == BST_CHECKED) dest |= kind;
			else dest &= ~kind;
		}
		
		/* ---------------------------------------------------------------- */
		//  change_cxtmenu
		/* ---------------------------------------------------------------- */
		static void change_ctxmenu(HWND hWnd, bool customized) {
			const flag_map& context = context_root_map();
			for (flag_map::const_iterator pos = context.begin(); pos != context.end(); ++pos) {
				EnableWindow(GetDlgItem(hWnd, pos->first), customized ? FALSE : TRUE);
				
				// サブ項目を取得
				const flag_map& ctxsub = (pos->second == COMPRESS_FLAG) ? context_compress_map() :
					((pos->second == DECOMPRESS_FLAG) ? context_decompress_map() : context_mail_map());
				BOOL enabled = (customized || IsDlgButtonChecked(hWnd, pos->first) == BST_UNCHECKED) ? FALSE : TRUE;
				for (flag_map::const_iterator subpos = ctxsub.begin(); subpos != ctxsub.end(); ++subpos) {
						EnableWindow(GetDlgItem(hWnd, subpos->first), enabled);
				}
			}
		}
		
		/* ---------------------------------------------------------------- */
		/*
		 *  archive_initdialog
		 *
		 *  「圧縮」タブおよび「解凍」タブの初期設定
		 */
		/* ---------------------------------------------------------------- */
		static void archive_initdialog(HWND hWnd, user_setting::archive_type& setting) {
			// 「出力先フォルダ」グループ
			if (setting.output_condition() == OUTPUT_SPECIFIC) {
				CheckDlgButton(hWnd, IDC_SPECIFIC_RADIO, BM_SETCHECK);
				EnableWindow(GetDlgItem(hWnd, IDC_DEST_TEXTBOX), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_DEST_BUTTON), TRUE);
			}
			else if (setting.output_condition() == OUTPUT_SOURCE) {
				CheckDlgButton(hWnd, IDC_SOURCE_RADIO, BM_SETCHECK);
				EnableWindow(GetDlgItem(hWnd, IDC_DEST_TEXTBOX), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_DEST_BUTTON), FALSE);
			}
			else {
				CheckDlgButton(hWnd, IDC_RUNTIME_RADIO, BM_SETCHECK);
				EnableWindow(GetDlgItem(hWnd, IDC_DEST_TEXTBOX), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_DEST_BUTTON), FALSE);
			}
			
			if (!setting.output_path().empty()) {
				SetWindowText(GetDlgItem(hWnd, IDC_DEST_TEXTBOX), setting.output_path().c_str());
			}
			
			// 「詳細」グループ
			const flag_map& detail = detail_map();
			for (flag_map::const_iterator pos = detail.begin(); pos != detail.end(); ++pos) {
				if (setting.details() & pos->second) CheckDlgButton(hWnd, pos->first, BM_SETCHECK);
			}
			
			if ((setting.details() & DETAIL_OVERWRITE) == 0) {
				EnableWindow(GetDlgItem(hWnd, IDC_NEWER_CHECKBOX), FALSE);
			}
			
			if ((setting.details() & DETAIL_OPEN) == 0) {
				EnableWindow(GetDlgItem(hWnd, IDC_SKIP_DESKTOP_CHECKBOX), FALSE);
			}
		}

		/* ---------------------------------------------------------------- */
		/*
		 *  general_initdialog
		 * 
		 *  「一般」タブの初期設定
		 */
		/* ---------------------------------------------------------------- */
		static void general_initdialog(HWND hWnd) {
			// 「関連付け」グループ
			const flag_map& decomp = decompress_map();
			for (flag_map::const_iterator pos = decomp.begin(); pos != decomp.end(); ++pos) {
				if (Setting.decompression().flags() & pos->second) {
					CheckDlgButton(hWnd, pos->first, BM_SETCHECK);
				}
			}
			
			// 「コンテキストメニュー」グループ
			const flag_map& context = context_map();
			const flag_map& ctxcomp = context_compress_map();
			const flag_map& ctxmail = context_mail_map();
			const flag_map& ctxdecomp = context_decompress_map();
			for (flag_map::const_iterator pos = context.begin(); pos != context.end(); ++pos) {
				if (Setting.context_flags() & pos->second) {
					CheckDlgButton(hWnd, pos->first, BM_SETCHECK);
				}
				else if (pos->second == COMPRESS_FLAG) {
					for (flag_map::const_iterator subpos = ctxcomp.begin(); subpos != ctxcomp.end(); ++subpos) {
						EnableWindow(GetDlgItem(hWnd, subpos->first), FALSE);
					}
				}
				else if (pos->second == DECOMPRESS_FLAG) {
					for (flag_map::const_iterator subpos = ctxdecomp.begin(); subpos != ctxdecomp.end(); ++subpos) {
						EnableWindow(GetDlgItem(hWnd, subpos->first), FALSE);
					}
				}
				else if (pos->second == MAIL_FLAG) {
					for (flag_map::const_iterator subpos = ctxmail.begin(); subpos != ctxmail.end(); ++subpos) {
						EnableWindow(GetDlgItem(hWnd, subpos->first), FALSE);
					}
				}
			}
			change_ctxmenu(hWnd, Setting.context_customize());
			
			// 「ショートカット」グループ
			const flag_map& shortcut = shortcut_map();
			for (flag_map::const_iterator pos = shortcut.begin(); pos != shortcut.end(); ++pos) {
				if (Setting.shortcut_flags() & pos->second) {
					CheckDlgButton(hWnd, pos->first, BM_SETCHECK);
				}
			}
			
			// ショートカットの圧縮の種類
			HWND combo = GetDlgItem(hWnd, IDC_SC_TYPE_COMBOBOX);
			const param_map& params = compress_parameters();
			for (param_map::const_iterator pos = params.begin(); pos != params.end(); ++pos) {
				SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)pos->second.first.c_str());
			}
			SendMessage(combo, CB_SETCURSEL, Setting.shortcut_compress_index(), 0L);
		}
		
		/* ---------------------------------------------------------------- */
		//  filter_initdialog
		/* ---------------------------------------------------------------- */
		static void filter_initdialog(HWND hWnd) {
			if (!Setting.filters().empty()) {
				std::basic_string<TCHAR> s;
				clx::join(Setting.filters(), s, _T("\r\n"));
				HWND handle = GetDlgItem(hWnd, IDC_FILTER_TEXTBOX);
				SetWindowText(handle, s.c_str());
			}
		}
		
		/* ----------------------------------------------------------------- */
		//  filter_gettext
		/* ----------------------------------------------------------------- */
		static void filter_gettext(HWND hWnd) {
			const int maxlen = 64 * 1024;
			int len = GetWindowTextLength(GetDlgItem(hWnd, IDC_FILTER_TEXTBOX));
			if (len < maxlen) {  
				cubeice::user_setting::char_type buffer[maxlen] = {};
				std::size_t status = GetDlgItemText(hWnd, IDC_FILTER_TEXTBOX, (LPTSTR)buffer,  sizeof(buffer));
				if (status > 0 || GetLastError() == 0) {
					cubeice::user_setting::string_type s(buffer);
					Setting.filters().clear();
					clx::split_if(s, Setting.filters(), clx::is_any_of(_T("\r\n")));
				}
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
			case WM_DESTROY:
			case WM_CLOSE:
				PostQuitMessage(0);
				break;
			case WM_NOTIFY:
			{
				NMHDR* nmhdr = (NMHDR *)lp;
				if (nmhdr->code == PSN_APPLY) {
					filter_gettext(hWnd);
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
			case WM_INITDIALOG:
				general_initdialog(hWnd);
				return FALSE;
			case WM_COMMAND:
			{
				std::size_t parameter = LOWORD(wp);
				
				// 「関連付け」グループ
				const flag_map& decomp = decompress_map();
				flag_map::const_iterator pos = decomp.find(parameter);
				if (pos != decomp.end()) {
					change_flag(Setting.decompression().flags(), hWnd, pos->first, pos->second);
					Setting.associate_changed();
					return FALSE;
				}
				
				// 「全て選択/全て解除」のボタン
				if (parameter == IDC_ALLCHECK_BUTTON || parameter == IDC_ALLUNCHECK_BUTTON) {
					WPARAM check = (parameter == IDC_ALLCHECK_BUTTON) ? BST_CHECKED : BST_UNCHECKED;
					for( flag_map::const_iterator it = decomp.begin() ; it != decomp.end() ; ++it ) {
						SendDlgItemMessage( hWnd, it->first, BM_SETCHECK, check, 0 );
						change_flag(Setting.decompression().flags(), hWnd, it->first, it->second);
					}
					Setting.associate_changed();
					return FALSE;
				}
				
				// 「コンテキストメニュー」グループ
				const flag_map& context = context_map();
				pos = context.find(parameter);
				if (pos != context.end()) {
					change_flag(Setting.context_flags(), hWnd, pos->first, pos->second);
					
					if (pos->first == IDC_CT_COMPRESS_CHECKBOX) {
						BOOL enabled = (Setting.context_flags() & COMPRESS_FLAG) ? TRUE : FALSE;
						const flag_map& submenu = context_compress_map();
						for (flag_map::const_iterator subpos = submenu.begin(); subpos != submenu.end(); ++subpos) {
							EnableWindow(GetDlgItem(hWnd, subpos->first), enabled);
						}
					}
					else if (pos->first == IDC_CT_DECOMPRESS_CHECKBOX) {
						BOOL enabled = (Setting.context_flags() & DECOMPRESS_FLAG) ? TRUE : FALSE;
						const flag_map& submenu = context_decompress_map();
						for (flag_map::const_iterator subpos = submenu.begin(); subpos != submenu.end(); ++subpos) {
							EnableWindow(GetDlgItem(hWnd, subpos->first), enabled);
						}
					}
					else if (pos->first == IDC_CT_MAIL_CHECKBOX) {
						BOOL enabled = (Setting.context_flags() & MAIL_FLAG) ? TRUE : FALSE;
						const flag_map& submenu = context_mail_map();
						for (flag_map::const_iterator subpos = submenu.begin(); subpos != submenu.end(); ++subpos) {
							EnableWindow(GetDlgItem(hWnd, subpos->first), enabled);
						}
					}
					
					return FALSE;
				}
				
				// 「カスタマイズ」ボタン
				if (parameter == IDC_CUSTOMIZE_MENU_BUTTON) {
					bool prev = Setting.context_customize();
					if (!Setting.context_customize()) {
						Setting.context_submenu() = cubeice::user_setting_converter::to_customized_ctx_menu(Setting.context_flags());
						Setting.context_customize() = true;
					}
					
					if (cubeice::dialog::customize(hWnd, Setting) == IDOK) {
						Setting.context_customize() = true;
						change_ctxmenu(hWnd, true);
					}
					else if (!prev) {
						Setting.context_submenu().clear();
						Setting.context_customize() = false;
					}
					return FALSE;
				}
				
				// 「リセット」ボタン
				if (parameter == IDC_RESET_MENU_BUTTON) {
					Setting.context_submenu().clear();
					Setting.context_customize() = false;
					change_ctxmenu(hWnd, false);
					return FALSE;
				}
				
				// 「ショートカット」グループ
				const flag_map& shortcut = shortcut_map();
				pos = shortcut.find(parameter);
				if (pos != shortcut.end()) {
					change_flag(Setting.shortcut_flags(), hWnd, pos->first, pos->second);
					return FALSE;
				}
				
				if (parameter == IDC_SC_TYPE_COMBOBOX) {
					Setting.shortcut_compress_index() = SendMessage(GetDlgItem(hWnd, IDC_SC_TYPE_COMBOBOX), CB_GETCURSEL, 0, 0);
					return FALSE;
				}
				
				break;
			}
			default:
				break;
			}
			return common_dialogproc(hWnd, msg, wp, lp);
		}
		
		/* ---------------------------------------------------------------- */
		/*
		 *  archive_dialogproc
		 *
		 *  compress_dialogproc(), decompress_dialogproc() 共通の処理．
		 */
		/* ---------------------------------------------------------------- */
		static BOOL archive_dialogproc(user_setting::archive_type& setting, HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			std::size_t parameter = LOWORD(wp);
			
			// 「出力先フォルダ」グループ
			const flag_map& output = output_map();
			flag_map::const_iterator pos = output.find(parameter);
			if (pos != output.end()) {
				setting.output_condition() = pos->second;
				BOOL enabled = (setting.output_condition() == OUTPUT_SPECIFIC) ? TRUE : FALSE;
				EnableWindow(GetDlgItem(hWnd, IDC_DEST_TEXTBOX), enabled);
				EnableWindow(GetDlgItem(hWnd, IDC_DEST_BUTTON), enabled);
				return TRUE;
			}
			
			// 「詳細」グループ
			const flag_map& detail = detail_map();
			pos = detail.find(parameter);
			if (pos != detail.end()) {
				change_flag(setting.details(), hWnd, pos->first, pos->second);
				if (pos->first == IDC_OVERWRITE_CHECKBOX) {
					BOOL enabled = (setting.details() & DETAIL_OVERWRITE) ? TRUE : FALSE;
					EnableWindow(GetDlgItem(hWnd, IDC_NEWER_CHECKBOX), enabled);
				}
				
				if (pos->first == IDC_POSTOPEN_CHECKBOX) {
					BOOL enabled = (setting.details() & DETAIL_OPEN) ? TRUE : FALSE;
					EnableWindow(GetDlgItem(hWnd, IDC_SKIP_DESKTOP_CHECKBOX), enabled);
				}
				
				if (pos->first == IDC_CREATE_FOLDER_CHECKBOX) {
					BOOL enabled = (setting.details() & DETAIL_CREATE_FOLDER) ? TRUE : FALSE;
					EnableWindow(GetDlgItem(hWnd, IDC_SINGLE_FOLDER_CHECKBOX), enabled);
				}
				
				if (pos->first == IDC_TOOLTIP_CHECKBOX) {
					BOOL enabled = (setting.details() & DETAIL_TOOLTIP) ? TRUE : FALSE;
					EnableWindow(GetDlgItem(hWnd, IDC_MAX_FILELIST_TEXTBOX), enabled);
					EnableWindow(GetDlgItem(hWnd, IDC_FILELIST_UPDOWN), enabled);
				}
				
				return TRUE;
			}
			
			if (parameter == IDC_DEST_BUTTON) {
				std::basic_string<TCHAR> dest = browsefolder(_T("保存先ディレクトリを選択して下さい。"));
				if (!dest.empty()) {
					setting.output_path() = dest;
					SetWindowText(GetDlgItem(hWnd, IDC_DEST_TEXTBOX), dest.c_str());
				}
			}
			return common_dialogproc(hWnd, msg, wp, lp);
		}
		
		/* ----------------------------------------------------------------- */
		//  compress_dialogproc
		/* ----------------------------------------------------------------- */
		static BOOL CALLBACK compress_dialogproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			switch (msg) {
			case WM_INITDIALOG:
				archive_initdialog(hWnd, Setting.compression());
				break;
			case WM_COMMAND:
				return archive_dialogproc(Setting.compression(), hWnd, msg, wp, lp);
			default:
				break;
			}
			return common_dialogproc(hWnd, msg, wp, lp);
		}
		
		/* ----------------------------------------------------------------- */
		//  decompress_dialogproc
		/* ----------------------------------------------------------------- */
		static BOOL CALLBACK decompress_dialogproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			switch (msg) {
			case WM_INITDIALOG:
			{
				// ツールチップに表示する最大件数の設定
				HWND spin = GetDlgItem(hWnd, IDC_FILELIST_UPDOWN);
				if (spin) {
					std::size_t value = Setting.decompression().max_filelist();
					SendMessage(spin, UDM_SETRANGE, 0, MAKELONG(20, 1));
					SendMessage(spin, UDM_SETPOS, 0, MAKELONG(value, 0));
					if ((Setting.decompression().details() & DETAIL_TOOLTIP) == 0) {
						EnableWindow(GetDlgItem(hWnd, IDC_MAX_FILELIST_TEXTBOX), FALSE);
						EnableWindow(spin, FALSE);
					}
				}
				archive_initdialog(hWnd, Setting.decompression());
				break;
			}
			case WM_COMMAND:
			{
				// ツールチップに表示する最大件数の変更
				if (lp == (LPARAM)GetDlgItem(hWnd, IDC_MAX_FILELIST_TEXTBOX) && HIWORD(wp) == EN_CHANGE) {
					std::size_t pos = (std::size_t)SendMessage(GetDlgItem(hWnd, IDC_FILELIST_UPDOWN), UDM_GETPOS, 0, 0);
					if (pos > 0 && pos <= 20) Setting.decompression().max_filelist() = pos;
					return TRUE;
				}
				return archive_dialogproc(Setting.decompression(), hWnd, msg, wp, lp);
			}
			default:
				break;
			}
			return common_dialogproc(hWnd, msg, wp, lp);
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  filter_validate
		 *
		 *  入力された文字列がファイル名に使用できる文字列かどうか
		 *  チェックする．
		 */
		/* ----------------------------------------------------------------- */
		static BOOL filter_validate(HWND parent, HWND handle) {
			int n = SendMessage(handle, WM_GETTEXTLENGTH, 0, 0);
			std::vector<TCHAR> buffer(n + 1, 0);
			GetWindowText(handle, reinterpret_cast<TCHAR*>(&buffer.at(0)), buffer.size());
			if (buffer.at(buffer.size() - 1) == _T('\0')) buffer.pop_back();
			
			static const std::basic_string<TCHAR> invalids(_T("/:?\"<>|"));
			std::basic_string<TCHAR> str(buffer.begin(), buffer.end());
			for (std::basic_string<TCHAR>::const_iterator it = invalids.begin(); it != invalids.end(); ++it) {
				std::basic_string<TCHAR>::size_type pos = str.find(*it);
				if (pos != std::basic_string<TCHAR>::npos) {
					EDITBALLOONTIP balloon = {};
					balloon.cbStruct = sizeof(EDITBALLOONTIP);
					balloon.pszTitle = _T("ファイル名には次の文字は使えません。");
					balloon.pszText  = _T("/ : ? \" < > |");
					SendMessage(handle, EM_SHOWBALLOONTIP, 0, (LPARAM)&balloon);
					
					// 入力を受け付けない文字を除去したテキストを再設定する．
					str.erase(pos, 1);
					DWORD first = 0, last = 0;
					SendMessage(handle, EM_GETSEL, (WPARAM)&first, (WPARAM)&last);
					SetWindowText(handle, str.c_str());
					SendMessage(handle, EM_SETSEL, (first - 1 > 0) ? (first - 1) : 0, (last - 1) > 0 ? (last - 1) : 0);
				}
			}
			return FALSE;
		}

		/* ----------------------------------------------------------------- */
		//  filter_dialogproc
		/* ----------------------------------------------------------------- */
		static BOOL CALLBACK filter_dialogproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			switch (msg) {
			case WM_INITDIALOG:
				filter_initdialog(hWnd);
				break;
			case WM_COMMAND:
				if (LOWORD(wp) == IDC_FILTER_TEXTBOX && HIWORD(wp) == EN_UPDATE) {
					return filter_validate(hWnd, GetDlgItem(hWnd, IDC_FILTER_TEXTBOX));
				}
				break;
			default:
				break;
			}
			return common_dialogproc(hWnd, msg, wp, lp);
		}
		
		/* ----------------------------------------------------------------- */
		//  version_dialogproc
		/* ----------------------------------------------------------------- */
		static BOOL CALLBACK version_dialogproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			switch (msg) {
			case WM_INITDIALOG:
			{
				// アイコン
				HANDLE logo = LoadImage(GetModuleHandle(NULL), _T("IDI_APP"), IMAGE_ICON, 48, 48, LR_DEFAULTCOLOR | LR_SHARED | LR_VGACOLOR);
				HWND pic = GetDlgItem(hWnd, IDC_LOGO_PICTUREBOX);
				SendMessage(pic, STM_SETIMAGE, IMAGE_ICON, LPARAM(logo));
				
				// バージョン
				std::basic_string<TCHAR> message = _T("Version: ");
				message += Setting.version();
				if (sizeof(INT_PTR) == 4) message += _T(" (x86)");
				else message += _T(" (x64)");
				SetWindowText(GetDlgItem(hWnd, IDC_VERSION_LABEL), message.c_str());
				
				// アップデートの確認
				if (Setting.update()) CheckDlgButton(hWnd, IDC_UPDATE_CHECKBOX, BM_SETCHECK);
				
				break;
			}
			case WM_COMMAND:
				switch (LOWORD(wp)) {
				case IDC_UPDATE_CHECKBOX:
					Setting.update() = (IsDlgButtonChecked(hWnd, IDC_UPDATE_CHECKBOX) == BST_CHECKED);
					return TRUE;
				}
				break;
			default:
				break;
			}
			return common_dialogproc(hWnd, msg, wp, lp);
		}
	}
	
	/* --------------------------------------------------------------------- */
	//  create_propsheet
	/* --------------------------------------------------------------------- */
	INT_PTR CALLBACK psh_proc(HWND handle,UINT msg, LPARAM param) {
		if (msg == PSCB_INITIALIZED) {
			EnableWindow(GetDlgItem(handle, IDCANCEL), FALSE);
			HMENU menu = GetSystemMenu(handle, FALSE);
			RemoveMenu(menu, SC_CLOSE, MF_BYCOMMAND);
		}
		return FALSE;
	}
	
	/* --------------------------------------------------------------------- */
	//  create_propsheet
	/* --------------------------------------------------------------------- */
	INT_PTR create_propsheet(HWND parent, bool install) {
		static const std::size_t pagenum = 5;
		HPROPSHEETPAGE ref[pagenum] = {};
		
		PROPSHEETPAGE page = {};
		page.dwSize = sizeof (PROPSHEETPAGE);
		page.dwFlags = PSP_DEFAULT | PSP_USEICONID;
		page.hInstance = GetModuleHandle(NULL);
		
		// 「一般」ページ
		page.pszTemplate = _T("IDD_GENERAL");
		page.pfnDlgProc = (DLGPROC)detail::general_dialogproc;
		ref[0] = CreatePropertySheetPage(&page);
		
		// 「圧縮」ページ
		page.pszTemplate = _T("IDD_COMPRESS");
		page.pfnDlgProc = (DLGPROC)detail::compress_dialogproc;
		ref[1] = CreatePropertySheetPage(&page);
		
		// 「解凍」ページ
		page.pszTemplate = _T("IDD_DECOMPRESS");
		page.pfnDlgProc = (DLGPROC)detail::decompress_dialogproc;
		ref[2] = CreatePropertySheetPage(&page);
		
		// 「フィルタリング」ページ
		page.pszTemplate = _T("IDD_FILTER");
		page.pfnDlgProc = (DLGPROC)detail::filter_dialogproc;
		ref[3] = CreatePropertySheetPage(&page);

		// 「バージョン情報」ページ
		page.pszTemplate = _T("IDD_VERSION");
		page.pfnDlgProc = (DLGPROC)detail::version_dialogproc;;
		ref[4] = CreatePropertySheetPage(&page);
		
		PROPSHEETHEADER psh = {};
		psh.dwSize = sizeof (PROPSHEETHEADER);
		psh.dwFlags = PSH_DEFAULT | PSH_USEHICON  | PSH_NOAPPLYNOW;
		psh.hwndParent = parent;
		psh.hIcon = LoadIcon(page.hInstance, _T("IDI_APP"));
		psh.pszCaption = _T("CubeICE 設定");
		psh.nPages = pagenum;
		psh.phpage = ref;
		
		if (install) {
			psh.dwFlags |= PSH_USECALLBACK;
			psh.pfnCallback = (PFNPROPSHEETCALLBACK)psh_proc;
		}
		
		return PropertySheet(&psh);
	}
}