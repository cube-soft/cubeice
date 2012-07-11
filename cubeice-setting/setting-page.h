// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-setting/setting-page.h
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
#ifndef CUBEICE_SETTING_SETTING_PAGE_H
#define CUBEICE_SETTING_SETTING_PAGE_H

#include "cubeice-setting.h"
#include <boost/shared_ptr.hpp>
#include <psdotnet/forms/property-page.h>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// SettingPage
	///
	/// <summary>
	/// SettingDialog に表示させる各ページの基底クラスです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class SettingPage : public PsdotNet::Forms::PropertyPage {
	private:
		typedef PsdotNet::Forms::PropertyPage super;
		
	public:
		typedef boost::shared_ptr<bool> bool_ptr;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		SettingPage(const string_type& resource_name, UserSetting& data, bool_ptr changed) :
			super(resource_name), data_(data), changed_(changed) {
			this->InitializeComponent();
		}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~SettingPage() {}
		
		/* ----------------------------------------------------------------- */
		///
		/// Data
		///
		/// <summary>
		/// 各種ユーザ設定を保持しているデータを取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		UserSetting& Data() { return data_; }
		const UserSetting& Data() const { return data_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Changed
		///
		/// <summary>
		/// データに対して、何らかの修正が行われたかどうかを取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		bool Changed() const { return *changed_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Changed
		///
		/// <summary>
		/// データに対して、何らかの修正が行われたかどうかを設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Changed(bool x) { *changed_ = x; }
		
	protected:
		/* ----------------------------------------------------------------- */
		///
		/// InitializeComponent
		///
		/// <summary>
		/// ウィンドウ生成直後（ハンドルの取得済み）の初期処理を行います。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void OnCreateControl() {}
		
		/* ----------------------------------------------------------------- */
		///
		/// OnNotify
		///
		/// <summary>
		/// 現在のユーザ設定を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void OnNotify(PsdotNet::Forms::Message& m) {
			NMHDR* nmhdr = reinterpret_cast<NMHDR*>(m.LParam());
			if (nmhdr->code == PSN_APPLY && this->Changed()) {
				data_.Save();
				this->Changed(false);
			}
			m.Result(TRUE);
		}
		
		/* ----------------------------------------------------------------- */
		/// WndProc
		/* ----------------------------------------------------------------- */
		virtual void WndProc(PsdotNet::Forms::Message& m) {
			switch (m.Msg()) {
			case WM_COMMAND:
				this->OnCommand(m);
				if (m.Result()) {
					PropSheet_Changed(::GetParent(this->Handle()) , this->Handle());
					this->Changed(true);
				}
				break;
			default:
				super::WndProc(m);
				break;
			}
		}
		
	private:
		/* ----------------------------------------------------------------- */
		///
		/// InitializeComponent
		///
		/// <summary>
		/// 初期設定を行います。ウィンドウ生成直後（ハンドルの取得済み）
		/// の初期処理については OnCreateControl() に記述します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void InitializeComponent() {}
		
	private:
		UserSetting& data_;
		bool_ptr changed_;
	}; // class SettingPage
} // namespace CubeICE

#endif // CUBEICE_SETTING_SETTING_PAGE_H
