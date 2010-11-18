/* ------------------------------------------------------------------------- */
/*
 *  dialog.cpp
 *
 *  Copyright (c) 2010 CubeSoft.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    - No names of its contributors may be used to endorse or promote
 *      products derived from this software without specific prior written
 *      permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  Last-modified: Wed 17 Nov 2010 17:39:00 JST
 */
/* ------------------------------------------------------------------------- */
#include "dialog.h"
#include <string>
#include <windows.h>
#include <tchar.h>

namespace cube {
	namespace win32api {
		/* ----------------------------------------------------------------- */
		/*
		 *  open_dialog
		 *
		 *  MEMO: filter (OPENFILENAME.lpstrFilter) は区切り文字として
		 *  '\0' を使用しているため string は使用できない．
		 */
		/* ----------------------------------------------------------------- */
		std::basic_string<TCHAR> open_dialog(const TCHAR* filter, const TCHAR* title) {
			typedef TCHAR char_type;
			char_type path[CUBE_MAX_PATH] = {};
			char_type filename[CUBE_MAX_PATH] = {};
			
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
		//  save_dialog
		/* ----------------------------------------------------------------- */
		std::basic_string<TCHAR> save_dialog(const TCHAR* filter, const TCHAR* title) {
			typedef TCHAR char_type;
			char_type path[CUBE_MAX_PATH] = {};
			char_type filename[CUBE_MAX_PATH] = {};
			
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
		 *  folder_dialog
		 *
		 *  see also:
		 *  http://msdn.microsoft.com/en-us/library/bb762115(VS.85).aspx
		 *  http://msdn.microsoft.com/en-us/library/bb773205(VS.85).aspx
		 */
		/* ----------------------------------------------------------------- */
		std::basic_string<TCHAR> folder_dialog(const TCHAR* description) {
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
