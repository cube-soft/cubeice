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

#include <set>
#include <string>
#include <tchar.h>
#include "resource.h"

#ifndef NO_WIN32_LEAN_AND_MEAN
#define NO_WIN32_LEAN_AND_MEAN
#include <shlobj.h>
#endif // NO_WIN32_LEAN_AND_MEAN

#define CUBE_MAX_PATH 2048

namespace cubeice {
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

		extern std::basic_string<TCHAR> openfile(const TCHAR* filter, const TCHAR* init, const TCHAR* title = _T("ファイルを開く"));
		extern std::basic_string<TCHAR> savefile(const TCHAR* filter, const TCHAR* init, const TCHAR* title = _T("名前を付けて保存"));
		extern std::basic_string<TCHAR> browsefolder(const TCHAR* description = _T("フォルダの参照"));
		
		/* ----------------------------------------------------------------- */
		//  progressbar
		/* ----------------------------------------------------------------- */
		class progressbar {
		public:
			typedef TCHAR char_type;
			typedef std::basic_string<TCHAR> string_type;
			typedef std::size_t size_type;
			
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			progressbar() :
				handle_(NULL), pos_(0.0), min_(0), max_(1000), cancel_(false) {
				this->initialize();
			}

			/* ------------------------------------------------------------- */
			/*
			 *  operator+=
			 *
			 *  プログレスバーを n 進める．．
			 */
			/* ------------------------------------------------------------- */
			progressbar& operator+=(double n) {
				this->position(pos_ + n);
				return *this;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  operator-=
			 *
			 *  プログレスバーを n 戻す．
			 */
			/* ------------------------------------------------------------- */
			progressbar& operator-=(double n) {
				this->position(pos_ - n);
				return *this;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  operator++
			 *
			 *  プログレスバーを 1 進める．
			 */
			/* ------------------------------------------------------------- */
			progressbar& operator++() {
				this->position(pos_ + 1);
				return *this;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  operator++(int)
			 *
			 *  プログレスバーを 1 進める．
			 */
			/* ------------------------------------------------------------- */
			progressbar& operator++(int) {
				this->position(pos_ + 1);
				return *this;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  operator--
			 *
			 *  プログレスバーを 1 戻す．
			 */
			/* ------------------------------------------------------------- */
			progressbar& operator--() {
				this->position(pos_ - 1);
				return *this;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  operator--(int)
			 *
			 *  プログレスバーを 1 戻す．
			 */
			/* ------------------------------------------------------------- */
			progressbar& operator--(int) {
				this->position(pos_ - 1);
				return *this;
			}
			
			/* ------------------------------------------------------------- */
			//  position
			/* ------------------------------------------------------------- */
			double position() const { return pos_; }
			
			/* ------------------------------------------------------------- */
			//  position
			/* ------------------------------------------------------------- */
			void position(double pos) {
				if (pos < min_) pos_ = min_;
				else if (pos > max_) pos_ = max_;
				else pos_ = pos;
				SendMessage(GetDlgItem(handle_, IDC_PROGRESS), PBM_SETPOS, static_cast<int>(pos_), 0);
			}
			
			/* ------------------------------------------------------------- */
			//  minimum
			/* ------------------------------------------------------------- */
			size_type minimum() const { return min_; }
			
			/* ------------------------------------------------------------- */
			//  minimum
			/* ------------------------------------------------------------- */
			void minimum(size_type n) {
				min_ = n;
				if (min_ < max_) {
					HWND handle = GetDlgItem(handle_, IDC_PROGRESS);
					SendMessage(handle, PBM_SETRANGE, (WPARAM)0, MAKELPARAM(min_, max_));
				}
			}

			/* ------------------------------------------------------------- */
			//  maximum
			/* ------------------------------------------------------------- */
			size_type maximum() const { return max_; }

			/* ------------------------------------------------------------- */
			//  maximum
			/* ------------------------------------------------------------- */
			void maximum(size_type n) {
				max_ = n;
				if (min_ < max_) {
					HWND handle = GetDlgItem(handle_, IDC_PROGRESS);
					SendMessage(handle, PBM_SETRANGE, (WPARAM)0, MAKELPARAM(min_, max_));
				}
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
			//  is_cancel
			/* ------------------------------------------------------------- */
			bool is_cancel() const { return cancel_; }
			
			/* ------------------------------------------------------------- */
			/*
			 *  marquee
			 *
			 *  speed はミリ秒単位で指定する．値が大きくなるほど遅くなる．
			 */
			/* ------------------------------------------------------------- */
			void marquee(bool enable, int speed = 50) {
				HWND handle = GetDlgItem(handle_, IDC_PROGRESS);
				
				LONG_PTR style = ::GetWindowLongPtr(handle, GWL_STYLE);
				if (enable) style |= PBM_SETMARQUEE;
				else style &= ~PBM_SETMARQUEE;
				::SetWindowLongPtr(handle, GWL_STYLE, style | PBS_MARQUEE);
				
				BOOL wp = enable ? TRUE : FALSE;
				SendMessage(handle, PBM_SETMARQUEE, (WPARAM)wp, (LPARAM)speed);
			}

			/* ------------------------------------------------------------- */
			//  refresh
			/* ------------------------------------------------------------- */
			void refresh() {
				MSG msg = {};
				if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
					if (!GetMessage(&msg, NULL, 0, 0)) return;
					if (!IsDialogMessage(handle_, &msg)) {
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					
					if (progressbar::closed_handles().find(handle_) != progressbar::closed_handles().end()) {
						progressbar::closed_handles().erase(handle_);
						cancel_ = true;
					}
				}
			}
			
			/* ------------------------------------------------------------- */
			//  handle
			/* ------------------------------------------------------------- */
			const HWND handle() const { return handle_; }
			
		private:
			HWND handle_;
			double pos_;
			size_type min_;
			size_type max_;
			bool cancel_;
			
			/* ------------------------------------------------------------- */
			//  initialize
			/* ------------------------------------------------------------- */
			void initialize() {
				handle_ = CreateDialog(GetModuleHandle(NULL), _T("IDD_PROGRESS"), NULL, wndproc);
			}
			
			/* ------------------------------------------------------------- */
			//  closed_handles
			/* ------------------------------------------------------------- */
			static std::set<HANDLE>& closed_handles() {
				static std::set<HANDLE> handles;
				return handles;
			}
			
			/* ------------------------------------------------------------- */
			//  wndproc
			/* ------------------------------------------------------------- */
			static BOOL CALLBACK wndproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
				switch (msg) {
				case WM_INITDIALOG:
				{
					// アイコンの設定
					HICON icon = LoadIcon(GetModuleHandle(NULL), _T("IDI_APP"));
					SendMessage(hWnd, WM_SETICON, 0, LPARAM(icon));
					
					// 画面中央に表示
					RECT rect = {};
					GetWindowRect(hWnd, (LPRECT)&rect);
					int x = (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2;
					int y = (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2;
					SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
					
					// プログレスバーの min, max, step の初期値を登録
					HWND handle = GetDlgItem(hWnd, IDC_PROGRESS);
					SendMessage(handle, PBM_SETRANGE, (WPARAM)0, MAKELPARAM(0, 1000));
					SendMessage(handle, PBM_SETSTEP, 1, 0);
					
					return TRUE;
				}
				case WM_COMMAND:
					switch (LOWORD(wp)) {
					case IDCANCEL:
						DestroyWindow(hWnd);
						progressbar::closed_handles().insert(hWnd);
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

#include "password.h"

#endif // CUBE_DIALOG_H