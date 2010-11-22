#include "cubeice.h"
#include "tcharext.h"

#include <windows.h>

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR pCmdLine, int showCmd) {
	cube::cmdline::separator sep(_T(" \t"), _T("\""), _T(""));
	cube::cmdline::splitter args(pCmdLine, sep);
	cube::cmdline::splitter::iterator pos = args.begin();
	
	cube::archiver ar;
	if (pos != args.end() && *pos == _T("-c")) ar.compress(pos, args.end());
	else if (pos != args.end() && *pos == _T("-x")) ar.decompress(pos, args.end());
	else {
		cube::tcerr << _T("error") << std::endl;
		return -1;
	}

	return 0;
}
