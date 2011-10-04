/* ------------------------------------------------------------------------- */
/*
 *  test-compressor.h
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
#ifndef CUBEICE_TEST_COMPRESSOR_H
#define CUBEICE_TEST_COMPRESSOR_H

#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include "compressor.h"
#include <boost/test/minimal.hpp>

/* ------------------------------------------------------------------------- */
/*
 *  test_compressor_extension
 *
 *  ファイル種類と入力ファイル群から拡張子，および TAR 処理が必要かどうか
 *  を判定する関数のテスト．
 */
/* ------------------------------------------------------------------------- */
void test_compressor_extension() {
	std::vector<std::basic_string<TCHAR> > v;
	std::basic_string<TCHAR> filetype;
	bool optar;
	
	// TestCase1: TAR 関連以外のファイル群
	filetype = _T("7z");
	optar = false;
	v.clear();
	v.push_back(_T("foo.txt"));
	v.push_back(_T("bar\\bas.txt"));
	v.push_back(_T("テストファイル.dat"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".7z"));
	BOOST_CHECK(optar == false);
	
	// TestCase2: 単一ファイルを gzip に圧縮
	filetype = _T("gzip");
	optar = false;
	v.clear();
	v.push_back(_T("テストファイル.txt"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".gz"));
	BOOST_CHECK(optar == false);
	
	// TestCase3: 単一ファイルを tgz に圧縮
	filetype = _T("tgz");
	optar = false;
	v.clear();
	v.push_back(_T("テストファイル.txt"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tgz"));
	BOOST_CHECK(optar == true);
	
	// TestCase4: 単一ファイルを bzip2 に圧縮 (*.xxx.bz2 と言う拡張子になる)
	filetype = _T("bzip2");
	optar = false;
	v.clear();
	v.push_back(_T("テストファイル.txt"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".txt.bz2"));
	BOOST_CHECK(optar == false);

	// TestCase5: 単一ファイルを tbz に圧縮
	filetype = _T("tbz");
	optar = false;
	v.clear();
	v.push_back(_T("テストファイル.txt"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tbz"));
	BOOST_CHECK(optar == true);

	
	// TestCase6: 複数ファイル (ディレクトリ含まず) を gzip に圧縮
	filetype = _T("gzip");
	optar = false;
	v.clear();
	v.push_back(_T("foo.txt"));
	v.push_back(_T("bar.txt"));
	v.push_back(_T("テストファイル.dat"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tar.gz"));
	BOOST_CHECK(optar == true);
	
	// TestCase7: 複数ファイル (ディレクトリ含まず) を tgz に圧縮
	filetype = _T("tgz");
	optar = false;
	v.clear();
	v.push_back(_T("foo.txt"));
	v.push_back(_T("bar.txt"));
	v.push_back(_T("テストファイル.dat"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tgz"));
	BOOST_CHECK(optar == true);
	
	// TestCase8: 複数ファイル (ディレクトリ含まず) を bzip2 に圧縮
	filetype = _T("bzip2");
	optar = false;
	v.clear();
	v.push_back(_T("foo.txt"));
	v.push_back(_T("bar.txt"));
	v.push_back(_T("テストファイル.dat"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tar.bz2"));
	BOOST_CHECK(optar == true);
	
	// TestCase9: 複数ファイル (ディレクトリ含まず) を tbz に圧縮
	filetype = _T("tbz");
	optar = false;
	v.clear();
	v.push_back(_T("foo.txt"));
	v.push_back(_T("bar.txt"));
	v.push_back(_T("テストファイル.dat"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tbz"));
	BOOST_CHECK(optar == true);
	
	// TestCase10: 複数ファイル (ファイル + ディレクトリ) を gzip に圧縮
	filetype = _T("gzip");
	optar = false;
	v.clear();
	v.push_back(_T("foo.txt"));
	v.push_back(_T("bar"));
	v.push_back(_T("bar\\bas.txt"));
	v.push_back(_T("テストファイル.dat"));
	v.push_back(_T("テストディレクトリ"));
	v.push_back(_T("テストディレクトリ\\ファイル1.mp3"));
	v.push_back(_T("テストディレクトリ\\ファイル2.wmv"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tar.gz"));
	BOOST_CHECK(optar == true);
	
	// TestCase11: 複数ファイル (ファイル + ディレクトリ) を tgz に圧縮
	filetype = _T("tgz");
	optar = false;
	v.clear();
	v.push_back(_T("foo.txt"));
	v.push_back(_T("bar"));
	v.push_back(_T("bar\\bas.txt"));
	v.push_back(_T("テストファイル.dat"));
	v.push_back(_T("テストディレクトリ"));
	v.push_back(_T("テストディレクトリ\\ファイル1.mp3"));
	v.push_back(_T("テストディレクトリ\\ファイル2.wmv"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tgz"));
	BOOST_CHECK(optar == true);
	
	// TestCase12: 複数ファイル (ファイル + ディレクトリ) を bzip2 に圧縮
	filetype = _T("bzip2");
	optar = false;
	v.clear();
	v.push_back(_T("foo.txt"));
	v.push_back(_T("bar"));
	v.push_back(_T("bar\\bas.txt"));
	v.push_back(_T("テストファイル.dat"));
	v.push_back(_T("テストディレクトリ"));
	v.push_back(_T("テストディレクトリ\\ファイル1.mp3"));
	v.push_back(_T("テストディレクトリ\\ファイル2.wmv"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tar.bz2"));
	BOOST_CHECK(optar == true);
	
	// TestCase13: 複数ファイル (ファイル + ディレクトリ) を tbz に圧縮
	filetype = _T("tbz");
	optar = false;
	v.clear();
	v.push_back(_T("foo.txt"));
	v.push_back(_T("bar"));
	v.push_back(_T("bar\\bas.txt"));
	v.push_back(_T("テストファイル.dat"));
	v.push_back(_T("テストディレクトリ"));
	v.push_back(_T("テストディレクトリ\\ファイル1.mp3"));
	v.push_back(_T("テストディレクトリ\\ファイル2.wmv"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tbz"));
	BOOST_CHECK(optar == true);
	
	// TestCase14: 単一の tar ファイルを gzip に圧縮
	filetype = _T("gzip");
	optar = false;
	v.clear();
	v.push_back(_T("テストファイル.tar"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tar.gz"));
	BOOST_CHECK(optar == false);
	
	// TestCase15: 単一の tar ファイルを tgz に圧縮
	filetype = _T("tgz");
	optar = false;
	v.clear();
	v.push_back(_T("テストファイル.tar"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tgz"));
	BOOST_CHECK(optar == false);
	
	// TestCase16: 単一の tar ファイルを bzip2 に圧縮
	filetype = _T("bzip2");
	optar = false;
	v.clear();
	v.push_back(_T("テストファイル.tar"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tar.bz2"));
	BOOST_CHECK(optar == false);
	
	// TestCase17: 単一の tar ファイルを tbz に圧縮
	filetype = _T("tbz");
	optar = false;
	v.clear();
	v.push_back(_T("テストファイル.tar"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tbz"));
	BOOST_CHECK(optar == false);
	
	// TestCase18: 複数の tar ファイルを gzip に圧縮
	filetype = _T("gzip");
	optar = false;
	v.clear();
	v.push_back(_T("foo.tar"));
	v.push_back(_T("bar.tar"));
	v.push_back(_T("テストファイル1.tar"));
	v.push_back(_T("テストファイル2.tar"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tar.gz"));
	BOOST_CHECK(optar == true);
	
	// TestCase19: 複数の tar ファイルを tgz に圧縮
	filetype = _T("tgz");
	optar = false;
	v.clear();
	v.push_back(_T("foo.tar"));
	v.push_back(_T("bar.tar"));
	v.push_back(_T("テストファイル1.tar"));
	v.push_back(_T("テストファイル2.tar"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tgz"));
	BOOST_CHECK(optar == true);
	
	// TestCase20: 複数の tar ファイルを bzip2 に圧縮
	filetype = _T("bzip2");
	optar = false;
	v.clear();
	v.push_back(_T("foo.tar"));
	v.push_back(_T("bar.tar"));
	v.push_back(_T("テストファイル1.tar"));
	v.push_back(_T("テストファイル2.tar"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tar.bz2"));
	BOOST_CHECK(optar == true);
	
	// TestCase21: 複数の tar ファイルを tbz に圧縮
	filetype = _T("tbz");
	optar = false;
	v.clear();
	v.push_back(_T("foo.tar"));
	v.push_back(_T("bar.tar"));
	v.push_back(_T("テストファイル1.tar"));
	v.push_back(_T("テストファイル2.tar"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tbz"));
	BOOST_CHECK(optar == true);
	
	// TestCase22: tar ファイルを含む複数のファイルを gzip に圧縮
	filetype = _T("gzip");
	optar = false;
	v.clear();
	v.push_back(_T("hoge.tar"));
	v.push_back(_T("fuga.tar"));
	v.push_back(_T("foo.txt"));
	v.push_back(_T("bar"));
	v.push_back(_T("bar\\bas.txt"));
	v.push_back(_T("テストファイル.dat"));
	v.push_back(_T("テストディレクトリ"));
	v.push_back(_T("テストディレクトリ\\ファイル1.mp3"));
	v.push_back(_T("テストディレクトリ\\ファイル2.wmv"));
	v.push_back(_T("テストディレクトリ\\テストアーカイブ1.tar"));
	v.push_back(_T("テストディレクトリ\\テストアーカイブ2.tar"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tar.gz"));
	BOOST_CHECK(optar == true);
	
	// TestCase23: tar ファイルを含む複数のファイルを tgz に圧縮
	filetype = _T("tgz");
	optar = false;
	v.clear();
	v.push_back(_T("hoge.tar"));
	v.push_back(_T("fuga.tar"));
	v.push_back(_T("foo.txt"));
	v.push_back(_T("bar"));
	v.push_back(_T("bar\\bas.txt"));
	v.push_back(_T("テストファイル.dat"));
	v.push_back(_T("テストディレクトリ"));
	v.push_back(_T("テストディレクトリ\\ファイル1.mp3"));
	v.push_back(_T("テストディレクトリ\\ファイル2.wmv"));
	v.push_back(_T("テストディレクトリ\\テストアーカイブ1.tar"));
	v.push_back(_T("テストディレクトリ\\テストアーカイブ2.tar"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tgz"));
	BOOST_CHECK(optar == true);
	
	// TestCase24: tar ファイルを含む複数のファイルを bzip2 に圧縮
	filetype = _T("bzip2");
	optar = false;
	v.clear();
	v.push_back(_T("hoge.tar"));
	v.push_back(_T("fuga.tar"));
	v.push_back(_T("foo.txt"));
	v.push_back(_T("bar"));
	v.push_back(_T("bar\\bas.txt"));
	v.push_back(_T("テストファイル.dat"));
	v.push_back(_T("テストディレクトリ"));
	v.push_back(_T("テストディレクトリ\\ファイル1.mp3"));
	v.push_back(_T("テストディレクトリ\\ファイル2.wmv"));
	v.push_back(_T("テストディレクトリ\\テストアーカイブ1.tar"));
	v.push_back(_T("テストディレクトリ\\テストアーカイブ2.tar"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tar.bz2"));
	BOOST_CHECK(optar == true);
	
	// TestCase25: tar ファイルを含む複数のファイルを tbz に圧縮
	filetype = _T("tbz");
	optar = false;
	v.clear();
	v.push_back(_T("hoge.tar"));
	v.push_back(_T("fuga.tar"));
	v.push_back(_T("foo.txt"));
	v.push_back(_T("bar"));
	v.push_back(_T("bar\\bas.txt"));
	v.push_back(_T("テストファイル.dat"));
	v.push_back(_T("テストディレクトリ"));
	v.push_back(_T("テストディレクトリ\\ファイル1.mp3"));
	v.push_back(_T("テストディレクトリ\\ファイル2.wmv"));
	v.push_back(_T("テストディレクトリ\\テストアーカイブ1.tar"));
	v.push_back(_T("テストディレクトリ\\テストアーカイブ2.tar"));
	BOOST_CHECK(cubeice::compressor::extension(v.begin(), v.end(), filetype, optar) == _T(".tbz"));
	BOOST_CHECK(optar == true);
}

#endif // CUBEICE_TEST_COMPRESSOR_H
