// Common/StringConvert.cpp

#include "StdAfx.h"

#include "StringConvert.h"
#include <babel/babel.h>
#include <tchar.h>

#ifndef _WIN32
#include <stdlib.h>
#endif

#ifdef _WIN32
UString MultiByteToUnicodeString(const AString &srcString, UINT codePage)
{
  static const char *utf_8_mac_mapping[][2] = {
  	{ "\xE3\x81\x8B\xE3\x82\x99", "\xE3\x81\x8C" },		// が
  	{ "\xE3\x81\x8D\xE3\x82\x99", "\xE3\x81\x8E" },		// ぎ
  	{ "\xE3\x81\x8F\xE3\x82\x99", "\xE3\x81\x90" },		// ぐ
  	{ "\xE3\x81\x91\xE3\x82\x99", "\xE3\x81\x92" },		// げ
  	{ "\xE3\x81\x93\xE3\x82\x99", "\xE3\x81\x94" },		// ご
  	{ "\xE3\x81\x95\xE3\x82\x99", "\xE3\x81\x96" },		// ざ
  	{ "\xE3\x81\x97\xE3\x82\x99", "\xE3\x81\x98" },		// じ
  	{ "\xE3\x81\x99\xE3\x82\x99", "\xE3\x81\x9A" },		// ず
  	{ "\xE3\x81\x9B\xE3\x82\x99", "\xE3\x81\x9C" },		// ぜ
  	{ "\xE3\x81\x9D\xE3\x82\x99", "\xE3\x81\x9E" },		// ぞ
  	{ "\xE3\x81\x9F\xE3\x82\x99", "\xE3\x81\xA0" },		// だ
  	{ "\xE3\x81\xA1\xE3\x82\x99", "\xE3\x81\xA2" },		// ぢ
  	{ "\xE3\x81\xA4\xE3\x82\x99", "\xE3\x81\xA5" },		// づ
  	{ "\xE3\x81\xA6\xE3\x82\x99", "\xE3\x81\xA7" },		// で
  	{ "\xE3\x81\xA8\xE3\x82\x99", "\xE3\x81\xA9" },		// ど
  	{ "\xE3\x81\xAF\xE3\x82\x99", "\xE3\x81\xB0" },		// ば
  	{ "\xE3\x81\xB2\xE3\x82\x99", "\xE3\x81\xB3" },		// び
  	{ "\xE3\x81\xB5\xE3\x82\x99", "\xE3\x81\xB6" },		// ぶ
  	{ "\xE3\x81\xB8\xE3\x82\x99", "\xE3\x81\xB9" },		// べ
  	{ "\xE3\x81\xBB\xE3\x82\x99", "\xE3\x81\xBC" },		// ぼ
  	{ "\xE3\x81\xAF\xE3\x82\x9A", "\xE3\x81\xB1" },		// ぱ
  	{ "\xE3\x81\xB2\xE3\x82\x9A", "\xE3\x81\xB4" },		// ぴ
  	{ "\xE3\x81\xB5\xE3\x82\x9A", "\xE3\x81\xB7" },		// ぷ
  	{ "\xE3\x81\xB8\xE3\x82\x9A", "\xE3\x81\xBA" },		// ぺ
  	{ "\xE3\x81\xBB\xE3\x82\x9A", "\xE3\x81\xBD" },		// ぽ
  	{ "\xE3\x82\xAB\xE3\x82\x99", "\xE3\x82\xAC" },		// ガ
  	{ "\xE3\x82\xAD\xE3\x82\x99", "\xE3\x82\xAE" },		// ギ
  	{ "\xE3\x82\xAF\xE3\x82\x99", "\xE3\x82\xB0" },		// グ
  	{ "\xE3\x82\xB1\xE3\x82\x99", "\xE3\x82\xB2" },		// ゲ
  	{ "\xE3\x82\xB3\xE3\x82\x99", "\xE3\x82\xB4" },		// ゴ
  	{ "\xE3\x82\xB5\xE3\x82\x99", "\xE3\x82\xB6" },		// ザ
  	{ "\xE3\x82\xB7\xE3\x82\x99", "\xE3\x82\xB8" },		// ジ
  	{ "\xE3\x82\xB9\xE3\x82\x99", "\xE3\x82\xBA" },		// ズ
  	{ "\xE3\x82\xBB\xE3\x82\x99", "\xE3\x82\xBC" },		// ゼ
  	{ "\xE3\x82\xBD\xE3\x82\x99", "\xE3\x82\xBE" },		// ゾ
  	{ "\xE3\x82\xBF\xE3\x82\x99", "\xE3\x83\x80" },		// ダ
  	{ "\xE3\x83\x81\xE3\x82\x99", "\xE3\x83\x82" },		// ヂ
  	{ "\xE3\x83\x84\xE3\x82\x99", "\xE3\x83\x85" },		// ヅ
  	{ "\xE3\x83\x86\xE3\x82\x99", "\xE3\x83\x87" },		// デ
  	{ "\xE3\x83\x88\xE3\x82\x99", "\xE3\x83\x89" },		// ド
  	{ "\xE3\x83\x8F\xE3\x82\x99", "\xE3\x83\x90" },		// バ
  	{ "\xE3\x83\x92\xE3\x82\x99", "\xE3\x83\x93" },		// ビ
  	{ "\xE3\x83\x95\xE3\x82\x99", "\xE3\x83\x96" },		// ブ
  	{ "\xE3\x83\x98\xE3\x82\x99", "\xE3\x83\x99" },		// ベ
  	{ "\xE3\x83\x9B\xE3\x82\x99", "\xE3\x83\x9C" },		// ボ
  	{ "\xE3\x83\x8F\xE3\x82\x9A", "\xE3\x83\x91" },		// パ
  	{ "\xE3\x83\x92\xE3\x82\x9A", "\xE3\x83\x94" },		// ピ
  	{ "\xE3\x83\x95\xE3\x82\x9A", "\xE3\x83\x97" },		// プ
  	{ "\xE3\x83\x98\xE3\x82\x9A", "\xE3\x83\x9A" },		// ペ
  	{ "\xE3\x83\x9B\xE3\x82\x9A", "\xE3\x83\x9D" },		// ポ
  };
  UString resultString;
  if (!srcString.IsEmpty())
  {
#if SEVENZIP_ORIGINAL
    int numChars = MultiByteToWideChar(codePage, 0, srcString,
      srcString.Length(), resultString.GetBuffer(srcString.Length()),
      srcString.Length() + 1);
    if (numChars == 0)
      throw 282228;
    resultString.ReleaseBuffer(numChars);
#else
    babel::init_babel();
    babel::analyze_result analyze_result = babel::analyze_base_encoding((const char*)srcString);
    switch(analyze_result.get_strict_result()) {
    case babel::base_encoding::utf8 : {
        AString convString = srcString;
        for( unsigned int i = 0 ; i < sizeof( utf_8_mac_mapping ) / sizeof( utf_8_mac_mapping[0] ) ; ++i ) {
            convString.Replace( utf_8_mac_mapping[i][0], utf_8_mac_mapping[i][1] );
        }
        resultString = babel::utf8_to_unicode((const char*)convString).c_str(); break;
    }
    case babel::base_encoding::sjis : {
        bool f = false;
        DWORD flags;
        DWORD size = sizeof(flags);
        //if (RegGetValue(HKEY_CURRENT_USER, _T("Software\\CubeSoft\\CubeICE\\Decompress"), _T("Details"), RRF_RT_REG_DWORD, NULL, &flags, &size) == ERROR_SUCCESS)
        //    f = ((flags & 0x10000000) != 0);
        if (codePage == CP_UTF8 && f) { // Windows 以外で圧縮されたファイル．恐らく babel の推測ミス．
            AString convString = srcString;
            for( unsigned int i = 0 ; i < sizeof( utf_8_mac_mapping ) / sizeof( utf_8_mac_mapping[0] ) ; ++i ) {
                convString.Replace( utf_8_mac_mapping[i][0], utf_8_mac_mapping[i][1] );
            }
            resultString = babel::utf8_to_unicode((const char*)convString).c_str(); break;
        }
        else resultString = babel::sjis_to_unicode((const char*)srcString).c_str();
        break;
    }
    case babel::base_encoding::euc :  resultString = babel::euc_to_unicode((const char*)srcString).c_str(); break;
    case babel::base_encoding::jis :  resultString = babel::jis_to_unicode((const char*)srcString).c_str(); break;
    default: 
      int numChars = MultiByteToWideChar(codePage, 0, srcString,
        srcString.Length(), resultString.GetBuffer(srcString.Length()),
        srcString.Length() + 1);
      if (numChars == 0) throw 282228;
      resultString.ReleaseBuffer(numChars);
      break;
    }
#endif
  }
  return resultString;
}

