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
#include "resource.h"

// ログ出力を無効にする．
#ifndef PSDOTNET_INVALIDATE_LOG_MACRO
#define PSDOTNET_INVALIDATE_LOG_MACRO
#endif
#include <cubeice/user-setting.h>

const int	ICON_SIZE = 16;
const int	ICON_Y_SIZE = ICON_SIZE;
const int	ICON_X_SIZE = ICON_SIZE;

class IMenuInfo {
public:
	typedef std::basic_string<TCHAR>		tstring;

	virtual const std::vector<tstring> &GetFileList() = 0;
	virtual const tstring &GetFilePath( const std::size_t &index ) = 0;
	virtual std::size_t GetSize() = 0;
	virtual tstring GetOption() = 0;
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
	{ COMP_ZIP_FLAG,		ICON_NOT_USED,	MAKE_AWSTRING( "zip" ),					NULL,				MAKE_AWSTRING( "zip 圧縮" ),				TEXT( "/c:zip" ) },
	{ COMP_ZIP_PASS_FLAG,	ICON_NOT_USED,	MAKE_AWSTRING( "zip (パスワード)" ),	NULL,				MAKE_AWSTRING( "zip 圧縮 (パスワード)" ),		TEXT( "/c:zip /p" ) },
	{ COMP_SEVENZIP_FLAG,	ICON_NOT_USED,	MAKE_AWSTRING( "7z" ),					NULL,				MAKE_AWSTRING( "7-zip 圧縮" ),			TEXT( "/c:7z" ) },
	{ COMP_BZIP2_FLAG,		ICON_NOT_USED,	MAKE_AWSTRING( "bzip2" ),				NULL,				MAKE_AWSTRING( "bzip2 圧縮" ),			TEXT( "/c:bzip2" ) },
	{ COMP_GZIP_FLAG,		ICON_NOT_USED,	MAKE_AWSTRING( "gzip" ),				NULL,				MAKE_AWSTRING( "gzip 圧縮" ),			TEXT( "/c:gzip" ) },
	{ COMP_EXE_FLAG,		ICON_NOT_USED,	MAKE_AWSTRING( "exe" ),					NULL,				MAKE_AWSTRING( "自己解凍形式で圧縮" ),		TEXT( "/c:exe" ) },
	{ COMP_DETAIL_FLAG,		ICON_NOT_USED,	MAKE_AWSTRING( "詳細設定" ),			NULL,				MAKE_AWSTRING( "詳細を設定して圧縮" ),			TEXT( "/c:detail" ) },
	{ 0, ICON_NOT_USED, NULL, NULL, NULL, NULL, NULL, NULL }		// end marker
};
const SUB_MENU_ITEM		SubMenuCompAndMail[] = {
	{ MAIL_ZIP_FLAG,		ICON_NOT_USED,	MAKE_AWSTRING( "zip" ),					NULL,				MAKE_AWSTRING( "zip 圧縮してメール送信" ),				TEXT( "/c:zip /m" ) },
	{ MAIL_ZIP_PASS_FLAG,	ICON_NOT_USED,	MAKE_AWSTRING( "zip (パスワード)" ),	NULL,				MAKE_AWSTRING( "zip 圧縮してメール送信 (パスワード)" ),		TEXT( "/c:zip /p /m" ) },
	{ MAIL_SEVENZIP_FLAG,	ICON_NOT_USED,	MAKE_AWSTRING( "7z" ),					NULL,				MAKE_AWSTRING( "7-zip 圧縮してメール送信" ),			TEXT( "/c:7z /m" ) },
	{ MAIL_BZIP2_FLAG,		ICON_NOT_USED,	MAKE_AWSTRING( "bzip2" ),				NULL,				MAKE_AWSTRING( "bzip2 圧縮してメール送信" ),			TEXT( "/c:bzip2 /m" ) },
	{ MAIL_GZIP_FLAG,		ICON_NOT_USED,	MAKE_AWSTRING( "gzip" ),				NULL,				MAKE_AWSTRING( "gzip 圧縮してメール送信" ),			TEXT( "/c:gzip /m" ) },
	{ MAIL_EXE_FLAG,		ICON_NOT_USED,	MAKE_AWSTRING( "exe" ),					NULL,				MAKE_AWSTRING( "自己解凍形式で圧縮してしてメール送信" ),		TEXT( "/c:exe /m" ) },
	{ MAIL_DETAIL_FLAG,		ICON_NOT_USED,	MAKE_AWSTRING( "詳細設定" ),			NULL,				MAKE_AWSTRING( "圧縮してメール送信 (詳細設定)" ),			TEXT( "/c:detail /m" ) },
	{ 0, ICON_NOT_USED, NULL, NULL, NULL, NULL, NULL, NULL }		// end marker
};
const SUB_MENU_ITEM		SubMenuDecompress[] = {
	{ DECOMP_HERE_FLAG,		ICON_NOT_USED,	MAKE_AWSTRING( "ここに解凍" ),			NULL,				MAKE_AWSTRING( "この場所に解凍" ),		TEXT( "/x:source" ) },
	{ DECOMP_DESKTOP_FLAG,	ICON_NOT_USED,	MAKE_AWSTRING( "デスクトップに解凍" ),	NULL,				MAKE_AWSTRING( "デスクトップに解凍" ),	TEXT( "/x:desktop" ) },
	{ DECOMP_MYDOCUMENTS_FLAG,ICON_NOT_USED,	MAKE_AWSTRING( "マイドキュメントに解凍" ),	NULL,				MAKE_AWSTRING( "マイドキュメントに解凍" ),	TEXT( "/x:mydocuments" ) },
	{ DECOMP_RUNTIME_FLAG,	ICON_NOT_USED,	MAKE_AWSTRING( "場所を指定して解凍" ),	NULL,				MAKE_AWSTRING( "場所を指定して解凍" ),	TEXT( "/x:runtime" ) },
	{ 0, ICON_NOT_USED, NULL, NULL, NULL, NULL, NULL, NULL }		// end marker
};

const SUB_MENU_ITEM		MenuItem[] = {
	{ COMPRESS_FLAG,	IDB_COMPRESS,		MAKE_AWSTRING( "圧縮" ),				SubMenuCompress,	MAKE_AWSTRING( "圧縮" ),			NULL },
	{ DECOMPRESS_FLAG,	IDB_DECOMPRESS,		MAKE_AWSTRING( "解凍" ),				SubMenuDecompress,	MAKE_AWSTRING( "解凍" ),			NULL },
	{ MAIL_FLAG,		IDB_COMPRESS,		MAKE_AWSTRING( "圧縮してメール送信" ),	SubMenuCompAndMail,	MAKE_AWSTRING( "圧縮してメール送信" ),	NULL },
//	{ SETTING_FLAG,		IDB_LOGO,		MAKE_AWSTRING( "設定" ),				NULL,				MAKE_AWSTRING( "設定を行う" ),			NULL },
	{ 0, ICON_NOT_USED, NULL, NULL, NULL, NULL, NULL, NULL }		// end marker
};

#endif	// SHL_CTX_MENU_DATA_H