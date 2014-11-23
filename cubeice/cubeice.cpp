/* ------------------------------------------------------------------------- */
/*
 *  cubeice.cpp
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
 *  Last-modified: Fri 11 Jan 2011 13:59:00 JST
 */
/* ------------------------------------------------------------------------- */
#include "cubeice.h"
#include <cubeice/style.h>
#include <windows.h>
#include <psdotnet/logger.h>
#include <psdotnet/logger/file-appender.h>

cubeice::user_setting UserSetting;

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR pCmdLine, int showCmd) {
#ifndef PSDOTNET_INVALIDATE_LOG_MACRO
	std::basic_string<TCHAR> path(UserSetting.install_path() + _T("\\cubeice.log"));
	PsdotNet::FileAppender writer(path, PsdotNet::FileAppender::CloseOnWrite | PsdotNet::FileAppender::WriteAll);
	PsdotNet::Logger::Configure(writer, PsdotNet::Utility::ToLogLevel(UserSetting.loglevel()));
#endif
	
	LOG_INFO(_T("CubeICE version %s"), UserSetting.version().c_str());
	
	OSVERSIONINFO osinfo;
	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osinfo);
	std::basic_string<TCHAR> edition = sizeof(INT_PTR) == 4 ? _T("x86") : _T("x64");
	LOG_INFO(_T("Windows version %d.%d.%d (%s)"), osinfo.dwMajorVersion, osinfo.dwMinorVersion, osinfo.dwBuildNumber, edition.c_str());
	LOG_INFO(_T("InstallPath = %s"), UserSetting.install_path().c_str());
	LOG_INFO(_T("LogLevel = %d"), UserSetting.loglevel());
	LOG_INFO(_T("CmdLine = %s"), pCmdLine);
	
	cubeice::cmdline::separator sep(_T(" \t"), _T("\""), _T(""));
	cubeice::cmdline::splitter args(pCmdLine, sep);
	cubeice::cmdline::splitter::iterator pos = args.begin();
	
	cubeice::archiver ar(UserSetting);
	if (pos != args.end() && pos->compare(0, 3, _T("/c:")) == 0) ar.compress(pos, args.end());
	else if (pos != args.end() && pos->compare(0, 2, _T("/x")) == 0) ar.decompress(pos, args.end());
	else {
		// デフォルトは設定画面を開く．	

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
