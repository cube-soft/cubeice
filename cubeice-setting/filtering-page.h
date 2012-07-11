// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-setting/filtering-page.h
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
#ifndef CUBEICE_SETTING_FILTERING_PAGE_H
#define CUBEICE_SETTING_FILTERING_PAGE_H

#include <cubeice/config.h>
#include "setting-page.h"
#include "resource.h"

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
	class FilteringPage : public SettingPage {
	private:
		typedef SettingPage super;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		FilteringPage(UserSetting& data, bool_ptr changed) :
			super(_T("IDD_FILTERING"), data, changed) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~FilteringPage() {}
		
	}; // class CompressionPage
} // namespace CubeICE

#endif // CUBEICE_SETTING_FILTERING_PAGE_H
