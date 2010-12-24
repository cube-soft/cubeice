#ifndef		SHL_CTX_MENU_DATA_H
#define		SHL_CTX_MENU_DATA_H

#define		MAKE_AWSTRING( text )		text, L##text

#include <windows.h>
#include <vector>
#include <string>
#include "user-setting.h"
#include "resource.h"

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
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "zip(pass)" ),			NULL,				MAKE_AWSTRING( "zip圧縮(パス)" ),		TEXT( "/c:zip /p" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "7-zip" ),				NULL,				MAKE_AWSTRING( "7-zip圧縮" ),			TEXT( "/c:7z" ) },
//	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "bzip2" ),				NULL,				MAKE_AWSTRING( "bzip2圧縮" ),			TEXT( "/c:bzip2" ) },
//	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "gzip" ),				NULL,				MAKE_AWSTRING( "gzip圧縮" ),			TEXT( "/c:gzip" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "exe" ),					NULL,				MAKE_AWSTRING( "自己解凍形式" ),		TEXT( "/c:exe" ) },
	{ 0, ICON_NOT_USED, NULL, NULL, NULL, NULL, NULL, NULL }		// end marker
};
const SUB_MENU_ITEM		SubMenuDecompress[] = {
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "ここに展開" ),			NULL,				MAKE_AWSTRING( "この場所に展開" ),		TEXT( "/x:here" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "デスクトップに展開" ),	NULL,				MAKE_AWSTRING( "デスクトップに展開" ),	TEXT( "/x:desktop" ) },
	{ 0,				ICON_NOT_USED,	MAKE_AWSTRING( "場所を指定して展開" ),	NULL,				MAKE_AWSTRING( "場所を指定して展開" ),	TEXT( "/x:specified" ) },
	{ 0, ICON_NOT_USED, NULL, NULL, NULL, NULL, NULL, NULL }		// end marker
};

const SUB_MENU_ITEM		MenuItem[] = {
	{ COMPRESS_FLAG,	IDB_LOGO,		MAKE_AWSTRING( "CubeICEで圧縮" ),		SubMenuCompress,	MAKE_AWSTRING( "圧縮を行う" ),			NULL },
	{ DECOMPRESS_FLAG,	IDB_LOGO,		MAKE_AWSTRING( "CubeICEで展開" ),		SubMenuDecompress,	MAKE_AWSTRING( "展開を行う" ),			NULL },
	{ SETTING_FLAG,		IDB_LOGO,		MAKE_AWSTRING( "CubeICEの設定" ),		NULL,				MAKE_AWSTRING( "設定を行う" ),			TEXT( "" ) },
	{ 0, ICON_NOT_USED, NULL, NULL, NULL, NULL, NULL, NULL }		// end marker
};

#endif	// SHL_CTX_MENU_DATA_H