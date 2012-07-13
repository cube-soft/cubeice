// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-setting/compression-page.h
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
#ifndef CUBEICE_SETTING_COMPRESSION_PAGE_H
#define CUBEICE_SETTING_COMPRESSION_PAGE_H

#include "cubeice-setting.h"
#include "archive-page.h"

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// CompressionPage
	///
	/// <summary>
	/// SettingDialog に表示させる「圧縮」ページです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class CompressionPage : public ArchivePage {
	private:
		typedef ArchivePage super;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		CompressionPage(UserSetting& data, int_ptr changed) :
			super(_T("IDD_COMPRESSION"), data, changed) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~CompressionPage() {}
		
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
			LOG_DEBUG(_T("CompressionPage::OnCreateControl"));
			this->LoadOutput(this->Data().Compression());
			this->LoadOverwrite(this->Data().Compression());
			this->LoadFiltering(this->Data().Compression());
			this->LoadOpen(this->Data().Compression());
			this->LoadDeleteOnMail();
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
				m.Result(this->SaveOutputCondition(this->Data().Compression(), id));
				break;
			case IDC_OUTPUT_PATH:
				if (HIWORD(m.WParam()) == EN_KILLFOCUS) {
					m.Result(this->SaveOutputPath(this->Data().Compression(), id));
				}
				break;
			case IDC_OUTPUT_PATH_BUTTON:
				m.Result(this->SaveOutputPath(this->Data().Compression(), id));
				break;
			case IDC_DETAIL_CONFIRM:
			case IDC_DETAIL_CONFIRMOLDER:
				m.Result(this->SaveOverwrite(this->Data().Compression(), id));
				break;
			case IDC_DETAIL_FILTERING:
				m.Result(this->SaveFiltering(this->Data().Compression(), id));
				break;
			case IDC_DETAIL_OPEN:
			case IDC_DETAIL_SKIPDESKTOP:
				m.Result(this->SaveOpen(this->Data().Compression(), id));
				break;
			case IDC_DETAIL_DELETEONMAIL:
				m.Result(this->SaveDeleteOnMail());
				break;
			default:
				super::OnCommand(m);
				break;
			}
		}
		
	private:
		/* ----------------------------------------------------------------- */
		///
		/// LoadDeleteOnMail
		///
		/// <summary>
		/// メール送信後に削除する機能に関する設定を読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type LoadDeleteOnMail() {
			UserCompressionSetting& data = this->Data().Compression();
			LOG_DEBUG(_T("CompressionPage::LoadDeleteOnMail (%s)"), (data.DeleteOnMail() ? _T("Checked") : _T("Unchecked")));
			::CheckDlgButton(this->Handle(), IDC_DETAIL_DELETEONMAIL, data.DeleteOnMail() ? TRUE : FALSE);
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveDeleteOnMail
		///
		/// <summary>
		/// メール送信後に削除する機能に関する設定を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type SaveDeleteOnMail() {
			UserCompressionSetting& data = this->Data().Compression();
			data.DeleteOnMail(::IsDlgButtonChecked(this->Handle(), IDC_DETAIL_DELETEONMAIL) == BST_CHECKED);
			LOG_DEBUG(_T("CompressionPage::SaveDeleteOnMail (%s)"), (data.DeleteOnMail() ? _T("Checked") : _T("Unchecked")));
			return TRUE;
		}
	}; // class CompressionPage
} // namespace CubeICE

#endif // CUBEICE_SETTING_COMPRESSION_PAGE_H
