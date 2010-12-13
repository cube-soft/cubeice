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
#ifndef CUBE_DIALOG_H
#define CUBE_DIALOG_H

#include <string>
#include <tchar.h>
#include "resource.h"

#ifndef NO_WIN32_LEAN_AND_MEAN
#define NO_WIN32_LEAN_AND_MEAN
#include <shlobj.h>
#endif // NO_WIN32_LEAN_AND_MEAN

#define CUBE_MAX_PATH 2048

namespace cube {
	namespace dialog {
		extern std::basic_string<TCHAR> openfile(const TCHAR* filter, const TCHAR* title = _T("ファイルを開く"));
		extern std::basic_string<TCHAR> savefile(const TCHAR* filter, const TCHAR* title = _T("名前を付けて保存"));
		extern std::basic_string<TCHAR> browsefolder(const TCHAR* description = _T("フォルダの参照"));
		
		/* ----------------------------------------------------------------- */
		//  progressbar
		/* ----------------------------------------------------------------- */
		class progressbar {
		public:
			typedef TCHAR char_type;
			typedef std::basic_string<TCHAR> string_type;
			
			/* ------------------------------------------------------------- */
			//  static functions
			/* ------------------------------------------------------------- */
			static int minimum() { return 0; }
			static int maximum() { return 1000; }
			static int step() { return 1; }
			
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			progressbar() :
				handle_(NULL), pos_(progressbar::minimum()) {
				this->initialize();  
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  operator+=
			 *
			 *  プログレスバーを n ステップ進める．1 ステップの値は
			 *  progressbar::step() から取得できる．
			 */
			/* ------------------------------------------------------------- */
			progressbar& operator+=(int n) {
				this->position(pos_ + n * progressbar::step());
				SendMessage(GetDlgItem(handle_, IDC_PROGRESS), PBM_SETPOS, pos_, 0);
				return *this;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  operator-=
			 *
			 *  プログレスバーを n ステップ戻す．1 ステップの値は
			 *  progressbar::step() から取得できる．
			 */
			/* ------------------------------------------------------------- */
			progressbar& operator-=(int n) {
				this->position(pos_ - n * progressbar::step());
				SendMessage(GetDlgItem(handle_, IDC_PROGRESS), PBM_SETPOS, pos_, 0);
				return *this;
			}

			/* ------------------------------------------------------------- */
			/*
			 *  operator++(int)
			 *
			 *  プログレスバーを 1 ステップ進める．1 ステップの値は
			 *  progressbar::step() から取得できる．
			 */
			/* ------------------------------------------------------------- */
			progressbar& operator++(int) {
				this->position(pos_ + progressbar::step());
				SendMessage(GetDlgItem(handle_, IDC_PROGRESS), PBM_SETPOS, pos_, 0);
				return *this;
			}

			/* ------------------------------------------------------------- */
			/*
			 *  operator++
			 *
			 *  プログレスバーを 1 ステップ進める．1 ステップの値は
			 *  progressbar::step() から取得できる．
			 */
			/* ------------------------------------------------------------- */
			progressbar& operator++() {
				this->position(pos_ + progressbar::step());
				SendMessage(GetDlgItem(handle_, IDC_PROGRESS), PBM_SETPOS, pos_, 0);
				return *this;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  operator--
			 *
			 *  プログレスバーを 1 ステップ戻す．1 ステップの値は
			 *  progressbar::step() から取得できる．
			 */
			/* ------------------------------------------------------------- */
			progressbar& operator--() {
				this->position(pos_ - progressbar::step());
				SendMessage(GetDlgItem(handle_, IDC_PROGRESS), PBM_SETPOS, pos_, 0);
				return *this;
			}

			/* ------------------------------------------------------------- */
			/*
			 *  operator--(int)
			 *
			 *  プログレスバーを 1 ステップ戻す．1 ステップの値は
			 *  progressbar::step() から取得できる．
			 */
			/* ------------------------------------------------------------- */
			progressbar& operator--(int) {
				this->position(pos_ - progressbar::step());
				SendMessage(GetDlgItem(handle_, IDC_PROGRESS), PBM_SETPOS, pos_, 0);
				return *this;
			}

			/* ------------------------------------------------------------- */
			//  position
			/* ------------------------------------------------------------- */
			int position() const { return pos_; }
			
			/* ------------------------------------------------------------- */
			//  position
			/* ------------------------------------------------------------- */
			void position(int pos) {
				if (pos < progressbar::minimum()) pos_ = progressbar::minimum();
				else if (pos > progressbar::maximum()) pos_ = progressbar::maximum();
				else pos_ = pos;
			}

			/* ------------------------------------------------------------- */
			//  text
			/* ------------------------------------------------------------- */
			string_type text() const {
				TCHAR buffer[2048];
				GetWindowText(GetDlgItem(handle_, IDC_INFO_LABEL), buffer, 2048);
				return string_type(buffer);
			}
			
			/* ------------------------------------------------------------- */
			//  text
			/* ------------------------------------------------------------- */
			void text(const string_type& s) {
				SetWindowText(GetDlgItem(handle_, IDC_INFO_LABEL), s.c_str());
			}

			/* ------------------------------------------------------------- */
			//  handle
			/* ------------------------------------------------------------- */
			const HWND handle() const { return handle_; }
			
		private:
			HWND handle_;
			int pos_;
			
			/* ------------------------------------------------------------- */
			//  initialize
			/* ------------------------------------------------------------- */
			void initialize() {
				handle_ = CreateDialog(GetModuleHandle(NULL), _T("IDD_PROGRESS"), NULL, wndproc);
			}
			
			/* ------------------------------------------------------------- */
			//  wndproc
			/* ------------------------------------------------------------- */
			static BOOL CALLBACK wndproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
				switch (msg) {
				case WM_INITDIALOG:
				{
					// 画面中央に表示
					RECT rect = {};
					GetWindowRect(hWnd, (LPRECT)&rect);
					int x = (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2;
					int y = (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2;
					SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
					
					// プログレスバーの min, max, step 値を登録
					HWND handle = GetDlgItem(hWnd, IDC_PROGRESS);
					SendMessage(handle, PBM_SETRANGE, (WPARAM)0, MAKELPARAM(progressbar::minimum(), progressbar::maximum()));
					SendMessage(handle, PBM_SETSTEP, (WPARAM)progressbar::step(), 0);

					return TRUE;
				}
				case WM_COMMAND:
					switch (LOWORD(wp)) {
					case IDCANCEL:
						EndDialog(hWnd, 0);
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

		private:
			/* ------------------------------------------------------------- */
			//  non-copyable
			/* ------------------------------------------------------------- */
			progressbar(const progressbar& cp);
			progressbar& operator=(const progressbar& cp);
		};
	}
}

#endif // CUBE_DIALOG_H