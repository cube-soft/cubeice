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
		result_type LoadOutput(SettingType& data) {
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
			LOG_TRACE(_T("OutputCondition: %d"), id);
			
			BOOL enabled = (id == IDC_OUTPUT_SPECIFIC) ? TRUE : FALSE;
			::CheckDlgButton(this->Handle(), id, BST_CHECKED);
			::EnableWindow(::GetDlgItem(this->Handle(), IDC_OUTPUT_PATH), enabled);
			::EnableWindow(::GetDlgItem(this->Handle(), IDC_OUTPUT_PATH_BUTTON), enabled);
			
			if (!data.OutputPath().empty()) ::SetWindowText(::GetDlgItem(this->Handle(), IDC_OUTPUT_PATH), data.OutputPath().c_str());
			LOG_TRACE(_T("OutputPath: %s"), data.OutputPath().c_str());
			
			return TRUE;
		}
	}; // class ArchivePage
} // namespace CubeICE

#endif // CUBEICE_SETTING_ARCHIVE_PAGE_H
