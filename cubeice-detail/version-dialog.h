// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-detail/version-dialog.h
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
#ifndef CUBEICE_DETAIL_VERSION_DIALOG_H
#define CUBEICE_DETAIL_VERSION_DIALOG_H

#include <cubeice/config.h>
#include <psdotnet/forms.h>
#include <cubeice/user-setting.h>
#include "resource.h"

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	/// VersionDialog
	/* --------------------------------------------------------------------- */
	class VersionDialog : public PsdotNet::Forms::ModalForm {
	private:
		typedef PsdotNet::Forms::ModalForm super;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		explicit VersionDialog(const UserSetting& setting) :
			super(_T("IDD_VERSION")), setting_(setting) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~VersionDialog() {}
		
	protected:
		virtual void OnCreateControl() {
			HANDLE logo = ::LoadImage(GetModuleHandle(NULL), _T("IDI_APP"), IMAGE_ICON, 48, 48, LR_DEFAULTCOLOR | LR_SHARED | LR_VGACOLOR);
			handle_type pic = GetDlgItem(this->Handle(), IDC_VERSION_LOGO);
			::SendMessage(pic, STM_SETIMAGE, IMAGE_ICON, LPARAM(logo));
			
			string_type edition = _T("Version: ");
			edition += setting_.Version();
			if (sizeof(INT_PTR) == 4) edition += _T(" (x86)");
			else edition += _T(" (x64)");
			::SetWindowText(::GetDlgItem(this->Handle(), IDC_VERSION_EDITION), edition.c_str());
			
			super::OnCreateControl();
		}
		
	private:
		const UserSetting& setting_;
	}; // class VersionDialog
} // namespace CubeICE

#endif // CUBEICE_DETAIL_VERSION_DIALOG_H
