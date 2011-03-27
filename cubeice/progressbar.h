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

#include <shobjidl.h>
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
				handle_(NULL), taskbar_(NULL), style_(normal), pos_(0.0), prev_(0.0), sub_(0.0), min_(0), max_(10000),
				cancel_(false), suspend_(false), totalsuspend_(0.0), partsuspend_(0.0), start_(false),
				denominator_(0), denominator_timer_(), denominator_delta_(0.1),
				numerator_(0), remain_(0), remain_fake_(std::make_pair(0, 0)) {
				this->init();
			}
			
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			explicit progressbar(int style) :
				handle_(NULL), taskbar_(NULL), style_(style), pos_(0.0), prev_(0.0), sub_(0.0), min_(0), max_(10000),
				cancel_(false), suspend_(false), totalsuspend_(0.0), partsuspend_(0.0), start_(false),
				denominator_(0), denominator_timer_(), denominator_delta_(0.1),
				numerator_(0), remain_(0), remain_fake_(std::make_pair(0, 0)) {
				this->init();
			}
			
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			~progressbar() {
				CoUninitialize();
			}
			
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			void style(int style) {
				style_ = style;
			}
			
			/* ------------------------------------------------------------- */
			//  show
			/* ------------------------------------------------------------- */
			void show() {
				string_type name = (style_ == simple) ? _T("IDD_PROGRESS_SIMPLE") : _T("IDD_PROGRESS");
				handle_ = CreateDialogParam(GetModuleHandle(NULL), name.c_str(), NULL, wndproc, (LPARAM)this);
				if (handle_) {
					//LONG_PTR style = GetWindowLongPtr(handle_, GWL_STYLE);
					//style |= WS_EX_APPWINDOW;
					//SetWindowLongPtr(handle_, GWL_STYLE, style);
					if (taskbar_) taskbar_->SetProgressState(handle_, TBPF_NORMAL);
				}
				timer_.restart();
				denominator_timer_.restart();
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
				if (taskbar_) taskbar_->SetProgressValue(handle_, static_cast<ULONGLONG>(pos_), static_cast<ULONGLONG>(max_));
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
			//  suspend
			/* ------------------------------------------------------------- */
			bool suspend(bool s) {
				suspend_ = s;
				if( suspend_ ) {
					double elp = timer_.elapsed();
					totalsuspend_ += elp;
					partsuspend_ += elp;
				} else {
					timer_.update();
				}
				return suspend_;
			}
			
			/* ------------------------------------------------------------- */
			//  is_suspend
			/* ------------------------------------------------------------- */
			bool is_suspend() const { return suspend_; }
			
			/* ------------------------------------------------------------- */
			//  is_marquee
			/* ------------------------------------------------------------- */
			bool is_marquee() const {
				HWND handle = GetDlgItem(handle_, IDC_PROGRESS);
				LONG_PTR style = ::GetWindowLongPtr(handle, GWL_STYLE);
				return (style & PBS_MARQUEE) != 0;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  marquee
			 *
			 *  speed はミリ秒単位で指定する．値が大きくなるほど遅くなる．
			 */
			/* ------------------------------------------------------------- */
			void marquee(bool enable, int speed = 50) {
				HWND main = GetDlgItem(handle_, IDC_PROGRESS);
				HWND sub  = GetDlgItem(handle_, IDC_PROGRESS_SUB);
				
				LONG_PTR main_style = ::GetWindowLongPtr(main, GWL_STYLE);
				LONG_PTR sub_style  = ::GetWindowLongPtr(sub, GWL_STYLE);
				if (enable) {
					main_style |= PBS_MARQUEE;
					sub_style  |= PBS_MARQUEE;
				}
				else {
					main_style &= ~PBS_MARQUEE;
					sub_style  &= ~PBS_MARQUEE;
				}
				::SetWindowLongPtr(main, GWL_STYLE, main_style);
				::SetWindowLongPtr(sub, GWL_STYLE, sub_style);
				
				if (enable) {
					SendMessage(main, PBM_SETMARQUEE, (WPARAM)TRUE, (LPARAM)speed);
					SendMessage(sub, PBM_SETMARQUEE, (WPARAM)TRUE, (LPARAM)speed);
				}
				else {
					SendMessage(main, PBM_SETMARQUEE, (WPARAM)FALSE, (LPARAM)0);
					SendMessage(sub, PBM_SETMARQUEE, (WPARAM)FALSE, (LPARAM)0);
				}
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
				TCHAR		elapse_time[128] = {};
				TCHAR		remain_time[128] = {};
				
				if( !suspend_ ) {
					format_time( elapse_time, (int)( timer_.elapsed() + totalsuspend_ ) );
					SetDlgItemText( handle_, IDC_ELAPSE_LABEL, elapse_time );
					
					if( start_ && pos_ != min_ ) {
						if (pos_ - prev_ > 1) {
							remain_ = static_cast<int>((timer_.elapsed() + partsuspend_) * (max_ - pos_) / (pos_ - min_));
							remain_fake_.first = static_cast<int>(timer_.elapsed() + partsuspend_) - remain_fake_.first;
							remain_fake_.second = 0;
							
							// 1 の位を切り上げ
							remain_ += 10;
							remain_ /= 10;
							remain_ *= 10;
							
							prev_ = pos_;
						}
						else {
							// フェイクで減少させる時間は，直前の更新に要した経過時間を最大値とする．
							if (remain_ > 10 && remain_fake_.second < remain_fake_.first) {
								--remain_;
								++remain_fake_.second;
							}
						}
						format_time( remain_time, remain_ );
						string_type message(_T("約 "));
						message += remain_time;
						SetDlgItemText( handle_, IDC_REMAIN_LABEL, message.c_str() );
					} else {
						SetDlgItemText( handle_, IDC_REMAIN_LABEL, _T("--:--:--") );
					}
				}
			}
			
			/* ------------------------------------------------------------- */
			//  start
			/* ------------------------------------------------------------- */
			void start() {
				start_ = true;
				partsuspend_ = 0;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  denomcount
			 *
			 *  ファイル総数をカウントアップさせる．denomcount は，
			 *  実際の表示部分はタイマーを用いて一定時間ごとに更新している．
			 *  そのため，場合によっては更新されない場合がある．
			 *  その場合は，引数に 0 を指定する事によって強制的に現在の値
			 *  を表示させる．
			 */
			/* ------------------------------------------------------------- */
			void denomcount(int diff = 1) {
				denominator_ += diff;
				
				if (diff == 0 || denominator_timer_.elapsed() > denominator_delta_) {
					std::basic_string<TCHAR> str = clx::lexical_cast<std::basic_string<TCHAR> >(denominator_);
					SetDlgItemText( handle_, IDC_FN_DENOM_LABEL, str.c_str() );
					denominator_timer_.update();
					if (diff == 0) denominator_delta_ = 0.1;
					else if (denominator_delta_ < 1.0) denominator_delta_ += 0.1;
				}
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
			ITaskbarList3* taskbar_;
			int style_;
			double pos_;
			double prev_;
			double sub_;
			size_type min_;
			size_type max_;
			bool cancel_;
			bool suspend_;
			clx::timer timer_;
			double totalsuspend_;
			double partsuspend_;
			bool start_;
			int denominator_;
			clx::timer denominator_timer_;
			double denominator_delta_;
			int numerator_;
			int remain_;
			std::pair<int, int> remain_fake_; // 残り時間の表示を滑らかにするために使用する情報
			
			/* ------------------------------------------------------------- */
			//  init
			/* ------------------------------------------------------------- */
			void init() {
				CoInitialize(NULL);
				HRESULT result = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&taskbar_));
				if (!SUCCEEDED(result)) taskbar_ = NULL;
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
					case IDOK:
					{
						static TCHAR		title[1024] = _T("");

						prog->suspend( !prog->is_suspend() );
						SetDlgItemText(hWnd, IDOK, prog->is_suspend() ? _T("再開") : _T("中断"));
						if( prog->is_suspend() ) {
							TCHAR		tmp[1152];
							GetWindowText( hWnd, title, sizeof( title ) / sizeof( title[0] ) );
							_stprintf( tmp, _T("(中断) %s"), title );
							SetWindowText( hWnd, tmp );
						} else {
							SetWindowText( hWnd, title );
						}
						break;
					}
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
