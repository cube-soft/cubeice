// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-setting/version-page.h
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
#ifndef CUBEICE_SETTING_VERSION_PAGE_H
#define CUBEICE_SETTING_VERSION_PAGE_H

#include "cubeice-setting.h"
#include <sstream>
#include <psdotnet/drawing.h>
#include "setting-page.h"

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// CompressionPage
	///
	/// <summary>
	/// SettingDialog に表示させる「バージョン情報」ページです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class VersionPage : public SettingPage {
	private:
		typedef SettingPage super;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		VersionPage(UserSetting& data, int_ptr changed) :
			super(_T("IDD_VERSION"), data, changed) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~VersionPage() {}
		
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
			LOG_DEBUG(_T("VersionPage::OnCreateControl"));
			UserSetting& data = this->Data();
			
			// ロゴの表示
			PsdotNet::Drawing::Icon::Type type = PsdotNet::Drawing::Icon::Type::Resource;
			PsdotNet::Drawing::Icon logo(type, _T("IDI_APP"), PsdotNet::Drawing::Size(48, 48));
			handle_type handle = GetDlgItem(this->Handle(), IDC_VERSION_LOGO);
			::SendMessage(handle, STM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(logo.Handle()));
			
			// バージョン
			std::basic_stringstream<char_type> ss;
			int edition = PsdotNet::Environment::Is64BitProcess() ? 64 : 86;
			ss << _T("Version: ") << data.Version() << _T(" (x") << edition << _T(")");
			::SetWindowText(::GetDlgItem(this->Handle(), IDC_VERSION_VERSION), ss.str().c_str());
			
			// アップデートの確認
			if (data.CheckUpdate()) ::CheckDlgButton(this->Handle(), IDC_VERSION_UPDATE, BST_CHECKED);
			
			// エラーレポートを表示
			if (data.ErrorReport()) ::CheckDlgButton(this->Handle(), IDC_VERSION_ERRORREPORT, BST_CHECKED);
			
			// デバッグ情報を出力
			if (data.LogLevel() < PsdotNet::LogLevel::Error) ::CheckDlgButton(this->Handle(), IDC_VERSION_DEBUG, BST_CHECKED);
		}
		
		/* ----------------------------------------------------------------- */
		/// OnCommand
		/* ----------------------------------------------------------------- */
		virtual void OnCommand(PsdotNet::Forms::Message& m) {
			UserSetting& data = this->Data();
			int id = LOWORD(m.WParam());
			bool checked = ::IsDlgButtonChecked(this->Handle(), id) == BST_CHECKED;
			
			switch (id) {
			case IDC_VERSION_UPDATE:
				data.CheckUpdate(checked);
				m.Result(TRUE);
				break;
			case IDC_VERSION_ERRORREPORT:
				data.ErrorReport(checked);
				m.Result(TRUE);
				break;
			case IDC_VERSION_DEBUG:
				data.LogLevel(checked ? PsdotNet::LogLevel::Trace : PsdotNet::LogLevel::Error);
				m.Result(TRUE);
				break;
			default:
				m.Result(FALSE);
				break;
			}
		}
	}; // class CompressionPage
} // namespace CubeICE

#endif // CUBEICE_SETTING_VERSION_PAGE_H
