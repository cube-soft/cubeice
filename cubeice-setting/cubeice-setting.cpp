#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include <string>
#include "cubeice-setting.h"
#include "dialog.h"
#include "style.h"

cubeice::user_setting Setting;

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR pCmdLine, int showCmd) {
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
		
		Setting.context_flags() |= (COMPRESS_FLAG | DECOMPRESS_FLAG);
		Setting.shortcut_flags() |= (COMPRESS_FLAG | DECOMPRESS_FLAG);
	}
	
	cubeice::create_propsheet(NULL, args == _T("install"));
	
	MSG  msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 0;
}
