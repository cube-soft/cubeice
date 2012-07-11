/* ------------------------------------------------------------------------- */
/*
 *  cubeice/encoding.h
 *
 *  Copyright (c) 2010 CubeSoft Inc.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_ENCODING_H
#define CUBEICE_ENCODING_H

#include <cubeice/config.h>
#include <string>
#include <vector>

namespace CubeICE {
	namespace Detail {
		/* ----------------------------------------------------------------- */
		/*
		 *  encode
		 *
		 *  指定したコードページで MultiByteToWideChar を実行する。
		 *  http://msdn.microsoft.com/ja-jp/library/cc448053.aspx
		 */
		/* ----------------------------------------------------------------- */
		inline std::basic_string<wchar_t> Encode(const std::basic_string<char>& src, unsigned int code) {
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
		inline std::basic_string<char> Encode(const std::basic_string<wchar_t>& src, unsigned int code) {
			int flags = (code == CP_ACP) ? WC_NO_BEST_FIT_CHARS : 0;
			const int size = ::WideCharToMultiByte(code, flags, src.c_str(),  (src.size() + 1) * sizeof(wchar_t), NULL, 0, NULL, NULL);
			std::vector<char> buffer(size, 0);
			::WideCharToMultiByte(code, 0, src.c_str(), (src.size() + 1) * sizeof(wchar_t), reinterpret_cast<char*>(&buffer.at(0)), size, NULL, NULL);
			return std::basic_string<char>(reinterpret_cast<char*>(&buffer.at(0)));
		}
	} // namespace detail
	
	/* --------------------------------------------------------------------- */
	/*
	 *  sjis_to_unicode
	 *
	 *  Shift_JIS (CP932) から Unicode (UTF-16) へ変換する。
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<wchar_t> SjisToUnicode(const std::basic_string<char>& src) {
		return Detail::Encode(src, CP_ACP);
	}
	
	/* --------------------------------------------------------------------- */
	/*
	 *  unicode_to_sjis
	 *
	 *  Unicode (UTF-16) から Shift_JIS (CP932) へ変換する。
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<char> UnicodeToSjis(const std::basic_string<wchar_t>& src) {
		return Detail::Encode(src, CP_ACP);
	}
	
	/* --------------------------------------------------------------------- */
	/*
	 *  utf8_to_unicode
	 *
	 *  UTF-8 から Unicode (UTF-16) へ変換する。
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<wchar_t> Utf8ToUnicode(const std::basic_string<char>& src) {
		return Detail::Encode(src, CP_UTF8);
	}
	
	/* --------------------------------------------------------------------- */
	/*
	 *  unicode_to_utf8
	 *
	 *  Unicode (UTF-16) から UTF-8 へ変換する。
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<char> UnicodeToUtf8(const std::basic_string<wchar_t>& src) {
		return Detail::Encode(src, CP_UTF8);
	}
	
	/* --------------------------------------------------------------------- */
	/*
	 *  sjis_to_utf8
	 *
	 *  Shift_JIS (CP932) から UTF-8 へ変換する。
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<char> SjisToUtf8(const std::basic_string<char>& src) {
		return UnicodeToUtf8(SjisToUnicode(src));
	}
	
	/* --------------------------------------------------------------------- */
	/*
	 *  utf8_to_sjis
	 *
	 *  UTF-8 から Shift_JIS (CP932) へ変換する。
	 */
	/* --------------------------------------------------------------------- */
	inline std::basic_string<char> Utf8ToSjis(const std::basic_string<char>& src) {
		return UnicodeToSjis(Utf8ToUnicode(src));
	}
} // namespace CubeICE

#endif // CUBEICE_ENCODE_H
