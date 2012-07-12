// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-setting/filtering-page.h
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
#ifndef CUBEICE_SETTING_FILTERING_PAGE_H
#define CUBEICE_SETTING_FILTERING_PAGE_H

#include "cubeice-setting.h"
#include <psdotnet/forms/utility.h>
#include "setting-page.h"

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// FilteringPage
	///
	/// <summary>
	/// SettingDialog に表示させる「フィルタリング」ページです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class FilteringPage : public SettingPage {
	private:
		typedef SettingPage super;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		FilteringPage(UserSetting& data, int_ptr changed) :
			super(_T("IDD_FILTERING"), data, changed), hfilter_(NULL) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~FilteringPage() {}
		
	protected:
		/* ----------------------------------------------------------------- */
		///
		/// OnCreateControl
		///
		/// <summary>
		/// 画面生成直後の初期化処理を行います。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void OnCreateControl() {
			LOG_DEBUG(_T("FilteringPage::OnCreateControl"));
			UserFilteringSetting& data = this->Data().Filtering();
			
			hfilter_ = ::GetDlgItem(this->Handle(), IDC_FILTERING);
			if (data.Empty()) return;
			string_type str = boost::algorithm::join(data.Data(), _T("\r\n"));
			SetWindowText(hfilter_, str.c_str());
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// OnKillFocus
		///
		/// <summary>
		/// フィルタリングの文字列を更新します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void OnKillFocus() {
			LOG_DEBUG(_T("FilteringPage::OnKillFocus"));
			UserFilteringSetting& data = this->Data().Filtering();
			
			LOG_TRACE(_T("PreModify : %s"), boost::algorithm::join(data.Data(), _T("|")).c_str());
			data.Clear();
			
			string_type str = PsdotNet::Forms::Utility::GetText(hfilter_);
			if (str.empty()) return;
			
			std::vector<string_type> v;
			boost::algorithm::split(v, str, boost::is_any_of(_T("\r\n")));
			BOOST_FOREACH(const string_type& s, v) {
				if (!s.empty()) data.Add(s);
			}
			LOG_TRACE(_T("PostModify: %s"), boost::algorithm::join(data.Data(), _T("|")).c_str());
		}
		
		/* ----------------------------------------------------------------- */
		/// OnUpdate
		/* ----------------------------------------------------------------- */
		virtual void OnUpdate(PsdotNet::Forms::Message& m) {
			m.Result(TRUE);
			
			string_type str = PsdotNet::Forms::Utility::GetText(hfilter_);
			const string_type invalids = _T("/:?\"<>|");
			BOOST_FOREACH(const char_type& c, invalids) {
				string_type::size_type pos = str.find(c);
				while (pos != string_type::npos) {
					LOG_TRACE(_T("Invalid char detected (char = %c)"), c);
					EDITBALLOONTIP balloon = {};
					balloon.cbStruct = sizeof (EDITBALLOONTIP);
					balloon.pszTitle = _T("ファイル名には次の文字は使えません。");
					balloon.pszText  = _T("/ : ? \" < > |");
					SendMessage(hfilter_, EM_SHOWBALLOONTIP, 0, (LPARAM)&balloon);
					m.Result(FALSE);
					
					// 入力を受け付けない文字を除去したテキストを再設定する．
					str.erase(pos, 1);
					DWORD first = 0, last = 0;
					::SendMessage(hfilter_, EM_GETSEL, (WPARAM)&first, (WPARAM)&last);
					::SetWindowText(hfilter_, str.c_str());
					SendMessage(hfilter_, EM_SETSEL, (first - 1 > 0) ? (first - 1) : 0, (last - 1) > 0 ? (last - 1) : 0);
					pos = str.find(c, pos);
				}
			}
		}
		
		/* ----------------------------------------------------------------- */
		/// OnCommand
		/* ----------------------------------------------------------------- */
		virtual void OnCommand(PsdotNet::Forms::Message& m) {
			switch (m.WParam()) {
			case IDC_FILTERING | (EN_KILLFOCUS << 16):
				this->OnKillFocus();
				m.Result(FALSE);
				break;
			case IDC_FILTERING | (EN_UPDATE << 16):
				this->OnUpdate(m);
				break;
			default:
				super::OnCommand(m);
				break;
			}
		}
		
	private:
		handle_type hfilter_;
	}; // class CompressionPage
} // namespace CubeICE

#endif // CUBEICE_SETTING_FILTERING_PAGE_H
