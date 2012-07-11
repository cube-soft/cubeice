// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-detail/cubeice-detail.cpp
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
#include <cubeice/config.h>
#include <psdotnet/application.h>
#include <psdotnet/forms/msgbox.h>
#include <cubeice/user-setting.h>
#include "mainform.h"

#define CUBEICE_DETAIL_WARNING _T("\
CubeICE 詳細設定からの変更によって発生した不都合についてはサポート外となります。\
設定変更によって影響を受ける範囲を把握している場合のみご利用下さい。\r\n\r\n\
設定の変更を続けますか？")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	CubeICE::UserSetting setting;
	setting.Load();
	
	if (setting.WarnOnDetail()) {
		PsdotNet::Forms::MsgBoxButtons button = PsdotNet::Forms::MsgBoxButtons::YesNo;
		PsdotNet::Forms::MsgBoxIcon icon = PsdotNet::Forms::MsgBoxIcon::Warning;
		if (PsdotNet::Forms::MsgBox::Show(CUBEICE_DETAIL_WARNING, _T("CubeICE 詳細設定"), button, icon) == PsdotNet::Forms::DialogResult::No) return 0;
	}
	
	CubeICE::MainForm form(setting);
	PsdotNet::Application::Run(form);
	
	return 0;
}
