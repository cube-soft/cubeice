#include "cubeice.h"
#include <windows.h>

cubeice::user_setting UserSetting;

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR pCmdLine, int showCmd) {
	cubeice::cmdline::separator sep(_T(" \t"), _T("\""), _T(""));
	cubeice::cmdline::splitter args(pCmdLine, sep);
	cubeice::cmdline::splitter::iterator pos = args.begin();

	cubeice::archiver ar(UserSetting);
	if (pos != args.end() && *pos == _T("-c")) ar.compress(pos, args.end());
	else if (pos != args.end() && *pos == _T("-x")) ar.decompress(pos, args.end());
	else return -1;

	return 0;
}
