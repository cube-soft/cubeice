// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-setting/archive-page.h
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
#ifndef CUBEICE_SETTING_ARCHIVE_PAGE_H
#define CUBEICE_SETTING_ARCHIVE_PAGE_H

#include "cubeice-setting.h"
#include "setting-page.h"
#include <cubeice/file-dialog.h> // deprecated

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// ArchivePage
	///
	/// <summary>
	/// 圧縮、解凍ページの基底となるプロパティシートページを表示する
	/// クラスです。共通の処理をここで定義します。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class ArchivePage : public SettingPage {
	private:
		typedef SettingPage super;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		ArchivePage(const string_type& resource_name, UserSetting& data, int_ptr changed) :
			super(resource_name, data, changed) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~ArchivePage() {}
		
	protected:
		/* ----------------------------------------------------------------- */
		///
		/// LoadOutput
		///
		/// <summary>
		/// 出力先に関する設定を読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		template <class SettingType>
		result_type LoadOutput(const SettingType& data) {
			LOG_DEBUG(_T("ArchivePage::LoadOutput"));
			
			int id = IDC_OUTPUT_SPECIFIC;
			switch (data.OutputCondition()) {
			case OutputKind::Specific:
				id = IDC_OUTPUT_SPECIFIC;
				break;
			case OutputKind::SameAsSource:
				id = IDC_OUTPUT_SOURCE;
				break;
			case OutputKind::Runtime:
				id = IDC_OUTPUT_RUNTIME;
				break;
			default:
				break;
			}
			LOG_TRACE(_T("OutputCondition: %d"), data.OutputCondition());
			
			BOOL enabled = (id == IDC_OUTPUT_SPECIFIC) ? TRUE : FALSE;
			::CheckDlgButton(this->Handle(), id, BST_CHECKED);
			::EnableWindow(::GetDlgItem(this->Handle(), IDC_OUTPUT_PATH), enabled);
			::EnableWindow(::GetDlgItem(this->Handle(), IDC_OUTPUT_PATH_BUTTON), enabled);
			
			if (!data.OutputPath().empty()) ::SetWindowText(::GetDlgItem(this->Handle(), IDC_OUTPUT_PATH), data.OutputPath().c_str());
			LOG_TRACE(_T("OutputPath: %s"), data.OutputPath().c_str());
			
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// LoadOverwrite
		///
		/// <summary>
		/// 上書きの確認方法に関する設定を読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		template <class SettingType>
		result_type LoadOverwrite(const SettingType& data) {
			LOG_DEBUG(_T("ArchivePage::LoadOverwrite"));
			int confirm = BST_UNCHECKED;
			int confirm_older = BST_UNCHECKED;
			
			switch (data.Overwrite()) {
			case OverwriteKind::Force:
				LOG_TRACE(_T("OverwriteKind: Force"));
				confirm = BST_UNCHECKED;
				confirm_older = BST_UNCHECKED;
				break;
			case OverwriteKind::Confirm:
				LOG_TRACE(_T("OverwriteKind: Confirm"));
				confirm = BST_CHECKED;
				confirm_older = BST_UNCHECKED;
				break;
			case OverwriteKind::ConfirmOlder:
				LOG_TRACE(_T("OverwriteKind: ConfirmOlder"));
				confirm = BST_CHECKED;
				confirm_older = BST_CHECKED;
			default:
				return TRUE;
			}
			
			::CheckDlgButton(this->Handle(), IDC_DETAIL_CONFIRM, confirm);
			::CheckDlgButton(this->Handle(), IDC_DETAIL_CONFIRMOLDER, confirm_older);
			::EnableWindow(::GetDlgItem(this->Handle(), IDC_DETAIL_CONFIRMOLDER), confirm == BST_CHECKED ? TRUE : FALSE);
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// LoadFiltering
		///
		/// <summary>
		/// フィルタリングの有無に関する設定を読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		template <class SettingType>
		result_type LoadFiltering(const SettingType& data) {
			LOG_DEBUG(_T("ArchivePage::LoadFiltering (%s)"), (data.Filtering() ? _T("Checked") : _T("Unchecked")));
			::CheckDlgButton(this->Handle(), IDC_DETAIL_FILTERING, data.Filtering() ? TRUE : FALSE);
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// LoadOpen
		///
		/// <summary>
		/// 保存先フォルダを開くかどうかに関する設定を読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		template <class SettingType>
		result_type LoadOpen(const SettingType& data) {
			LOG_DEBUG(_T("ArchivePage::LoadOpen"));
			int open = BST_UNCHECKED;
			int desktop = BST_UNCHECKED;
			
			switch (data.Open()) {
			case OpenKind::Skip:
				LOG_TRACE(_T("OpenKind: Skip"));
				open = BST_UNCHECKED;
				desktop = BST_UNCHECKED;
				break;
			case OpenKind::Open:
				LOG_TRACE(_T("OpenKind: Open"));
				open = BST_CHECKED;
				desktop = BST_UNCHECKED;
				break;
			case OpenKind::SkipDesktop:
				LOG_TRACE(_T("OpenKind: SkipDesktop"));
				open = BST_CHECKED;
				desktop = BST_CHECKED;
				break;
			default:
				return TRUE;
			}
			
			::CheckDlgButton(this->Handle(), IDC_DETAIL_OPEN, open);
			::CheckDlgButton(this->Handle(), IDC_DETAIL_SKIPDESKTOP, desktop);
			::EnableWindow(::GetDlgItem(this->Handle(), IDC_DETAIL_SKIPDESKTOP), open == BST_CHECKED ? TRUE : FALSE);
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveOutputCondition
		///
		/// <summary>
		/// 出力先に関する設定を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		template <class SettingType>
		result_type SaveOutputCondition(SettingType& data, int id) {
			LOG_DEBUG(_T("ArchivePage::SaveOutputCondition"));
			
			switch (id) {
			case IDC_OUTPUT_SPECIFIC:
				data.OutputCondition(OutputKind::Specific);
				break;
			case IDC_OUTPUT_SOURCE:
				data.OutputCondition(OutputKind::SameAsSource);
				break;
			case IDC_OUTPUT_RUNTIME:
				data.OutputCondition(OutputKind::Runtime);
				break;
			default:
				return FALSE;
			}
			
			return this->LoadOutput(data);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveOutputPath
		///
		/// <summary>
		/// 出力先に関する設定を保存します。
		/// TODO: PsdotNet で browsefolder を実装する
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		template <class SettingType>
		result_type SaveOutputPath(SettingType& data, int id) {
			LOG_DEBUG(_T("ArchivePage::SaveOutputPath (%d)"), id);
			if (id == IDC_OUTPUT_PATH_BUTTON) {
				string_type path = cubeice::dialog::browsefolder(this->Handle(), data.OutputPath().c_str(), _T("保存先ディレクトリを選択して下さい。"));
				if (path.empty()) return FALSE;
				data.OutputPath(path);
				::SetWindowText(::GetDlgItem(this->Handle(), IDC_OUTPUT_PATH), data.OutputPath().c_str());
			}
			else data.OutputPath(PsdotNet::Forms::Utility::GetText(::GetDlgItem(this->Handle(), id)));
			
			LOG_TRACE(_T("OutputPath: %s"), data.OutputPath().c_str());
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveOverwrite
		///
		/// <summary>
		/// 上書き方法に関する設定を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		template <class SettingType>
		result_type SaveOverwrite(SettingType& data, int /* id */) {
			LOG_DEBUG(_T("ArchivePage::SaveOverwrite"));
			bool confirm = ::IsDlgButtonChecked(this->Handle(), IDC_DETAIL_CONFIRM) == BST_CHECKED;
			bool confirm_older = ::IsDlgButtonChecked(this->Handle(), IDC_DETAIL_CONFIRMOLDER) == BST_CHECKED;
			if (confirm && confirm_older) data.Overwrite(OverwriteKind::ConfirmOlder);
			else if (confirm) data.Overwrite(OverwriteKind::Confirm);
			else data.Overwrite(OverwriteKind::Force);
			return this->LoadOverwrite(data);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveFiltering
		///
		/// <summary>
		/// フィルタリングに関する設定を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		template <class SettingType>
		result_type SaveFiltering(SettingType& data, int id) {
			data.Filtering(::IsDlgButtonChecked(this->Handle(), id) == BST_CHECKED);
			LOG_DEBUG(_T("ArchivePage::SaveFiltering (%s)"), (data.Filtering() ? _T("Checked") : _T("Unchecked")));
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveOpen
		///
		/// <summary>
		/// 保存先フォルダを開くかどうかに関する設定を読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		template <class SettingType>
		result_type SaveOpen(SettingType& data, int /* id */) {
			LOG_DEBUG(_T("ArchivePage::SaveOpen"));
			bool open = ::IsDlgButtonChecked(this->Handle(), IDC_DETAIL_OPEN) == BST_CHECKED;
			bool desktop = ::IsDlgButtonChecked(this->Handle(), IDC_DETAIL_SKIPDESKTOP) == BST_CHECKED;
			if (open && desktop) data.Open(OpenKind::SkipDesktop);
			else if (open) data.Open(OpenKind::Open);
			else data.Open(OpenKind::Skip);
			return this->LoadOpen(data);
		}
	}; // class ArchivePage
} // namespace CubeICE

#endif // CUBEICE_SETTING_ARCHIVE_PAGE_H
