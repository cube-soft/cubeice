// Common/StdOutStream.cpp

#include "StdAfx.h"

#include <tchar.h>
#include <babel/babel.h>

#include "IntToString.h"
#include "StdOutStream.h"
#include "StringConvert.h"
#include "UTFConvert.h"

#ifdef _MSC_VER
// "was declared deprecated" disabling
#pragma warning(disable : 4996 )
#endif

static const wchar_t *kNewLineChar =  L"\r\n";

static const char *kFileOpenMode = "wt";

extern int g_CodePage;

CStdOutStream  g_StdOut(stdout);
CStdOutStream  g_StdErr(stderr);

bool CStdOutStream::Open(const char *fileName)
{
  Close();
  _stream = fopen(fileName, kFileOpenMode);
  _streamIsOpen = (_stream != 0);
  return _streamIsOpen;
}

bool CStdOutStream::Close()
{
  if (!_streamIsOpen)
    return true;
  if (fclose(_stream) != 0)
    return false;
  _stream = 0;
  _streamIsOpen = false;
  return true;
}

bool CStdOutStream::Flush()
{
  return (fflush(_stream) == 0);
}

CStdOutStream::~CStdOutStream ()
{
  Close();
}

CStdOutStream & CStdOutStream::operator<<(CStdOutStream & (*aFunction)(CStdOutStream  &))
{
  (*aFunction)(*this);
  return *this;
}

CStdOutStream & endl(CStdOutStream & outStream)
{
  return outStream << kNewLineChar;
}

CStdOutStream & CStdOutStream::operator<<(const char *s)
{
//  fputs(s, _stream);

  std::string	str( s );

  if( str.empty() )
	  return *this;

  babel::init_babel();
  if( str[0] == '\n' )
	  str.insert( 0, "\r" );
  for( std::string::size_type i = str.find( "\n" ) ; i != std::string::npos ; i = str.find( "\n", i + 1 ) ) {
	  if( str[i-1] != '\r' ) {
		  str.insert( i, "\r" );
		  ++i;
	  }
  }

  *this << babel::utf8_to_unicode( str ).c_str();
  return *this;
}

CStdOutStream & CStdOutStream::operator<<(const wchar_t *s)
{
/*
  int codePage = g_CodePage;
  if (codePage == -1)
    codePage = CP_OEMCP;
  AString dest;
  if (codePage == CP_UTF8)
    ConvertUnicodeToUTF8(s, dest);
  else
    dest = UnicodeStringToMultiByte(s, (UINT)codePage);
  *this << (const char *)dest;
*/

  std::wstring	wstr( s );

  if( wstr.empty() )
	  return *this;

  if( wstr[0] == L'\n' )
	  wstr.insert( 0, L"\r" );
  for( std::wstring::size_type i = wstr.find( L"\n" ) ; i != std::wstring::npos ; i = wstr.find( L"\n", i + 1 ) ) {
	  if( wstr[i-1] != L'\r' ) {
		  wstr.insert( i, L"\r" );
		  ++i;
	  }
  }

  fwrite( wstr.c_str(), sizeof( wchar_t ), wstr.size(), _stream );

  return *this;
}

CStdOutStream & CStdOutStream::operator<<(char c)
{
//  fputc(c, _stream);

  char	str[2];
  str[0] = c;
  str[1] = '\0';
  *this << str;
  return *this;
}

CStdOutStream & CStdOutStream::operator<<(int number)
{
  char textString[32];
  ConvertInt64ToString(number, textString);
  return operator<<(textString);
}

CStdOutStream & CStdOutStream::operator<<(UInt64 number)
{
  char textString[32];
  ConvertUInt64ToString(number, textString);
  return operator<<(textString);
}
