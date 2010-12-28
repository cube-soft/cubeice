/* ------------------------------------------------------------------------- */
/*
 *  cubeice.cpp
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
 *  Last-modified: Mon 20 Dec 2010 22:05:00 JST
 */
/* ------------------------------------------------------------------------- */
#include "cubeice.h"
#include <windows.h>

cubeice::user_setting UserSetting;

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR pCmdLine, int showCmd) {
	cubeice::cmdline::separator sep(_T(" \t"), _T("\""), _T(""));
	cubeice::cmdline::splitter args(pCmdLine, sep);
	cubeice::cmdline::splitter::iterator pos = args.begin();

	cubeice::archiver ar(UserSetting);
	if (pos != args.end() && pos->compare(0, 3, _T("/c:")) == 0) ar.compress(pos, args.end());
	else if (pos != args.end() && pos->compare(0, 2, _T("/x")) == 0) ar.decompress(pos, args.end());
	else {
		// デフォルトは設定画面を開く．
		OSVERSIONINFO ovi = {};
		ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&ovi);
		
		// 設定プログラムのパスを取得．
		TCHAR buffer[CUBE_MAX_PATH] ={};
		GetModuleFileName(hInst, buffer, CUBE_MAX_PATH);
		std::basic_string<TCHAR> tmp = buffer;
		std::basic_string<TCHAR> path = tmp.substr(0, tmp.find_last_of(_T('\\'))) + _T("\\cubeice-setting.exe");
		
		HINSTANCE proc = ShellExecute(NULL, _T("runas"), path.c_str(), NULL, NULL, SW_SHOWNORMAL);
		DWORD result = (DWORD)proc;
		if (result <= 32) return -1;
	}
	
	return 0;
}
