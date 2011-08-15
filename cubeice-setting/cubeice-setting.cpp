#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <psdotnet/logger.h>
#include <psdotnet/appender.h>
#include "cubeice-setting.h"
#include "dialog.h"
#include "style.h"

cubeice::user_setting Setting(true);

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR pCmdLine, int showCmd) {
	Setting.load();
	
#ifndef PSDOTNET_INVALIDATE_LOG_MACRO
	std::basic_string<TCHAR> path(Setting.install_path() + _T("\\cubeice-setting.log"));
	PsdotNet::FileAppender writer(path, PsdotNet::FileAppender::CloseOnWrite | PsdotNet::FileAppender::WriteAll);
	PsdotNet::Logger::Configure(writer, PsdotNet::Utility::ToLogLevel(Setting.loglevel()));
#endif
	
	LOG_INFO(_T("CubeICE version %s"), Setting.version().c_str());
	
	OSVERSIONINFO osinfo;
	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osinfo);
	std::basic_string<TCHAR> edition = sizeof(INT_PTR) == 4 ? _T("x86") : _T("x64");
	LOG_INFO(_T("Windows version %d.%d.%d (%s)"), osinfo.dwMajorVersion, osinfo.dwMinorVersion, osinfo.dwBuildNumber, edition.c_str());
	LOG_INFO(_T("InstallPath = %s"), Setting.install_path().c_str());
	
	std::basic_string<TCHAR> args(pCmdLine);
	if (args == _T("install")) {
		/*
		 * 初期設定
		 *   1. 以下のファイルに関連付け:
		 *      *.zip, *.lzh, *.rar, *.tar, *.gz, *.7z, *.bz2, *.cab, *.tbz, *.tgz, *.xz
		 *   2. 以下のコンテキストメニューを作成
		 *      圧縮/解凍
		 *   3. 以下のショートカットを作成:
		 *      圧縮/解凍
		 */
		Setting.decompression().flags() |= (
			ZIP_FLAG | LZH_FLAG | RAR_FLAG | TAR_FLAG | GZ_FLAG | SEVENZIP_FLAG |
			BZ2_FLAG | CAB_FLAG | TBZ_FLAG | TGZ_FLAG | XZ_FLAG
		);
		
		Setting.context_flags() |= (
			COMPRESS_FLAG | COMP_ALL_FLAG |
			DECOMPRESS_FLAG | DECOMP_ALL_FLAG
		);
		
		Setting.shortcut_flags() |= (COMPRESS_FLAG | DECOMPRESS_FLAG);
		Setting.associate_changed();
	}
	
	cubeice::create_propsheet(NULL, args == _T("install"));
	
	MSG  msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 0;
}
