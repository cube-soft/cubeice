/* ------------------------------------------------------------------------- */
/*
 *  compressor.h
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
#ifndef CUBEICE_COMPRESSOR_H
#define CUBEICE_COMPRESSOR_H

#include <string>
#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <iterator>

#pragma comment(lib, "shlwapi.lib")

namespace cubeice {
	/* ------------------------------------------------------------------------- */
	/*
	 *  compressor
	 *
	 *  NOTE: archiver.h の cubeice::archiver クラスのリファクタリングのために
	 *  テスト and 分離可能なメンバ関数を cubeice::compressor もしくは
	 *  cubeice::decompressor クラス以下に分離し，テストコードを作成して
	 *  いく．最終的には，圧縮処理に関わるものは全て compressor クラスに
	 *  移行する．
	 */
	/* ------------------------------------------------------------------------- */
	class compressor {
	public:
		/* ----------------------------------------------------------------- */
		/*
		 *  extension
		 *
		 *  ユーザから指定されたファイル種類と圧縮するファイル群から，
		 *  生成される圧縮ファイルの最終的な拡張子を導出する．多くの場合は
		 *  filetype の文字列がそのまま拡張子となる．
		 *
		 *  TAR 関連のものが指定された場合，TAR 書庫にした上で圧縮する必要
		 *  があるかどうかも併せてチェックし，第 4引数にその結果を格納する．
		 *
		 */
		/* ----------------------------------------------------------------- */
		template <class InputIterator>
		static std::basic_string<TCHAR> extension(InputIterator first, InputIterator last, const std::basic_string<TCHAR>& filetype, bool& need_tar_operation) {
			std::basic_string<TCHAR> dest = _T(".");
			if (filetype == _T("gzip")) dest += _T("gz");
			else if (filetype == _T("bzip2")) dest += _T("bz2");
			else dest += filetype;
			
			need_tar_operation = false;
			if (filetype == _T("gzip") || filetype == _T("bzip2")) {
				int n = 0;
				bool is_tar = false;
				for (InputIterator iter = first; iter != last; ++iter) {
					++n;
					std::basic_string<TCHAR>::size_type pos = iter->find_last_of(_T('.'));
					if (pos != std::basic_string<TCHAR>::npos && iter->substr(pos) == _T(".tar")) is_tar = true;
				}
				
				if (n > 1 || ::PathIsDirectory(first->c_str())) {
					dest = _T(".tar") + dest;
					need_tar_operation = true;
				}
				else if (is_tar) dest = _T(".tar") + dest;
				else if (filetype == _T("bzip2")) {
					std::basic_string<TCHAR>::size_type pos = first->find_last_of('.');
					if (pos != std::basic_string<TCHAR>::npos) dest = first->substr(pos) + dest;
				}
			} else if (filetype == _T("tbz") || filetype == _T("tgz")) {
				std::basic_string<TCHAR>::size_type pos = first->find_last_of(_T('.'));
				if (std::distance(first, last) > 1 || ::PathIsDirectory(first->c_str()) || pos == std::basic_string<TCHAR>::npos || first->substr(pos) != _T(".tar")) {
					need_tar_operation = true;
				}
			}
			
			return dest;
		}
	};
} // namespace cubeice

#endif // CUBEICE_COMPRESSOR_H