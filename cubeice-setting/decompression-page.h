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
			this->LoadDeleteOnExtract();
		}
		
		/* ----------------------------------------------------------------- */
		/// OnCommand
		/* ----------------------------------------------------------------- */
		virtual void OnCommand(PsdotNet::Forms::Message& m) {
			int id = LOWORD(m.WParam());
			
			switch (id) {
			case IDC_OUTPUT_SPECIFIC:
			case IDC_OUTPUT_SOURCE:
			case IDC_OUTPUT_RUNTIME:
				m.Result(this->SaveOutputCondition(this->Data().Decompression(), id));
				break;
			case IDC_OUTPUT_PATH_BUTTON:
				m.Result(this->SaveOutputPath(this->Data().Decompression()));
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
			case IDC_DETAIL_DELETEONEXTRACT:
				m.Result(this->SaveDeleteOnExtract());
				break;
			default:
				super::OnCommand(m);
				break;
			}
		}
		
	private:
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
			LOG_DEBUG(_T("ArchivePage::LoadDeleteOnExtract (%s)"), (data.DeleteOnExtract() ? _T("Checked") : _T("Unchecked")));
			::CheckDlgButton(this->Handle(), IDC_DETAIL_DELETEONEXTRACT, data.DeleteOnExtract() ? TRUE : FALSE);
			return TRUE;
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
		result_type SaveDeleteOnExtract() {
			UserDecompressionSetting& data = this->Data().Decompression();
			data.DeleteOnExtract(::IsDlgButtonChecked(this->Handle(), IDC_DETAIL_DELETEONEXTRACT) == BST_CHECKED);
			LOG_DEBUG(_T("ArchivePage::SaveDeleteOnExtract (%s)"), (data.DeleteOnExtract() ? _T("Checked") : _T("Unchecked")));
			return TRUE;
		}
	}; // class CompressionPage
} // namespace CubeICE

#endif // CUBEICE_SETTING_DECOMPRESSION_PAGE_H
