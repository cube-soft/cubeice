/* ------------------------------------------------------------------------- */
/*
 *  pathmatch.h
 *
 *  Copyright (c) 2004 - 2010, clown. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    - No names of its contributors may be used to endorse or promote
 *      products derived from this software without specific prior written
 *      permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  Last-modified: Tue 16 Mar 2010 17:06:03 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_PATHMATCH_H
#define CUBEICE_PATHMATCH_H

#include <algorithm>
#include <iostream>
#include <string>
#include <tchar.h>

namespace cubeice {
	/* --------------------------------------------------------------------- */
	//  match
	/* --------------------------------------------------------------------- */
	template <class InIter>
	bool match(InIter first1, InIter last1, InIter first2, InIter last2) {
		if (first2 == last2) return first1 == last1;
		
		switch (*first2) {
			case _T('*'):
			{
				InIter pos = first2;
				return match(first1, last1, ++first2, last2) ||
					(first1 != last1 && match(++first1, last1, pos, last2));
			}
			case _T('?'):
				return (first1 != last1) && match(++first1, last1, ++first2, last2);
			default:
				break;
		}
		
		return first1 != last1 && *first1 == *first2 && match(++first1, last1, ++first2, last2);
	}
	
	/* --------------------------------------------------------------------- */
	//  match
	/* --------------------------------------------------------------------- */
	template <class Ch, class Tr>
	bool match(const std::basic_string<Ch, Tr>& src, const std::basic_string<Ch, Tr>& wildcard) {
		return match(src.begin(), src.end(), wildcard.begin(), wildcard.end());
	}
	
	/* --------------------------------------------------------------------- */
	//  match
	/* --------------------------------------------------------------------- */
	template <class CharT>
	bool match(const CharT* src, const CharT* wildcard) {
		std::basic_string<CharT> tmp_src(src);
		std::basic_string<CharT> tmp_wildcard(wildcard);
		return match(tmp_src, tmp_wildcard);
	}
	
	/* --------------------------------------------------------------------- */
	//  pathmatch
	/* --------------------------------------------------------------------- */
	template <class Ch, class Tr>
	bool pathmatch(const std::basic_string<Ch, Tr>& src, const std::basic_string<Ch, Tr>& wildcard) {
		static const Ch separator = _T('\\');
		static const Ch extension = _T('.');
		static const Ch asterisk = _T('*');
		
		if (src.empty() || wildcard.empty()) return false;
		
		// 対象パスの整形
		std::basic_string<Ch, Tr> tmp_src;
		if (*src.begin() != separator) tmp_src += separator;
		tmp_src += src;
		if (*src.rbegin() != separator) tmp_src += separator;
		
		// ワイルドカードの整形
		std::basic_string<Ch, Tr> tmp_wildcard;
		
		typename std::basic_string<Ch, Tr>::const_iterator pos = wildcard.begin();
		if (!(*pos == asterisk && ++pos != wildcard.end() && *pos == separator)) {
			tmp_wildcard += asterisk;
			tmp_wildcard += separator;
		}
		
		tmp_wildcard += wildcard;
		
		typename std::basic_string<Ch, Tr>::const_reverse_iterator rpos = wildcard.rbegin();
		if (*rpos != asterisk && ++rpos != wildcard.rend() && *rpos != separator) {
			tmp_wildcard += separator;
			tmp_wildcard += asterisk;
		}
		
		return match(tmp_src, tmp_wildcard);
	}
	
	/* --------------------------------------------------------------------- */
	//  pathmatch
	/* --------------------------------------------------------------------- */
	template <class CharT>
	bool pathmatch(const CharT* src, const CharT* wildcard) {
		std::basic_string<CharT> tmp_src(src);
		std::basic_string<CharT> tmp_wildcard(wildcard);
		return pathmatch(tmp_src, tmp_wildcard);
	}
}

#endif // CUBEICE_PATHMATCH_H
