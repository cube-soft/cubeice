/* ------------------------------------------------------------------------- */
/*
 *  encode.h
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
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_ENCODE_H
#define CUBEICE_ENCODE_H

#include <string>
#include <vector>
#include <windows.h>

namespace cubeice {
	namespace detail {
		/* ----------------------------------------------------------------- */
		/*
		 *  encode
		 *
		 *  指定したコードページで MultiByteToWideChar を実行する。
		 *  http://msdn.microsoft.com/ja-jp/library/cc448053.aspx
		 */
		/* ----------------------------------------------------------------- */
		inline std::basic_string<wchar_t> encode(const std::basic_string<char>& src, unsigned int code) {
			const int size = ::MultiByteToWideChar(code, 0, src.c_str(), (src.size() + 1) * sizeof(char), NULL, 0);
			std::vector<wchar_t> buffer(size, 0);
			::MultiByteToWideChar(code, 0, src.c_str(), (src.size() + 1) * sizeof(char), reinterpret_cast<wchar_t*>(&buffer.at(0)), size);
			return std::basic_string<wchar_t>(reinterpret_cast<wchar_t*>(&buffer.at(0)));
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  encode
		 *
		 *  指定したコードページで WideCharToMultiByte を実行する。
		 *  dwFlags には WC_NO_BEST_FIT_CHARS を指定する。
		 *  http://msdn.microsoft.com/ja-jp/library/cc448089.aspx
		 */
		/* ----------------------------------------------------------------- */
		inline std::basic_string<char> encode(const std::basic_string<wchar_t>& src, unsigned int code) {
			int flags = (code == CP_ACP) ? WC_NO_BEST_FIT_CHARS : 0;
			const int size = ::WideCharToMultiByte(code, flags, src.c_str(),  (src.size() + 1) * sizeof(wchar_t), NULL, 0, NULL, NULL);
			std::vector<char> buffer(size, 0);
			::WideCharToMultiByte(code, 0, src.c_str(), (src.size() + 1) * sizeof(wchar_t), reinterpret_cast<char*>(&buffer.at(0)), size, NULL, NULL);
			return std::basic_string<char>(reinterpret_cast<char*>(&buffer.at(0)));
		}
	}
	
	/* --------------------------------------------------------------------- */
	/*
	 *  sjis_to_unicode
	 *
	 *  Shift_JIS (CP932) から Unicode (UTF-16) へ変換する。
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<wchar_t> sjis_to_unicode(const std::basic_string<char>& src) {
		return detail::encode(src, CP_ACP);
	}
	
	/* --------------------------------------------------------------------- */
	/*
	 *  unicode_to_sjis
	 *
	 *  Unicode (UTF-16) から Shift_JIS (CP932) へ変換する。
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<char> unicode_to_sjis(const std::basic_string<wchar_t>& src) {
		return detail::encode(src, CP_ACP);
	}
	
	/* --------------------------------------------------------------------- */
	/*
	 *  utf8_to_unicode
	 *
	 *  UTF-8 から Unicode (UTF-16) へ変換する。
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<wchar_t> utf8_to_unicode(const std::basic_string<char>& src) {
		return detail::encode(src, CP_UTF8);
	}
	
	/* --------------------------------------------------------------------- */
	/*
	 *  unicode_to_utf8
	 *
	 *  Unicode (UTF-16) から UTF-8 へ変換する。
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<char> unicode_to_utf8(const std::basic_string<wchar_t>& src) {
		return detail::encode(src, CP_UTF8);
	}
	
	/* --------------------------------------------------------------------- */
	/*
	 *  sjis_to_utf8
	 *
	 *  Shift_JIS (CP932) から UTF-8 へ変換する。
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<char> sjis_to_utf8(const std::basic_string<char>& src) {
		return unicode_to_utf8(sjis_to_unicode(src));
	}
	
	/* --------------------------------------------------------------------- */
	/*
	 *  utf8_to_sjis
	 *
	 *  UTF-8 から Shift_JIS (CP932) へ変換する。
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<char> utf8_to_sjis(const std::basic_string<char>& src) {
		return unicode_to_sjis(utf8_to_unicode(src));
	}
}

#endif // CUBEICE_ENCODE_H
