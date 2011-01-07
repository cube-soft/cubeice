/* ------------------------------------------------------------------------- */
/*
 *  dialog.cpp
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
#include "dialog.h"
#include <string>
#include <windows.h>
#include <tchar.h>

namespace cubeice {
	namespace dialog {
		/* ----------------------------------------------------------------- */
		/*
		 *  openfile
		 *
		 *  NOTE: filter (OPENFILENAME.lpstrFilter) は区切り文字として
		 *  '\0' を使用しているため string は使用できない．
		 */
		/* ----------------------------------------------------------------- */
		std::basic_string<TCHAR> openfile(const TCHAR* filter, const TCHAR* init, const TCHAR* title) {
			typedef TCHAR char_type;
			char_type path[CUBE_MAX_PATH] = {};
			char_type filename[CUBE_MAX_PATH] = {};
			if (init != NULL) _tcsncpy_s(path, CUBE_MAX_PATH, init, _tcslen(init));
			
			OPENFILENAME ofn = {};
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFilter = filter;
			ofn.lpstrFile = path;
			ofn.lpstrFileTitle = filename;
			ofn.nMaxFile = CUBE_MAX_PATH;
			ofn.nMaxFileTitle = CUBE_MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST;
			ofn.lpstrTitle = title;
			
			if (GetOpenFileName(&ofn)) return std::basic_string<char_type>(path);
			else return std::basic_string<char_type>();
		}
		
		/* ----------------------------------------------------------------- */
		//  savefile
		/* ----------------------------------------------------------------- */
		std::basic_string<TCHAR> savefile(const TCHAR* filter, const TCHAR* init, const TCHAR* title) {
			typedef TCHAR char_type;
			typedef std::basic_string<TCHAR> string_type;
			
			char_type path[CUBE_MAX_PATH] = {};
			char_type filename[CUBE_MAX_PATH] = {};
			if (init != NULL) _tcsncpy_s(path, CUBE_MAX_PATH, init, _tcslen(init));
			
			OPENFILENAME ofn = {};
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFilter = filter;
			ofn.lpstrFile = path;
			ofn.lpstrFileTitle = filename;
			ofn.nMaxFile = CUBE_MAX_PATH;
			ofn.nMaxFileTitle = CUBE_MAX_PATH;
			ofn.Flags = OFN_OVERWRITEPROMPT;
			ofn.lpstrTitle = title;
			
			if (GetSaveFileName(&ofn)) return std::basic_string<char_type>(path);
			else return std::basic_string<char_type>();
		}
		
		/* ----------------------------------------------------------------- */
		//  browse_proc
		/* ----------------------------------------------------------------- */
		static int CALLBACK browse_proc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
			if(uMsg == BFFM_INITIALIZED){
				SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
			}
			return 0;
		}

		/* ----------------------------------------------------------------- */
		/*
		 *  browsefolder
		 *
		 *  see also:
		 *  http://msdn.microsoft.com/en-us/library/bb762115(VS.85).aspx
		 *  http://msdn.microsoft.com/en-us/library/bb773205(VS.85).aspx
		 */
		/* ----------------------------------------------------------------- */
		std::basic_string<TCHAR> browsefolder(const TCHAR* description) {
			typedef TCHAR char_type;
			char_type path[CUBE_MAX_PATH] = {};
			GetCurrentDirectory(CUBE_MAX_PATH, path);
			
			BROWSEINFO info = {};
			info.pszDisplayName = path;
			info.lpszTitle = description;
			info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
			info.lpfn = &browse_proc;
			info.lParam = (LPARAM)path;
			LPITEMIDLIST dest = SHBrowseForFolder(&info);
			
			SHGetPathFromIDList(dest, path);
			CoTaskMemFree(dest);
			
			return path;
		}
	}
}