AString UnicodeStringToMultiByte(const UString &s, UINT codePage, char defaultChar, bool &defaultCharWasUsed)
{
  AString dest;
  defaultCharWasUsed = false;
  if (!s.IsEmpty())
  {
    int numRequiredBytes = s.Length() * 2;
    BOOL defUsed;
    int numChars = WideCharToMultiByte(codePage, 0, s, s.Length(),
        dest.GetBuffer(numRequiredBytes), numRequiredBytes + 1,
        &defaultChar, &defUsed);
    defaultCharWasUsed = (defUsed != FALSE);
    if (numChars == 0)
      throw 282229;
    dest.ReleaseBuffer(numChars);
  }
  return dest;
}

AString UnicodeStringToMultiByte(const UString &srcString, UINT codePage)
{
  bool defaultCharWasUsed;
  return UnicodeStringToMultiByte(srcString, codePage, '_', defaultCharWasUsed);
}

#ifndef UNDER_CE
AString SystemStringToOemString(const CSysString &srcString)
{
  AString result;
  CharToOem(srcString, result.GetBuffer(srcString.Length() * 2));
  result.ReleaseBuffer();
  return result;
}
#endif

#else

UString MultiByteToUnicodeString(const AString &srcString, UINT codePage)
{
  UString resultString;
  for (int i = 0; i < srcString.Length(); i++)
    resultString += wchar_t(srcString[i]);
  /*
  if (!srcString.IsEmpty())
  {
    int numChars = mbstowcs(resultString.GetBuffer(srcString.Length()), srcString, srcString.Length() + 1);
    if (numChars < 0) throw "Your environment does not support UNICODE";
    resultString.ReleaseBuffer(numChars);
  }
  */
  return resultString;
}

AString UnicodeStringToMultiByte(const UString &srcString, UINT codePage)
{
  AString resultString;
  for (int i = 0; i < srcString.Length(); i++)
    resultString += char(srcString[i]);
  /*
  if (!srcString.IsEmpty())
  {
    int numRequiredBytes = srcString.Length() * 6 + 1;
    int numChars = wcstombs(resultString.GetBuffer(numRequiredBytes), srcString, numRequiredBytes);
    if (numChars < 0) throw "Your environment does not support UNICODE";
    resultString.ReleaseBuffer(numChars);
  }
  */
  return resultString;
}

#endif
