/* ------------------------------------------------------------------------- */
/*
 *  cubeice-ctxdata.h
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
#ifndef		SHL_CTX_MENU_DATA_H
#define		SHL_CTX_MENU_DATA_H

#define		MAKE_AWSTRING( text )		text, L##text

#include <windows.h>
#include <vector>
#include <string>
#include "user-setting.h"
#include "resource.h"


const int	ICON_SIZE = 16;
const int	ICON_Y_SIZE = ICON_SIZE;
const int	ICON_X_SIZE = ICON_SIZE;

class IMenuInfo {
public:
	typedef std::basic_string<TCHAR>		tstring;

	virtual const std::vector<tstring> &GetFileList() = 0;
	virtual const tstring &GetFilePath( const std::size_t &index ) = 0;
	virtual std::size_t GetSize() = 0;
};

void MenuSelectedCallback( const LPCTSTR arg, IMenuInfo *info );

struct SUB_MENU_ITEM {
	cubeice::user_setting::size_type	dispSetting;		// Display setting
	WORD								iconID;				// Display menu item icon
	LPCSTR								stringA;			// Display menu item string [A]
	LPCWSTR								stringW;			// Display menu item string [W]
	const SUB_MENU_ITEM					*submenu;			// Submenu
	LPCSTR								explanationA;		// Explanation string [A]
	LPCWSTR								explanationW;		// Explanation string [W]
	LPCTSTR								arg;				// Callback argument
};

const WORD		ICON_NOT_USED = -1;

const SUB_MENU_ITEM		SubMenuCompress[] = {
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "zip" ),					NULL,				MAKE_AWSTRING( "zip圧縮" ),				TEXT( "/c:zip" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "zip (パスワード)" ),	NULL,				MAKE_AWSTRING( "zip圧縮(パス)" ),		TEXT( "/c:zip /p" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "7z" ),					NULL,				MAKE_AWSTRING( "7-zip圧縮" ),			TEXT( "/c:7z" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "bzip2" ),				NULL,				MAKE_AWSTRING( "bzip2圧縮" ),			TEXT( "/c:bzip2" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "gzip" ),				NULL,				MAKE_AWSTRING( "gzip圧縮" ),			TEXT( "/c:gzip" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "詳細設定" ),			NULL,				MAKE_AWSTRING( "詳細設定" ),			TEXT( "/c:detail" ) },
	//	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "exe" ),					NULL,				MAKE_AWSTRING( "自己解凍形式" ),		TEXT( "/c:exe" ) },
	{ 0, ICON_NOT_USED, NULL, NULL, NULL, NULL, NULL, NULL }		// end marker
};
const SUB_MENU_ITEM		SubMenuCompAndMail[] = {
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "zip" ),					NULL,				MAKE_AWSTRING( "zip圧縮" ),				TEXT( "/c:zip /m" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "zip (パスワード)" ),	NULL,				MAKE_AWSTRING( "zip圧縮(パス)" ),		TEXT( "/c:zip /p /m" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "7z" ),					NULL,				MAKE_AWSTRING( "7-zip圧縮" ),			TEXT( "/c:7z /m" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "bzip2" ),				NULL,				MAKE_AWSTRING( "bzip2圧縮" ),			TEXT( "/c:bzip2 /m" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "gzip" ),				NULL,				MAKE_AWSTRING( "gzip圧縮" ),			TEXT( "/c:gzip /m" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "詳細設定" ),			NULL,				MAKE_AWSTRING( "詳細設定" ),			TEXT( "/c:detail /m" ) },
//	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "exe" ),					NULL,				MAKE_AWSTRING( "自己解凍形式" ),		TEXT( "/c:exe /m" ) },
	{ 0, ICON_NOT_USED, NULL, NULL, NULL, NULL, NULL, NULL }		// end marker
};
const SUB_MENU_ITEM		SubMenuDecompress[] = {
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "ここに解凍" ),			NULL,				MAKE_AWSTRING( "この場所に解凍" ),		TEXT( "/x:source" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "デスクトップに解凍" ),	NULL,				MAKE_AWSTRING( "デスクトップに解凍" ),	TEXT( "/x:desktop" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "場所を指定して解凍" ),	NULL,				MAKE_AWSTRING( "場所を指定して解凍" ),	TEXT( "/x:runtime" ) },
	{ 0, ICON_NOT_USED, NULL, NULL, NULL, NULL, NULL, NULL }		// end marker
};

const SUB_MENU_ITEM		MenuItem[] = {
	{ COMPRESS_FLAG,	IDB_COMPRESS,		MAKE_AWSTRING( "圧縮" ),				SubMenuCompress,	MAKE_AWSTRING( "圧縮を行う" ),			NULL },
	{ DECOMPRESS_FLAG,	IDB_DECOMPRESS,		MAKE_AWSTRING( "解凍" ),				SubMenuDecompress,	MAKE_AWSTRING( "展開を行う" ),			NULL },
	{ MAIL_FLAG,		IDB_COMPRESS,		MAKE_AWSTRING( "圧縮してメール送信" ),	SubMenuCompAndMail,	MAKE_AWSTRING( "圧縮してメール送信" ),	NULL },
//	{ SETTING_FLAG,		IDB_LOGO,		MAKE_AWSTRING( "設定" ),				NULL,				MAKE_AWSTRING( "設定を行う" ),			NULL },
	{ 0, ICON_NOT_USED, NULL, NULL, NULL, NULL, NULL, NULL }		// end marker
};

#endif	// SHL_CTX_MENU_DATA_H