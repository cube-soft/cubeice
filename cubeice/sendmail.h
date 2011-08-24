/* ------------------------------------------------------------------------- */
/*
 *  sendmail.h
 *
 *  Copyright (c) 2010 CubeSoft Inc.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see < http://www.gnu.org/licenses/ >.
 *
 *  Last-modified: Tue 28 Dec 2010 20:42:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef	CUBEICE_SENDMAIL_H
#define	CUBEICE_SENDMAIL_H

#include <windows.h>
#include <mapi.h>
#include <tchar.h>
#include <vector>
#include <string>

namespace cube {
	namespace utility {
		namespace sendmail {
			namespace detail {
				/* --------------------------------------------------------- */
				//  Exec
				/* --------------------------------------------------------- */
				inline bool Exec( MapiMessage &mms ) {
					HINSTANCE	hDll;
					bool		res = false;

					hDll = LoadLibrary( TEXT( "mapi32.dll" ) );
					if( !hDll )
						return false;

					ULONG ( WINAPI *SendMail )( LHANDLE, ULONG, lpMapiMessage, FLAGS, ULONG );
					reinterpret_cast<FARPROC&>(SendMail) = GetProcAddress( hDll, "MAPISendMail" );

					if( SendMail && SendMail( 0, 0, &mms, MAPI_DIALOG | MAPI_LOGON_UI, 0 ) == SUCCESS_SUCCESS ) {
						res = true;
					}
					
					FreeLibrary( hDll );
					return res;
				}
			}

			typedef	std::basic_string<TCHAR>		tstring;

			/* ------------------------------------------------------------- */
			//  SendMail
			/* ------------------------------------------------------------- */
			inline bool SendMail( const char *subject, const char *body ) {
				MapiMessage		mms;
				ZeroMemory( &mms, sizeof( mms ) );
				mms.lpszSubject		= const_cast<char*>(subject);
				mms.lpszNoteText	= const_cast<char*>(body);
				mms.flFlags			= MAPI_RECEIPT_REQUESTED;
				
				return detail::Exec( mms );
			}
			
			/* ------------------------------------------------------------- */
			//  SendMail
			/* ------------------------------------------------------------- */
			inline bool SendMail( const char *subject, const char *body, const char *attach, const char *name = NULL ) {
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

#endif	// CUBEICE_SENDMAIL_H
