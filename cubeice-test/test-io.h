/* ------------------------------------------------------------------------- */
/*
 *  test-io.h
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
#ifndef CUBEICE_TEST_IO_H
#define CUBEICE_TEST_IO_H

#include <windows.h>
#include <tchar.h>
#include <string>
#include "io.h"
#include <boost/test/minimal.hpp>

/* ------------------------------------------------------------------------- */
/*
 *  test_directory
 *
 *  ディレクトリ作成・削除のテスト．
 */
/* ------------------------------------------------------------------------- */
inline void test_directory() {
	TCHAR buffer[MAX_PATH] = {};
	BOOST_CHECK(GetCurrentDirectory(sizeof(buffer) / sizeof(TCHAR), buffer) > 0);
	
	std::basic_string<TCHAR> root = buffer;
	std::basic_string<TCHAR> dir;
	
	// TestCase1: 通常ケース
	// カレントパス下に空のディレクトリを作成し，それを削除する．
	dir = root + _T("\\cubeice-test-createdir");
	BOOST_CHECK(cubeice::createdir(dir));
	BOOST_CHECK(PathIsDirectory(dir.c_str()));
	BOOST_CHECK(cubeice::removedir(dir));
	
	// TestCase2: 再帰的にディレクトリを作成し，削除する．
	dir = root + _T("\\cubeice-test-createdir");
	std::basic_string<TCHAR> subdir = dir + _T("\\subdir");
	BOOST_CHECK(cubeice::createdir(subdir));
	BOOST_CHECK(PathIsDirectory(dir.c_str()));
	BOOST_CHECK(PathIsDirectory(subdir.c_str()));
	BOOST_CHECK(cubeice::removedir(dir));
	
}

/* ------------------------------------------------------------------------- */
/*
 *  test_tmpfile
 *
 *  一時パス取得のテスト．
 */
/* ------------------------------------------------------------------------- */
inline void test_tmpfile() {
	TCHAR buffer[MAX_PATH] = {};
	BOOST_CHECK(GetCurrentDirectory(sizeof(buffer) / sizeof(TCHAR), buffer) > 0);

	std::basic_string<TCHAR> root = buffer;
	std::basic_string<TCHAR> prefix = _T("cubeice");
	std::basic_string<TCHAR> path;

	// TestCase1: 親ディレクトリを指定する．一時ファイルは実際には作成されない．
	path = cubeice::tmpfile(root, prefix, false);
	BOOST_CHECK(!path.empty());
	BOOST_CHECK(!::PathFileExists(path.c_str())); // tmpfile() 関数は実際にファイルは作成しない．

	// TestCase2: 親ディレクトリを指定する．一時ファイルが実際に作成される．
	path = cubeice::tmpfile(root, prefix, true);
	BOOST_CHECK(!path.empty());
	BOOST_CHECK(::PathFileExists(path.c_str()));
	BOOST_CHECK(::DeleteFile(path.c_str()) != FALSE);
	BOOST_CHECK(!::PathFileExists(path.c_str()));

	// TestCase3: 親ディレクトリを指定しない．一時ファイルは実際には作成されない．
	path = cubeice::tmpfile(prefix, false);
	BOOST_CHECK(!path.empty());
	BOOST_CHECK(!::PathFileExists(path.c_str()));

	// TestCase4: 親ディレクトリを指定しない．一時ファイルが実際に作成される．
	path = cubeice::tmpfile(prefix, true);
	BOOST_CHECK(!path.empty());
	BOOST_CHECK(::PathFileExists(path.c_str()));
	BOOST_CHECK(::DeleteFile(path.c_str()) != FALSE);
	BOOST_CHECK(!::PathFileExists(path.c_str()));
}

/* ------------------------------------------------------------------------- */
/*
 *  test_tmpdir
 *
 *  一時ディレクトリ取得のテスト．
 */
/* ------------------------------------------------------------------------- */
inline void test_tmpdir() {
	TCHAR buffer[MAX_PATH] = {};
	BOOST_CHECK(GetCurrentDirectory(sizeof(buffer) / sizeof(TCHAR), buffer) > 0);

	std::basic_string<TCHAR> root = buffer;
	std::basic_string<TCHAR> prefix = _T("cubeice");
	std::basic_string<TCHAR> path;

	// TestCase1: 親ディレクトリを指定する．
	path = cubeice::tmpdir(root, prefix);
	BOOST_CHECK(!path.empty());
	BOOST_CHECK(::PathIsDirectory(path.c_str()));
	BOOST_CHECK(cubeice::removedir(path));
	BOOST_CHECK(!::PathIsDirectory(path.c_str()));

	// TestCase2: 親ディレクトリを指定しない．
	path = cubeice::tmpdir(prefix);
	BOOST_CHECK(!path.empty());
	BOOST_CHECK(::PathIsDirectory(path.c_str()));
	BOOST_CHECK(cubeice::removedir(path));
	BOOST_CHECK(!::PathIsDirectory(path.c_str()));
}

#endif // CUBEICE_TEST_IO_H
