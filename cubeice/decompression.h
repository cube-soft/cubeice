/* ------------------------------------------------------------------------- */
/*
 *  decompression.h
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
#ifndef CUBEICE_DECOMPRESSION_H
#define CUBEICE_DECOMPRESSION_H

#include <string>
#include <windows.h>
#include <tchar.h>
#include <iterator>

/* ------------------------------------------------------------------------- */
/*
 *  NOTE: archiver.h の cubeice::archiver クラスのリファクタリングのために
 *  テスト and/or 分離可能なメンバ関数を cubeice::compression もしくは
 *  cubeice::decompression 名前空間以下に分離し，テストコードを作成して
 *  いく．
 */
/* ------------------------------------------------------------------------- */
namespace cubeice {
	namespace decompression {
	} // namespace decompression
} // namespace cubeice

#endif // CUBEICE_DECOMPRESSION_H
