/* ------------------------------------------------------------------------- */
/*
 *  progressbar.h
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
#ifndef CUBEICE_PROGRESSBAR_H
#define CUBEICE_PROGRESSBAR_H

#include <clx/time.h>

namespace cubeice {
	namespace dialog {
		/* ----------------------------------------------------------------- */
		//  progressbar
		/* ----------------------------------------------------------------- */
		class progressbar {
		public:
			typedef TCHAR char_type;
			typedef std::basic_string<TCHAR> string_type;
			typedef std::size_t size_type;
			
			enum {
				normal = 0x01,
				simple = 0x02
			};
			
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			progressbar() :
				handle_(NULL), style_(normal), pos_(0.0), sub_(0.0), min_(0), max_(10000), cancel_(false), denominator_(0), numerator_(0) {}
			
			explicit progressbar(int style) :
				handle_(NULL), style_(style), pos_(0.0), sub_(0.0), min_(0), max_(10000), cancel_(false), denominator_(0), numerator_(0) {}
			
			/* ------------------------------------------------------------- */
			//  show
			/* ------------------------------------------------------------- */
			void show() {
				string_type name = (style_ == simple) ? _T("IDD_PROGRESS_SIMPLE") : _T("IDD_PROGRESS");
				handle_ = CreateDialogParam(GetModuleHandle(NULL), name.c_str(), NULL, wndproc, (LPARAM)this);
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
			//  subposition
			/* ------------------------------------------------------------- */
			double subposition() const { return sub_; }
			
			/* ------------------------------------------------------------- */
			//  subposition
			/* ------------------------------------------------------------- */
			void subposition(double pos) {
				if (pos < min_) sub_ = min_;
				else if (pos > max_) sub_ = max_;
				else sub_ = pos;
				SendMessage(GetDlgItem(handle_, IDC_PROGRESS_SUB), PBM_SETPOS, static_cast<int>(sub_), 0);
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
					handle = GetDlgItem(handle_, IDC_PROGRESS_SUB);
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
					handle = GetDlgItem(handle_, IDC_PROGRESS_SUB);
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
			//  title
			/* ------------------------------------------------------------- */
			void title(const string_type& s) {
				SendMessage(handle_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(s.c_str()));
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
				if (enable) style |= PBS_MARQUEE;
				else style &= ~PBS_MARQUEE;
				::SetWindowLongPtr(handle, GWL_STYLE, style);
				
				if (enable) SendMessage(handle, PBM_SETMARQUEE, (WPARAM)TRUE, (LPARAM)speed);
				else SendMessage(handle, PBM_SETMARQUEE, (WPARAM)FALSE, (LPARAM)0);
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
			//  timer_refresh
			/* ------------------------------------------------------------- */
			void timer_refresh() {
				TCHAR		elapse_time[128];
				TCHAR		remain_time[128];

				format_time( elapse_time, (int)timer_.total_elapsed() );
				SetDlgItemText( handle_, IDC_ELAPSE_LABEL, elapse_time );
				if( pos_ != min_ ) {
					format_time( remain_time, (int)( timer_.elapsed() * ( max_ - pos_ ) / ( pos_ - min_ ) ) );
					SetDlgItemText( handle_, IDC_REMAIN_LABEL, remain_time );
				} else {
					SetDlgItemText( handle_, IDC_REMAIN_LABEL, _T("--:--:--") );
				}
			}

			/* ------------------------------------------------------------- */
			//  start
			/* ------------------------------------------------------------- */
			void start() { timer_.update(); }


			/* ------------------------------------------------------------- */
			//  denomcount
			/* ------------------------------------------------------------- */
			void denomcount(int diff = 1) {
				std::basic_string<TCHAR>	str;

				denominator_ += diff;

				str = clx::lexical_cast<std::basic_string<TCHAR> >(denominator_);
				SetDlgItemText( handle_, IDC_FN_DENOM_LABEL, str.c_str() );
			}

			/* ------------------------------------------------------------- */
			//  numcount
			/* ------------------------------------------------------------- */
			void numcount(int diff = 1) {
				std::basic_string<TCHAR>	str;

				numerator_ += diff;

				str = clx::lexical_cast<std::basic_string<TCHAR> >(numerator_);
				SetDlgItemText( handle_, IDC_FN_NUM_LABEL, str.c_str() );
			}

			/* ------------------------------------------------------------- */
			//  handle
			/* ------------------------------------------------------------- */
			const HWND handle() const { return handle_; }
			
		private:
			HWND handle_;
			int style_;
			double pos_;
			double sub_;
			size_type min_;
			size_type max_;
			bool cancel_;
			clx::timer timer_;
			int denominator_;
			int numerator_;
			
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
			static INT_PTR CALLBACK wndproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
				static progressbar	*prog;
				const UINT_PTR		TIMER_ID = WM_APP;

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
					SendMessage(handle, PBM_SETRANGE, (WPARAM)0, MAKELPARAM(0, 10000));
					SendMessage(handle, PBM_SETSTEP, 1, 0);
					
					// サブバーの min, max, step の初期値を登録
					handle = GetDlgItem(hWnd, IDC_PROGRESS_SUB);
					SendMessage(handle, PBM_SETRANGE, (WPARAM)0, MAKELPARAM(0, 10000));
					SendMessage(handle, PBM_SETSTEP, 1, 0);
					
					// タイマの設定
					SetTimer(hWnd, TIMER_ID, 1000, NULL);
					prog = (progressbar*)lp;
					return TRUE;
				}
				case WM_COMMAND:
					switch (LOWORD(wp)) {
					case IDCANCEL:
						KillTimer(hWnd, TIMER_ID);
						DestroyWindow(hWnd);
						progressbar::closed_handles().insert(hWnd);
						break;
					default:
						break;
					}
					break;
				case WM_TIMER:
					if(wp != TIMER_ID)
						break;
					prog->timer_refresh();
					return TRUE;
				default:
					break;
				}
				
				return FALSE;
			}
			
			/* ------------------------------------------------------------- */
			//  format_time
			/* ------------------------------------------------------------- */
			void format_time( TCHAR *dest, int time )
			{
				_stprintf( dest, _T("%02d:%02d:%02d"), time / 3600, ( time % 3600 ) / 60, time % 60);
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

#endif // CUBEICE_PROGRESSBAR_H
