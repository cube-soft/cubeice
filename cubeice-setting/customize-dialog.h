// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-setting/customize-dialog.h
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
#ifndef CUBEICE_SETTING_CUSTOMIZE_DIALOG_H
#define CUBEICE_SETTING_CUSTOMIZE_DIALOG_H

#include "cubeice-setting.h"
#include <psdotnet/forms/modal-form.h>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// CustomizeDialog
	///
	/// <summary>
	/// コンテキストメニューのカスタマイズのためのダイアログです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class CustomizeDialog : public PsdotNet::Forms::ModalForm {
	private:
		typedef PsdotNet::Forms::ModalForm super;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		CustomizeDialog(UserSetting& data) :
			super(_T("IDD_CUSTOMIZE")), data_(data) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~CustomizeDialog() {}
		
	private:
		UserSetting& data_;
	};
} // namespace CubeICE

#endif // CUBEICE_SETTING_CUSTOMIZE_DIALOG_H
