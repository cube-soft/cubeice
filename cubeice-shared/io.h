/* ------------------------------------------------------------------------- */
/*
 *  io.h
 *
 *  Copyright (c) 2010 - 2011 CubeSoft Inc.
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
#ifndef CUBEICE_IO_H
#define CUBEICE_IO_H

#include <cstring>
#include <string>
#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

namespace cubeice {
	/* ----------------------------------------------------------------- */
	/*
	 *  createdir
	 *
	 *  引数に指定したディレクトリを作成する．親ディレクトリが存在しない
	 *  場合は再帰的に作成する．第2引数が true 場合，同名のファイルが
	 *  存在する時はファイルを削除後にディレクトリを作成する．
	 */
	/* ----------------------------------------------------------------- */
	inline bool createdir(const std::basic_string<TCHAR>& path, bool force = false) {
		if (::PathFileExists(path.c_str())) {
			if (::PathIsDirectory(path.c_str())) return true;
			else if (!force) return false;
			else DeleteFile(path.c_str());
		}
		
		std::basic_string<TCHAR>::size_type pos = path.find_last_of(_T('\\'));
		if (pos != std::basic_string<TCHAR>::npos) if (!createdir(path.substr(0, pos), force)) return false;
		
		return ::CreateDirectory(path.c_str(), NULL) != FALSE;
	}
	
	/* ----------------------------------------------------------------- */
	/*
	 *  removedir
	 *
	 *  引数に指定されたディレクトリを削除する．ディレクトリが空でない
	 *  場合は，再帰的に削除する．
	 */
	/* ----------------------------------------------------------------- */
	inline bool removedir(const std::basic_string<TCHAR>& path) {
		TCHAR szPathName[MAX_PATH] = {};
		_tcscpy_s(szPathName, path.c_str());
		_tcscat_s(szPathName, _T("\\*.*"));
		
		WIN32_FIND_DATA wfd;
		HANDLE handle = FindFirstFile(szPathName, &wfd);
		if(handle == INVALID_HANDLE_VALUE) return false;
		
		do {
			if (_tcscmp(wfd.cFileName, _T(".")) != 0 && _tcscmp(wfd.cFileName, _T("..")) != 0) {
				TCHAR szFileName[_MAX_PATH] = {};
				_tcscpy_s(szFileName, path.c_str());
				_tcscat_s(szFileName, _T("\\") );
				_tcscat_s(szFileName, wfd.cFileName);
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) removedir(szFileName);
				else {
					// ReadOnly 属性を外す
					DWORD dwFileAttributes;
					dwFileAttributes = GetFileAttributes(szFileName);
					dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY;
					SetFileAttributes(szFileName, dwFileAttributes);
					
					DeleteFile(szFileName);
				}
			}
		} while (FindNextFile(handle, &wfd));
		FindClose(handle);
		
		// ReadOnly 属性を外す
		DWORD dwFileAttributes;
		dwFileAttributes = GetFileAttributes(path.c_str());
		dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY;
		SetFileAttributes(path.c_str(), dwFileAttributes);
		
		return RemoveDirectory(path.c_str()) != FALSE;
	}
	
	/* ----------------------------------------------------------------- */
	/*
	 *  tmpfile
	 *
	 *  parent\<random_filename> と言うランダムな一時パスを生成する．
	 *  prefix は，ランダムなファイル名を生成する際のソルト的な役目
	 *  となる．多くの場合は，prefix に指定した最初の数文字がランダム
	 *  なファイル名の頭数文字になっている．
	 *  引数 create が true の場合は，実際に一時ファイルを作成した後
	 *  そのファイルへのパスを返す．
	 */
	/* ----------------------------------------------------------------- */
	inline std::basic_string<TCHAR> tmpfile(const std::basic_string<TCHAR>& parent, const std::basic_string<TCHAR>& prefix, bool create) {
		std::basic_string<TCHAR> dest;
		TCHAR buffer[MAX_PATH] = {};
		if (::GetTempFileName(parent.c_str(), prefix.c_str(), 0, buffer) == 0) return dest;
		dest = buffer;
		
		// 一時ファイルが生成されているので削除する．
		if (!create && PathFileExists(dest.c_str())) DeleteFile(dest.c_str());
		return dest;
	}
	
	/* ----------------------------------------------------------------- */
	/*
	 *  tmpfile
	 *
	 *  ランダムな一時パスを生成する．ディレクトリには環境変数 tmp
	 *  に指定されているパスが使用される．
	 */
	/* ----------------------------------------------------------------- */
	inline std::basic_string<TCHAR> tmpfile(const std::basic_string<TCHAR>& prefix, bool create) {
		TCHAR dir[MAX_PATH] = {};
		if (::GetTempPath(MAX_PATH, dir) == 0) return std::basic_string<TCHAR>();
		return cubeice::tmpfile(std::basic_string<TCHAR>(dir), prefix, create);
	}
	
	/* ----------------------------------------------------------------- */
	/*
	 *  tmpdir
	 *
	 *  ディレクトリ parent 下にラインダムな一時ディレクトリを生成する．
	 */
	/* ----------------------------------------------------------------- */
	inline std::basic_string<TCHAR> tmpdir(const std::basic_string<TCHAR>& parent, const std::basic_string<TCHAR>& prefix) {
		std::basic_string<TCHAR> path = cubeice::tmpfile(parent, prefix, false);
		if (path.empty() || !cubeice::createdir(path, true)) return std::basic_string<TCHAR>();
		return path;
	}
	
	/* ----------------------------------------------------------------- */
	/*
	 *  tmpdir
	 *
	 *  環境変数 tmp に指定されているディレクトリ下にラインダムな
	 *  一時ディレクトリを生成する．
	 */
	/* ----------------------------------------------------------------- */
	inline std::basic_string<TCHAR> tmpdir(const std::basic_string<TCHAR>& prefix) {
		std::basic_string<TCHAR> path = cubeice::tmpfile(prefix, false);
		if (path.empty() || !cubeice::createdir(path, true)) return std::basic_string<TCHAR>();
		return path;
	}
} // namespace cubeice

#endif // CUBEICE_IO_H
