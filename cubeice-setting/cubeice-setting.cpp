#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "cubeice-setting.h"
#include "dialog.h"


int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR pCmdLine, int showCmd) {
	cubeice::create_propsheet(NULL);
	
	MSG  msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
