#ifndef	SENDMAIL_H
#define	SENDMAIL_H

#include <windows.h>
#include <mapi.h>
#include <tchar.h>
#include <vector>
#include <string>

namespace cube {
	namespace utility {
		namespace sendmail {
			namespace detail {
				bool Exec( MapiMessage &mms ) {
					HINSTANCE	hDll;
					bool		res = false;

					hDll = LoadLibrary( TEXT( "mapi32.dll" ) );
					if( !hDll )
						return false;

					ULONG ( WINAPI *SendMail )( LHANDLE, ULONG, lpMapiMessage, FLAGS, ULONG );
					reinterpret_cast<FARPROC&>(SendMail) = GetProcAddress( hDll, "MAPISendMail" );

					if( SendMail && SendMail( 0, 0, &mms, MAPI_DIALOG | MAPI_LOGON_UI, 0 ) == SUCCESS_SUCCESS )
						res = true;

					FreeLibrary( hDll );
					return res;
				}
			}

			typedef	std::basic_string<TCHAR>		tstring;

			bool SendMail( const char *subject, const char *body ) {
				MapiMessage		mms;
				ZeroMemory( &mms, sizeof( mms ) );
				mms.lpszSubject		= const_cast<char*>(subject);
				mms.lpszNoteText	= const_cast<char*>(body);
				mms.flFlags			= MAPI_RECEIPT_REQUESTED;

				return detail::Exec( mms );
			}

			bool SendMail( const char *subject, const char *body, const char *attach, const char *name = NULL ) {
				MapiFileDesc	mfd;

				ZeroMemory( &mfd, sizeof( mfd ) );
				mfd.lpszPathName	= const_cast<char*>(attach);
				mfd.lpszFileName	= const_cast<char*>(name);

				MapiMessage		mms;
				ZeroMemory( &mms, sizeof( mms ) );
				mms.lpszSubject		= const_cast<char*>(subject);
				mms.lpszNoteText	= const_cast<char*>(body);
				mms.flFlags			= MAPI_RECEIPT_REQUESTED;
				mms.nFileCount		= 1;
				mms.lpFiles			= &mfd;

				return detail::Exec( mms );
			}
		}
	}
}

#endif	// SENDMAIL_H
