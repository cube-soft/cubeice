/* ------------------------------------------------------------------------- */
/*
 *  cubeice/config.h
 *
 *  Copyright (c) 2010 CubeSoft Inc.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_CONFIG_H
#define CUBEICE_CONFIG_H

#include <psdotnet/config.h>

/* -------------------------------------------------------------------------- */
//  アプリケーション名一覧
/* -------------------------------------------------------------------------- */
#define CUBEICE_NAME                _T("cubeice.exe")
#define CUBEICE_EXEC_NAME           _T("cubeice-exec.exe")
#define CUBEICE_SETTING_NAME        _T("cubeice-setting.exe")
#define CUBEICE_DETAIL_NAME         _T("cubeice-detail.exe")
#define CUBEICE_CHECKER_NAME        _T("cubeice-checker.exe")
#define CUBEICE_ASSOCIATE_NAME      _T("cubeice-associate.exe")

/* -------------------------------------------------------------------------- */
//  GUID
/* -------------------------------------------------------------------------- */
// {F3DB85F4-4731-4e80-BC2E-754A7320D830}
DEFINE_GUID(CLSID_CubeICE_CTX, 0xf3db85f4, 0x4731, 0x4e80, 0xbc, 0x2e, 0x75, 0x4a, 0x73, 0x20, 0xd8, 0x30);
#define CUBEICE_CLSID_CONTEXT _T("{F3DB85F4-4731-4e80-BC2E-754A7320D830}")
#define CUBEICE_CLSID_TOOLTIP _T("{00021500-0000-0000-C000-000000000046}")

// {AD7C6B17-656C-4DD5-B88C-B4B5253A19BD}
DEFINE_GUID(CLSID_CubeICE_PROPSHEET, 0xad7c6b17, 0x656c, 0x4dd5, 0xb8, 0x8c, 0xb4, 0xb5, 0x25, 0x3a, 0x19, 0xbd);

/* -------------------------------------------------------------------------- */
//  GUI 画面のスタイルの設定
/* -------------------------------------------------------------------------- */
#if defined(_UNICODE) || defined(UNICODE)
#if defined(_M_IX86)
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined(_M_IA64)
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined(_M_X64)
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif // _M_IX86/_M_IA64/_M_X64/Others
#endif // UNICODE

namespace CubeICE {
	/* ---------------------------------------------------------------------- */
	//  型定義
	/* ---------------------------------------------------------------------- */
	typedef PsdotNet::TString string_type;
	typedef string_type::value_type char_type;
	using PsdotNet::optional;
} // namespace CubeICE

// NOTE: リファクタリングが終わったら削除する。
namespace cubeice {
	using namespace CubeICE;
}

#endif // CUBEICE_CONFIG_H
