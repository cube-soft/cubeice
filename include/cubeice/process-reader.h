// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/process-reader.h
 *
 *  Copyright (c) 2010 CubeSoft, Inc.
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
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_PROCESS_READER_H
#define CUBEICE_PROCESS_READER_H

#include <cubeice/config.h>
#include <tlhelp32.h>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// ProcessReader
	///
	/// <summary>
	/// プロセスを実行し、そのプロセスの標準出力から出力される文字列を
	/// 取得するためのクラスです。popen() をGUI アプリケーションから
	/// 使用した場合に DOS ウィンドウが開かない用に修正したものになります。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class ProcessReader {
	public:
		typedef CubeICE::char_type char_type;
		typedef CubeICE::string_type string_type;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		ProcessReader() :
			_hChildProcess(INVALID_HANDLE_VALUE), _hInputOutput(INVALID_HANDLE_VALUE),
			_line(), _status( 0 ) {}
			
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		~ProcessReader() {
			this->Close();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Open
		///
		/// <summary>
		/// プロセスを実行し、標準出力を読み込み可能な形に初期化します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		bool Open(const string_type& command) {
			return this->OpenExec(command.c_str());
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Close
		///
		/// <summary>
		/// 実行中のプロセスを終了します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Close() {
			if( _hChildProcess != INVALID_HANDLE_VALUE ) {
				TerminateProcess( _hChildProcess, 0 );
				CloseHandle( _hChildProcess );
				_hChildProcess = INVALID_HANDLE_VALUE;
			}
			if( _hInputOutput != INVALID_HANDLE_VALUE ) {
				CloseHandle( _hInputOutput );
				_hInputOutput = INVALID_HANDLE_VALUE;
			}
			_line.clear();
			_status = 0;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Peek
		///
		/// <summary>
		/// 実行中のプロセスが出力する次の一行を取得します。Peek() メンバ
		/// 関数は、読み込み終了の位置は更新しません。
		/// 
		/// return value
		///     <0  error
		///     0   no data
		///     1   data retrieved
		///     2   pipe closed
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		int Peek(string_type& str) {
			char_type buffer[2048];
			DWORD nBytesRead;
			DWORD nTotalBytesAvail;
			
			str.clear();
			if(_hInputOutput == INVALID_HANDLE_VALUE || _hChildProcess == INVALID_HANDLE_VALUE) {
				return -1;
			}
			
			if( _status ) {
				str = _line;
				return _status;
			}
			
			while( PeekNamedPipe( _hInputOutput, buffer, sizeof( buffer ) - sizeof( buffer[0] ) * 2, &nBytesRead, &nTotalBytesAvail, NULL ) ) {
				char_type	*pCRLF;
				DWORD	loadSize;
				
				_status = 0;
				if( !nBytesRead ) return 0;
				
				buffer[nBytesRead/sizeof(buffer[0])] = TEXT( '\0' );
				
				if( pCRLF = _tcschr( buffer, TEXT( '\n' ) ) ) {
					loadSize = ( pCRLF - buffer + 1 ) * sizeof( buffer[0] );
					_status = 1;
				} else {
					loadSize = ( nBytesRead / sizeof( buffer[0] ) ) * sizeof( buffer[0] );
					_status = 0;
				}
				
				if( !ReadFile( _hInputOutput, buffer, loadSize, &nBytesRead, NULL ) || nBytesRead != loadSize )
					return -2;
				
				if( _status ) {
					buffer[nBytesRead/sizeof(buffer[0])] = TEXT( '\0' );
					_line += buffer;
					if( _line.size() > 1 && _line[_line.size()-2] == TEXT( '\r' ) )
						_line = _line.substr( 0, _line.size() - 2 );
					else
						_line = _line.substr( 0, _line.size() - 1 );
					str = _line;
					return 1;
				}
				buffer[nBytesRead/sizeof(buffer[0])] = TEXT( '\0' );
				_line += buffer;
			}
			
			if( GetLastError() == ERROR_BROKEN_PIPE ) {
				str = _line;
				_status = 2;
				return 2;
			}
			
			return -3;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// ReadLine
		///
		/// <summary>
		/// 実行中のプロセスが出力する次の一行を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		int ReadLine(string_type& str) {
			int		res;
			
			res = this->Peek( str );
			if( res == 1 ) {
				_line.clear();
				_status = 0;
			}
			return res;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Suspend
		///
		/// <summary>
		/// 実行中のプロセスを一時停止します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Suspend() {
			if( _hChildProcess == INVALID_HANDLE_VALUE )
				return;
			
			HANDLE		hThs = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );
			if( _hChildProcess == (HANDLE)-1 )
				return;
			
			THREADENTRY32		thentry;
			
			thentry.dwSize = sizeof( thentry );
			if( Thread32First( hThs, &thentry ) ) {
				do {
					if( thentry.th32OwnerProcessID == _idChildProcess ) {
						HANDLE		hThread = OpenThread( THREAD_SUSPEND_RESUME, FALSE, thentry.th32ThreadID );
						if( hThread ) {
							SuspendThread( hThread );
							CloseHandle( hThread );
						}
					}
				} while( Thread32Next( hThs, &thentry ) );
			}
			
			CloseHandle( hThs );
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Resume
		///
		/// <summary>
		/// 停止させたプロセスを復帰させます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Resume() {
			if( _hChildProcess == INVALID_HANDLE_VALUE )
				return;
			
			HANDLE		hThs = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );
			if( _hChildProcess == (HANDLE)-1 )
				return;
			
			THREADENTRY32		thentry;
			
			thentry.dwSize = sizeof( thentry );
			if( Thread32First( hThs, &thentry ) ) {
				do {
					if( thentry.th32OwnerProcessID == _idChildProcess ) {
						HANDLE		hThread = OpenThread( THREAD_SUSPEND_RESUME, FALSE, thentry.th32ThreadID );
						if( hThread ) {
							ResumeThread( hThread );
							CloseHandle( hThread );
						}
					}
				} while( Thread32Next( hThs, &thentry ) );
			}
			
			CloseHandle( hThs );
		}
		
	private:
		/* ----------------------------------------------------------------- */
		///
		/// OpenExec
		///
		/// <summary>
		/// プロセスを実行し、標準出力を読み込み可能な形に初期化します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		bool OpenExec(const char_type* command) {
			HANDLE					hReadPipe, hWritePipe;
			SECURITY_ATTRIBUTES		sa;
			STARTUPINFO				si;
			PROCESS_INFORMATION		pi;
			
			if( _hChildProcess != INVALID_HANDLE_VALUE || _hInputOutput != INVALID_HANDLE_VALUE ) {
				return false;
			}
			
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
			
			si.hStdOutput = hWritePipe;
			if( !DuplicateHandle( GetCurrentProcess(), hReadPipe, GetCurrentProcess(), &_hInputOutput, 0, FALSE, DUPLICATE_SAME_ACCESS ) ) {
				CloseHandle( hReadPipe );
				CloseHandle( hWritePipe );
				return false;
			}
			CloseHandle( hReadPipe );
			
			char_type	*cmdtmp = new char_type[(_tcslen(command)+5)*sizeof(command[0])];
			if( !cmdtmp ) {
				CloseHandle( hWritePipe );
				return false;
			}
			_tcscpy( cmdtmp, command );
			
			BOOL	bcp = CreateProcess( NULL, cmdtmp, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi );
			
			delete[] cmdtmp;
			
			CloseHandle( hWritePipe );
			
			if( !bcp ) return false;
			
			_hChildProcess = pi.hProcess;
			_idChildProcess = pi.dwProcessId;
			
			CloseHandle( pi.hThread );
			
			return true;
		}
		
	private:
		HANDLE _hInputOutput;
		HANDLE _hChildProcess;
		DWORD _idChildProcess;
		string_type _line;
		int _status;
	}; // class ProcessStream
} // namespace CubeICE

#endif // CUBEICE_PROCESS_STREAM_H
