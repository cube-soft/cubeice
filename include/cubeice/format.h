/* ------------------------------------------------------------------------- */
/*
 *  format.h
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
#ifndef CUBEICE_FORMAT_H
#define CUBEICE_FORMAT_H

#include <cmath>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include <tchar.h>

namespace cubeice {
	/* ----------------------------------------------------------------- */
	/*
	 *  punct
	 *
	 *  引数に指定された整数に対して，3桁毎にカンマを挿入した文字列
	 *  を取得する．
	 */
	/* ----------------------------------------------------------------- */
	template <typename IntT>
	inline std::basic_string<TCHAR> punct(IntT src) {
		IntT number = (src < 0) ? -src : src;
		int sign = (src >= 0) ? 1 : -1;
		
		std::vector<int> v;
		while (number / 1000) {
			v.push_back(number % 1000);
			number /= 1000;
		}
		
		std::basic_stringstream<TCHAR> ss;
		ss << number * sign;
		for (std::vector<int>::reverse_iterator pos = v.rbegin(); pos != v.rend(); ++pos) {
			ss << _T(",") << std::setfill(_T('0')) << std::setw(3) << *pos;
		}
		return ss.str();
	}

	/* ----------------------------------------------------------------- */
	/*
	 *  byte_format
	 *
	 *  引数に指定された整数に対して、適切な単位 (バイト、KB、MB、...)
	 *  に揃えて出力する。数値部分は有効数字 3 桁。
	 */
	/* ----------------------------------------------------------------- */
	template <typename IntT>
	inline std::basic_string<TCHAR> byte_format(IntT src) {
		static const double digit = 1000.0;
		static const TCHAR units[][4] = { _T("バイト"), _T("KB"), _T("MB"), _T("GB"), _T("TB"), _T("PB"), _T("EB"), _T("ZB"), _T("YB") };

		double value = static_cast<double>(src);
		int n = 0;
		while (value > digit) {
			value /= digit;
			++n;
			if (n > sizeof(units) / sizeof(units[0]) - 1) break;
		}

		TCHAR dest[64] = {};
		_stprintf(dest, _T("%.3g %s"), value, units[n]);

		return std::basic_string<TCHAR>(dest);
	}

} // namespace cubeice

#endif // CUBEICE_FORMAT_H
