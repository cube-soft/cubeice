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
	/// ChangeKind
	/* --------------------------------------------------------------------- */
	namespace ChangeKind {
		enum { None, Normal, RequireAdmin };
	}
	
	/* --------------------------------------------------------------------- */
	///
	/// SettingPage
	///
	/// <summary>
	/// SettingDialog に表示させる各ページの基底クラスです。
	/// プロパティシートでは、OK ボタンや適用ボタンののイベントがシート内に
	/// 表示されている全てのページに通知されるため、changed と言う共有
	/// 変数を用いて、保存処理が複数回行われないようにします。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class SettingPage : public PsdotNet::Forms::PropertyPage {
	private:
		typedef PsdotNet::Forms::PropertyPage super;
		
	public:
		typedef boost::shared_ptr<int> int_ptr;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		SettingPage(const string_type& resource_name, UserSetting& data, int_ptr changed) :
			super(resource_name), data_(data), changed_(changed) {}
		
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
		int Changed() const { return *changed_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Changed
		///
		/// <summary>
		/// データに対して、何らかの変更が行われたかどうかを設定します。
		/// 設定する値は、以下の通りです。
		///
		///   0: 保存が必要な変更なし
		///   1: ユーザ権限で保存が必要な変更が発生
		///   2: 管理者権限で保存が必要な変更が発生
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Changed(int level) { *changed_ = level; }
		
	protected:
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
			if (nmhdr->code == PSN_APPLY && this->Changed() >= ChangeKind::Normal) {
				data_.Save();
				if (this->Changed() >= ChangeKind::RequireAdmin) this->SaveAsAdministrator();
				this->Changed(ChangeKind::None);
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
					if (this->Changed() == 0) this->Changed(ChangeKind::Normal);
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
		/// SaveAsAdministrator
		///
		/// <summary>
		/// 管理者権限で変更されたユーザ設定を保存します。現在、管理者権限
		/// の必要な変更は、関連付けの変更のみです。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void SaveAsAdministrator() {
			::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
			string_type associate = data_.InstallDirectory() + _T("\\") + CUBEICE_ASSOCIATE_NAME;
			SHELLEXECUTEINFO info = {};
			info.cbSize = sizeof(SHELLEXECUTEINFO);
			info.lpFile = associate.c_str();
			::ShellExecuteEx(&info);
			::CoUninitialize();
		}
		
	private:
		UserSetting& data_;
		int_ptr changed_;
	}; // class SettingPage
} // namespace CubeICE

#endif // CUBEICE_SETTING_SETTING_PAGE_H
