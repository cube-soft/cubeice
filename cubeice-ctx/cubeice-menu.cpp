// for C4996 warning (_tcscpy)
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include <string>
#include <vector>
#include "cubeice-ctxdata.h"

const LPCTSTR		CUBEICE_FILE_NAME = TEXT( "cubeice.exe" );

extern HINSTANCE	hDllInstance;

void MenuSelectedCallback( const LPCTSTR arg, IMenuInfo *info )
{
	TCHAR						exePath[2048];
	std::basic_string<TCHAR>	commandLine;

	GetModuleFileName( static_cast<HMODULE>( hDllInstance ), exePath, sizeof( exePath ) / sizeof( exePath[0] ) );
	PathRemoveFileSpec( exePath );
	PathAppend( exePath, CUBEICE_FILE_NAME );

	commandLine = TEXT( "\"" );
	commandLine += exePath;
	commandLine += TEXT( "\" " );
	commandLine += arg;
	const std::vector<IMenuInfo::tstring>	&fileList = info->GetFileList();
	for( std::vector<IMenuInfo::tstring>::const_iterator it = fileList.begin(), stop = fileList.end() ; it != stop ; ++it )
		commandLine += TEXT( " \"" ) + *it + TEXT( "\"" );

	STARTUPINFO				si;
	PROCESS_INFORMATION		pi;
	LPTSTR	comline = new TCHAR[commandLine.size()+5];
	_tcscpy( comline, commandLine.c_str() );
	ZeroMemory( &pi, sizeof( pi ) );
	ZeroMemory( &si, sizeof( si ) );
	si.cb = sizeof( si );
	CreateProcess( NULL, comline, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi );
	delete[] comline;
	CloseHandle( pi.hThread );
	CloseHandle( pi.hProcess );

	return;
}
