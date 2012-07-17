// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-setting/decompression-page.h
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
#ifndef CUBEICE_SETTING_DECOMPRESSION_PAGE_H
#define CUBEICE_SETTING_DECOMPRESSION_PAGE_H

#include "cubeice-setting.h"
#include "archive-page.h"

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// CompressionPage
	///
	/// <summary>
	/// SettingDialog に表示させる「解凍」ページです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class DecompressionPage : public ArchivePage {
	private:
		typedef ArchivePage super;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		DecompressionPage(UserSetting& data, int_ptr changed) :
			super(_T("IDD_DECOMPRESSION"), data, changed) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~DecompressionPage() {}
		
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
			LOG_DEBUG(_T("DecompressionPage::OnCreateControl"));
			this->LoadOutput(this->Data().Decompression());
			this->LoadOverwrite(this->Data().Decompression());
			this->LoadFiltering(this->Data().Decompression());
			this->LoadOpen(this->Data().Decompression());
			this->LoadCreateFolder();
			this->LoadDeleteOnExtract();
			this->LoadTooltip();
		}
		
		/* ----------------------------------------------------------------- */
		/// OnCommand
		/* ----------------------------------------------------------------- */
		virtual void OnCommand(PsdotNet::Forms::Message& m) {
			int id = LOWORD(m.WParam());
			
			handle_type handle = ::GetDlgItem(this->Handle(), IDC_DETAIL_TOOLTIPCOUNT_T);
			if (m.LParam() == reinterpret_cast<LPARAM>(handle) && HIWORD(m.WParam()) == EN_CHANGE) {
				m.Result(this->SaveTooltipCount(IDC_DETAIL_TOOLTIPCOUNT));
				return;
			}
			
			switch (id) {
			case IDC_OUTPUT_SPECIFIC:
			case IDC_OUTPUT_SOURCE:
			case IDC_OUTPUT_RUNTIME:
				m.Result(this->SaveOutputCondition(this->Data().Decompression(), id));
				break;
			case IDC_OUTPUT_PATH:
				if (HIWORD(m.WParam()) == EN_KILLFOCUS) {
					m.Result(this->SaveOutputPath(this->Data().Decompression(), id));
				}
				break;
			case IDC_OUTPUT_PATH_BUTTON:
				m.Result(this->SaveOutputPath(this->Data().Decompression(), id));
				break;
			case IDC_DETAIL_CONFIRM:
			case IDC_DETAIL_CONFIRMOLDER:
				m.Result(this->SaveOverwrite(this->Data().Decompression(), id));
				break;
			case IDC_DETAIL_FILTERING:
				m.Result(this->SaveFiltering(this->Data().Decompression(), id));
				break;
			case IDC_DETAIL_OPEN:
			case IDC_DETAIL_SKIPDESKTOP:
				m.Result(this->SaveOpen(this->Data().Decompression(), id));
				break;
			case IDC_DETAIL_CREATEFOLDER:
			case IDC_DETAIL_SKIPSINGLEFOLDER:
				m.Result(this->SaveCreateFolder(id));
				break;
			case IDC_DETAIL_DELETEONEXTRACT:
				m.Result(this->SaveDeleteOnExtract(id));
				break;
			case IDC_DETAIL_TOOLTIP:
				m.Result(this->SaveTooltip(id));
				break;
			default:
				super::OnCommand(m);
				break;
			}
		}
		
	private:
		/* ----------------------------------------------------------------- */
		///
		/// LoadCreateFolder
		///
		/// <summary>
		/// フォルダを作成する方法に関する設定を読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type LoadCreateFolder() {
			LOG_DEBUG(_T("DecompressionPage::LoadCreateFolder"));
			UserDecompressionSetting& data = this->Data().Decompression();
			
			int create = BST_UNCHECKED;
			int single_folder = BST_UNCHECKED;
			
			switch (data.CreateFolder()) {
			case CreateFolderKind::Skip:
				LOG_TRACE(_T("CreateFolderKind: Skip"));
				create = BST_UNCHECKED;
				single_folder = BST_UNCHECKED;
				break;
			case CreateFolderKind::Create:
				LOG_TRACE(_T("CreateFolderKind: Create"));
				create = BST_CHECKED;
				single_folder = BST_UNCHECKED;
				break;
			case CreateFolderKind::SkipSingleFolder:
			case CreateFolderKind::SkipSingleFile:
				LOG_TRACE(_T("CreateFolderKind: SkipSingleFolder"));
				create = BST_CHECKED;
				single_folder = BST_CHECKED;
				break;
			default:
				return TRUE;
			}
			
			::CheckDlgButton(this->Handle(), IDC_DETAIL_CREATEFOLDER, create);
			::CheckDlgButton(this->Handle(), IDC_DETAIL_SKIPSINGLEFOLDER, single_folder);
			::EnableWindow(::GetDlgItem(this->Handle(), IDC_DETAIL_SKIPSINGLEFOLDER), create == BST_CHECKED ? TRUE : FALSE);
			
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// LoadDeleteOnExtract
		///
		/// <summary>
		/// 解凍に削除する機能に関する設定を読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type LoadDeleteOnExtract() {
			UserDecompressionSetting& data = this->Data().Decompression();
			LOG_DEBUG(_T("DecompressionPage::LoadDeleteOnExtract (%s)"), (data.DeleteOnExtract() ? _T("Checked") : _T("Unchecked")));
			::CheckDlgButton(this->Handle(), IDC_DETAIL_DELETEONEXTRACT, data.DeleteOnExtract() ? TRUE : FALSE);
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// LoadTooltip
		///
		/// <summary>
		/// ツールチップの表示に関する設定を読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type LoadTooltip() {
			UserSetting& data = this->Data();
			LOG_DEBUG(_T("DecompressionPage::LoadTooltip (%s)"), (data.Tooltip() ? _T("Checked") : _T("Unchecked")));
			::CheckDlgButton(this->Handle(), IDC_DETAIL_TOOLTIP, data.Tooltip() ? TRUE : FALSE);
			
			handle_type handle = ::GetDlgItem(this->Handle(), IDC_DETAIL_TOOLTIPCOUNT);
			::SendMessage(handle, UDM_SETRANGE, 0, MAKELONG(20, 1));
			::SendMessage(handle, UDM_SETPOS, 0, data.TooltipCount());
			::EnableWindow(handle, data.Tooltip() ? TRUE : FALSE);
			::EnableWindow(::GetDlgItem(this->Handle(), IDC_DETAIL_TOOLTIPCOUNT_T), data.Tooltip() ? TRUE : FALSE);
			
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveCreateFolder
		///
		/// <summary>
		/// フォルダを作成する方法に関する設定を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type SaveCreateFolder(int id) {
			LOG_DEBUG(_T("DecompressionPage::SaveCreateFolder"));
			UserDecompressionSetting& data = this->Data().Decompression();
			
			bool create = ::IsDlgButtonChecked(this->Handle(), IDC_DETAIL_CREATEFOLDER) == BST_CHECKED;
			bool single_folder = ::IsDlgButtonChecked(this->Handle(), IDC_DETAIL_SKIPSINGLEFOLDER) == BST_CHECKED;
			
			if (create && single_folder) data.CreateFolder(CreateFolderKind::SkipSingleFolder);
			else if (create) data.CreateFolder(CreateFolderKind::Create);
			else data.CreateFolder(CreateFolderKind::Skip);
			
			return this->LoadCreateFolder();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveDeleteOnExtract
		///
		/// <summary>
		/// 解凍に削除する機能に関する設定を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type SaveDeleteOnExtract(int /* id */) {
			UserDecompressionSetting& data = this->Data().Decompression();
			data.DeleteOnExtract(::IsDlgButtonChecked(this->Handle(), IDC_DETAIL_DELETEONEXTRACT) == BST_CHECKED);
			LOG_DEBUG(_T("DecompressionPage::SaveDeleteOnExtract (%s)"), (data.DeleteOnExtract() ? _T("Checked") : _T("Unchecked")));
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveTooltip
		///
		/// <summary>
		/// ツールチップの表示に関する設定を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type SaveTooltip(int /* id */) {
			UserSetting& data = this->Data();
			data.Tooltip(::IsDlgButtonChecked(this->Handle(), IDC_DETAIL_TOOLTIP) == BST_CHECKED);
			LOG_DEBUG(_T("DecompressionPage::SaveTooltip (%s)"), (data.Tooltip() ? _T("Checked") : _T("Unchecked")));
			return this->LoadTooltip();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveTooltip
		///
		/// <summary>
		/// ツールチップの表示件数に関する設定を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type SaveTooltipCount(int /* id */) {
			UserSetting& data = this->Data();
			LOG_DEBUG(_T("DecompressionPage::SaveTooltipCount"));
			int n = ::SendMessage(::GetDlgItem(this->Handle(), IDC_DETAIL_TOOLTIPCOUNT), UDM_GETPOS, 0, 0);
			LOG_TRACE(_T("TooltipCount: %d"), n);
			data.TooltipCount(n);
			return TRUE;
		}
	}; // class CompressionPage
} // namespace CubeICE

#endif // CUBEICE_SETTING_DECOMPRESSION_PAGE_H
