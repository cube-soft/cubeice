/* ------------------------------------------------------------------------- */
/*
 *  wpopen.h
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
 *  Last-modified: Tue 21 Dec 2010 11:56:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBE_WPOPEN_H_
#define CUBE_WPOPEN_H_

#include <windows.h>
#include <tchar.h>
#include <string>

namespace cube {
	/* --------------------------------------------------------------------- */
	/*
	 *  popen
	 *
	 *  GUI アプリケーションから使用された場合に DOS ウィンドウが開かない
	 *  ように修正した popen．
	 *  NOTE: created by http://twitter.com/rofi
	 */
	/* --------------------------------------------------------------------- */
	class popen {
	public:
		popen() :
			_mode( NONE ), _hChildProcess( INVALID_HANDLE_VALUE ), _hInputOutput( INVALID_HANDLE_VALUE ), _line( TEXT( "" ) ) {}

		~popen() {
			this->close();
		}

		enum open_mode {
			NONE	= 0x00,
			READ	= 0x01,
			WRITE	= 0x02		// not implements
		};

		bool open( const TCHAR *command, const TCHAR *mode ) {
			open_mode	m = NONE;

			for( unsigned int i = 0 ; mode[i] ; ++i ) {
				switch( mode[i] ) {
					case TEXT( 'r' ):
					case TEXT( 'R' ):
						if( m == WRITE )
							return false;
						m = READ;
						break;
					case TEXT( 'w' ):
					case TEXT( 'W' ):
						if( m == READ )
							return false;
						m = WRITE;
						break;
					default:
						break;
				}
			}

			return open( command, m );
		}

		bool open( const TCHAR *command, const open_mode &mode ) {
			HANDLE					hReadPipe, hWritePipe;
			SECURITY_ATTRIBUTES		sa;
			STARTUPINFO				si;
			PROCESS_INFORMATION		pi;

			if( _hChildProcess != INVALID_HANDLE_VALUE || _hInputOutput != INVALID_HANDLE_VALUE )
				return false;

			if( ( mode == NONE ) || ( ( mode & READ ) && ( mode & WRITE ) ) )
				return false;

			sa.nLength				= sizeof( sa );
			sa.lpSecurityDescriptor	= NULL;
			sa.bInheritHandle		= TRUE;

			if( !CreatePipe( &hReadPipe, &hWritePipe, &sa, 0 ) )
				return false;

			ZeroMemory( &si, sizeof( si ) );
			si.cb			= sizeof( si );
			si.dwFlags		= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
			si.wShowWindow	= SW_HIDE;
			si.hStdInput	= GetStdHandle( STD_INPUT_HANDLE );
			si.hStdOutput	= GetStdHandle( STD_OUTPUT_HANDLE );
			si.hStdError	= GetStdHandle( STD_ERROR_HANDLE );

			if( mode & READ ) {
				si.hStdOutput = hWritePipe;
				if( !DuplicateHandle( GetCurrentProcess(), hReadPipe, GetCurrentProcess(), &_hInputOutput, 0, FALSE, DUPLICATE_SAME_ACCESS ) ) {
					CloseHandle( hReadPipe );
					CloseHandle( hWritePipe );
					return false;
				}
				CloseHandle( hReadPipe );
			} else {
				si.hStdInput = hReadPipe;
				if( DuplicateHandle( GetCurrentProcess(), hWritePipe, GetCurrentProcess(), &_hInputOutput, 0, FALSE, DUPLICATE_SAME_ACCESS ) ) {
					CloseHandle( hReadPipe );
					CloseHandle( hWritePipe );
					return false;
				}
				CloseHandle( hWritePipe );
			}

			TCHAR	*cmdtmp = new TCHAR[(_tcslen(command)+5)*sizeof(command[0])];
			if( !cmdtmp ) {
				if( mode & WRITE )
					CloseHandle( hReadPipe );
				else
					CloseHandle( hWritePipe );
				return false;
			}
			_tcscpy( cmdtmp, command );

			BOOL	bcp = CreateProcess( NULL, cmdtmp, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi );

			delete[] cmdtmp;

			if( mode & WRITE )
				CloseHandle( hReadPipe );
			else
				CloseHandle( hWritePipe );

			if( !bcp )
				return false;

			_hChildProcess = pi.hProcess;

			CloseHandle( pi.hThread );

			_mode = mode;

			return true;
		}

		bool open( const std::basic_string<TCHAR> &command, const std::basic_string<TCHAR> &mode ) {
			return open( command.c_str(), mode.c_str() );
		}

		bool open( const std::basic_string<TCHAR> &command, const open_mode &mode ) {
			return open( command.c_str(), mode );
		}

		void close() {
			if( _hChildProcess != INVALID_HANDLE_VALUE ) {
				TerminateProcess( _hChildProcess, 0 );
				CloseHandle( _hChildProcess );
				_hChildProcess = INVALID_HANDLE_VALUE;
			}
			if( _hInputOutput != INVALID_HANDLE_VALUE ) {
				CloseHandle( _hInputOutput );
				_hInputOutput = INVALID_HANDLE_VALUE;
			}
		}
		
		/* -------------------------------------------
		 *
		 *	return value
		 *		<0	error
		 *		0	no data
		 *		1	data retrieved
		 *		2	pipe closed
		 *
		 * ------------------------------------------- */
		int gets( std::basic_string<TCHAR> &str ) {
			TCHAR	buffer[2048];
			DWORD	nBytesRead;
			DWORD	nTotalBytesAvail;

			str.clear();
			if( !( _mode & READ ) || _hInputOutput == INVALID_HANDLE_VALUE || _hChildProcess == INVALID_HANDLE_VALUE )
				return -1;

			while( PeekNamedPipe( _hInputOutput, buffer, sizeof( buffer ) - sizeof( buffer[0] ) * 2, &nBytesRead, &nTotalBytesAvail, NULL ) ) {
				TCHAR	*pCRLF;
				DWORD	loadSize;
				bool	flag = false;

				if( !nBytesRead ) return 0;

				buffer[nBytesRead/sizeof(buffer[0])] = TEXT( '\0' );

				if( pCRLF = _tcschr( buffer, TEXT( '\n' ) ) ) {
					loadSize = ( pCRLF - buffer + 1 ) * sizeof( buffer[0] );
					flag = true;
				} else {
					loadSize = ( nBytesRead / sizeof( buffer[0] ) ) * sizeof( buffer[0] );
					flag = false;
				}

				if( !ReadFile( _hInputOutput, buffer, loadSize, &nBytesRead, NULL ) || nBytesRead != loadSize )
					return -2;

				if( flag ) {
					buffer[nBytesRead/sizeof(buffer[0])] = TEXT( '\0' );
					_line += buffer;
					if( _line.size() > 1 && _line[_line.size()-2] == TEXT( '\r' ) )
						str = _line.substr( 0, _line.size() - 2 );
					else
						str = _line.substr( 0, _line.size() - 1 );
					_line.clear();
					return 1;
				}
				buffer[nBytesRead/sizeof(buffer[0])] = TEXT( '\0' );
				_line += buffer;
			}

			if( GetLastError() == ERROR_BROKEN_PIPE ) {
				str = _line;
				_line.clear();
				return 2;
			}

			return -3;
		}

	private:
		open_mode					_mode;
		HANDLE						_hInputOutput;
		HANDLE						_hChildProcess;
		std::basic_string<TCHAR>	_line;
	};
}

#endif // CUBE_WPOPEN_H_
