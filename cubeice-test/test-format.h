/* ------------------------------------------------------------------------- */
/*
 *  test-format.h
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
#ifndef CUBEICE_TEST_FORMAT_H
#define CUBEICE_TEST_FORMAT_H

#include <windows.h>
#include <tchar.h>
#include <string>
#include "format.h"
#include <boost/test/minimal.hpp>

/* ------------------------------------------------------------------------- */
/*
 *  test_punct
 *
 *  3桁毎にカンマを挿入する関数のテスト．
 */
/* ------------------------------------------------------------------------- */
inline void test_punct() {
	BOOST_CHECK(cubeice::punct(0) == _T("0"));
	BOOST_CHECK(cubeice::punct(1) ==  _T("1"));
	BOOST_CHECK(cubeice::punct(12) == _T("12"));
	BOOST_CHECK(cubeice::punct(123) == _T("123"));
	BOOST_CHECK(cubeice::punct(1234) == _T("1,234"));
	BOOST_CHECK(cubeice::punct(12345) == _T("12,345"));
	BOOST_CHECK(cubeice::punct(123456) == _T("123,456"));
	BOOST_CHECK(cubeice::punct(1234567) == _T("1,234,567"));
	BOOST_CHECK(cubeice::punct(12345678) == _T("12,345,678"));
	BOOST_CHECK(cubeice::punct(123456789) == _T("123,456,789"));
	BOOST_CHECK(cubeice::punct(1234567890) == _T("1,234,567,890"));
	BOOST_CHECK(cubeice::punct(-1) == _T("-1"));
	BOOST_CHECK(cubeice::punct(-12) == _T("-12"));
	BOOST_CHECK(cubeice::punct(-123) == _T("-123"));
	BOOST_CHECK(cubeice::punct(-1234) == _T("-1,234"));
	BOOST_CHECK(cubeice::punct(-12345) == _T("-12,345"));
	BOOST_CHECK(cubeice::punct(-123456) == _T("-123,456"));
	BOOST_CHECK(cubeice::punct(-1234567) == _T("-1,234,567"));
	BOOST_CHECK(cubeice::punct(-12345678) == _T("-12,345,678"));
	BOOST_CHECK(cubeice::punct(-123456789) == _T("-123,456,789"));
	BOOST_CHECK(cubeice::punct(-1234567890) == _T("-1,234,567,890"));

	__int64 value = 123456789123456789;
	BOOST_CHECK(cubeice::punct(value) == _T("123,456,789,123,456,789"));
	value = 1234567891234567890;
	BOOST_CHECK(cubeice::punct(value) == _T("1,234,567,891,234,567,890"));
	value = -123456789123456789;
	BOOST_CHECK(cubeice::punct(value) == _T("-123,456,789,123,456,789"));
	value = -1234567891234567890;
	BOOST_CHECK(cubeice::punct(value) == _T("-1,234,567,891,234,567,890"));
}

#endif // CUBEICE_TEST_FORMAT_H
