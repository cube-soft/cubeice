/* ------------------------------------------------------------------------- */
/*
 *  cubeice.h
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
 *
 *  Last-modified: Tue 28 Dec 2010 20:42:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBE_ICE_H
#define CUBE_ICE_H

#include <string>
#include <vector>
#include <tchar.h>

/* ------------------------------------------------------------------------- */
/*
 *  CLX C++ Libraries can be downloaded from the following URL:
 *  http://clx.cielquis.net/
 */
/* ------------------------------------------------------------------------- */
#ifndef CLX_USE_WCHAR
#define CLX_USE_WCHAR
#endif
#include <clx/config.h>
#include <clx/tokenizer.h>

#include <cubeice/config.h>
#include <cubeice/user-setting.h>
#include <cubeice/dialog.h>
#include <cubeice/sendmail.h>
#include "archiver.h"

namespace cubeice {
	namespace cmdline {
		typedef clx::escape_separator<TCHAR> separator;
		typedef clx::basic_tokenizer<clx::escape_separator<TCHAR>, std::basic_string<TCHAR> > splitter;
	}
}

#endif // CUBE_ICE_H