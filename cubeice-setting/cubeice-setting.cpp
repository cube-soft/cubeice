#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include "cubeice-setting.h"
#include "dialog.h"

cubeice::user_setting Setting;
HANDLE MultiGuard;

static void WakeupWindow(HWND hWnd) {
	if (IsIconic(hWnd)) ShowWindowAsync(hWnd, SW_RESTORE);
	SetForegroundWindow(hWnd);
}

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR pCmdLine, int showCmd) {
	MultiGuard = CreateMutex(NULL, TRUE,  "MultiplexStartingPreventionTest");
	if (GetLastError() == ERROR_ALREADY_EXISTS){
		WakeupWindow(FindWindow(NULL, _T("CubeICE ê›íË")));
		ReleaseMutex(MultiGuard);
		CloseHandle(MultiGuard);
		return -1;
	}
	
	cubeice::create_propsheet(NULL);
	
	MSG  msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	ReleaseMutex(MultiGuard);
	CloseHandle(MultiGuard); 
	return 0;
}
