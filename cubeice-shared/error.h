/* ------------------------------------------------------------------------- */
/*
 *  error.h
 *
 *  Copyright (c) 2010 - 2011 CubeSoft Inc.
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
#ifndef CUBEICE_ERROR_H
#define CUBEICE_ERROR_H

#include <string>
#include <windows.h>
#include <tchar.h>

namespace cubeice {
	/* --------------------------------------------------------------------- */
	/*
	 *  error_message
	 *
	 *  指定されたエラー ID に対応するメッセージを取得する．
	 *  http://msdn.microsoft.com/ja-jp/library/cc428939.aspx
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<TCHAR> error_message(DWORD id) {
		std::basic_string<TCHAR> dest;
		
		LPVOID buffer = NULL;
		DWORD result = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<TCHAR*>(&buffer), 0, NULL);
		
		if (result > 0 && buffer != NULL) {
			dest = reinterpret_cast<TCHAR*>(buffer);
			LocalFree(buffer);
			if (!dest.empty() && dest[dest.size() - 1] == _T('\n')) dest.erase(dest.size() - 1);
			if (!dest.empty() && dest[dest.size() - 1] == _T('\r')) dest.erase(dest.size() - 1);
		}
		return dest;
	}
	
	/* --------------------------------------------------------------------- */
	/*
	 *  error_message
	 *
	 *  GetLastError() に対応するメッセージを取得する．
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<TCHAR> error_message() {
		return error_message(::GetLastError());
	}
} // namespace cubeice

#endif // CUBEICE_ERROR_H
