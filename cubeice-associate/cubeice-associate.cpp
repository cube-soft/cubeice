// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-associate/cubeice-associate.cpp
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
#include <cubeice/user-setting.h>
#include <psdotnet/registry.h>

int _tmain(int argc, TCHAR* argv[]) {
	std::basic_stringstream<CubeICE::char_type> ss;
	ss << CUBEICE_REG_ROOT << _T("\\") << CUBEICE_REG_STRUCTVERSION << _T("\\Association");
	PsdotNet::RegistryKey subkey = PsdotNet::Registry::CurrentUser().CreateSubKey(ss.str());
	if (!subkey) return -1;

	CubeICE::UserAssociationSetting::Synchronize(subkey);
	return 0;
}