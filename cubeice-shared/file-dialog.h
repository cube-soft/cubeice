/* ------------------------------------------------------------------------- */
/*
 *  file-dialog.cpp
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
		inline std::basic_string<TCHAR> openfile(HWND hWnd, const TCHAR* filter, const TCHAR* init, const TCHAR* title = _T("ファイルを開く")) {
			typedef TCHAR char_type;
			char_type path[CUBE_MAX_PATH] = {};
			char_type filename[CUBE_MAX_PATH] = {};
			if (init != NULL) _tcsncpy_s(path, CUBE_MAX_PATH, init, _tcslen(init));
			
			OPENFILENAME ofn = {};
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
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
		inline std::basic_string<TCHAR> savefile(HWND hWnd, const TCHAR* filter, const TCHAR* init, const DWORD flag = OFN_OVERWRITEPROMPT, const TCHAR* title = _T("名前を付けて保存")) {
			typedef TCHAR char_type;
			typedef std::basic_string<TCHAR> string_type;
			
			char_type path[CUBE_MAX_PATH] = {};
			char_type filename[CUBE_MAX_PATH] = {};
			if (init != NULL) _tcsncpy_s(path, CUBE_MAX_PATH, init, _tcslen(init));
			
			OPENFILENAME ofn = {};
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = filter;
			ofn.lpstrFile = path;
			ofn.lpstrFileTitle = filename;
			ofn.nMaxFile = CUBE_MAX_PATH;
			ofn.nMaxFileTitle = CUBE_MAX_PATH;
			ofn.Flags = flag;
			ofn.lpstrTitle = title;
			
			if (GetSaveFileName(&ofn)) return std::basic_string<char_type>(path);
			else return std::basic_string<char_type>();
		}
		
		/* ----------------------------------------------------------------- */
		//  browse_proc
		/* ----------------------------------------------------------------- */
		static int CALLBACK browse_proc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
			if (uMsg == BFFM_INITIALIZED){
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
		inline std::basic_string<TCHAR> browsefolder(HWND hWnd, const TCHAR* init, const TCHAR* description = _T("フォルダの参照")) {
			typedef TCHAR char_type;
			
			BROWSEINFO info = {};
			info.hwndOwner = hWnd;
			info.pszDisplayName = const_cast<TCHAR*>(init);
			info.lpszTitle = description;
			info.ulFlags = BIF_RETURNONLYFSDIRS; // | BIF_NEWDIALOGSTYLE;
			info.lpfn = &browse_proc;
			info.lParam = (LPARAM)init;
			LPITEMIDLIST dest = SHBrowseForFolder(&info);
			
			char_type path[CUBE_MAX_PATH] = {};
			//GetCurrentDirectory(CUBE_MAX_PATH, path);
			SHGetPathFromIDList(dest, path);
			CoTaskMemFree(dest);
			
			return path;
		}
	}
}
