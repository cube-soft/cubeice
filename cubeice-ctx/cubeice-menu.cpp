/* ------------------------------------------------------------------------- */
/*
 *  cubeice-menu.cpp
 *
 *  Copyright (c) 2010 CubeSoft Inc.
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
 *  Last-modified: Wed 12 Jan 2011 14:28:04 JST
 */
/* ------------------------------------------------------------------------- */
// for C4996 warning (_tcscpy)
#define _CRT_SECURE_NO_WARNINGS

#include <cubeice/config.h>
#include <shlwapi.h>
#include <vector>
#include "cubeice-ctxbase.h"

/* ------------------------------------------------------------------------- */
//  MenuSelectedCallback
/* ------------------------------------------------------------------------- */
void MenuSelectedCallback(const CubeICE::Context& src, IMenuInfo *info)
{
	CubeICE::string_type commandLine;
	
	commandLine = TEXT( "\"" );
	commandLine += src.TargetPath();
	commandLine += TEXT( "\" " );
	if (!src.Arguments().empty()) commandLine += src.Arguments();
	commandLine += info->GetOption();
	
	const std::vector<IMenuInfo::string_type>& fileList = info->GetFileList();
	for( std::vector<IMenuInfo::string_type>::const_iterator it = fileList.begin(), stop = fileList.end() ; it != stop ; ++it ) {
		commandLine += TEXT( " \"" ) + *it + TEXT( "\"" );
	}
	
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
