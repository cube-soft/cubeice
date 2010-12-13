/* ------------------------------------------------------------------------- */
/*
 *  cubeice.h
 *
 *  Copyright (c) 2010 CubeSoft.
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
 *  Last-modified: Thu 18 Nov 2010 14:40:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBE_ICE_H
#define CUBE_ICE_H

#include <string>
#include <vector>
#include <tchar.h>

/* ------------------------------------------------------------------------- */
/*
 *  CLX C++ Libraries can be downloaded from the following URL:
 *  http://clx.cielquis.net/
 */
/* ------------------------------------------------------------------------- */
#ifndef CLX_USE_WCHAR
#define CLX_USE_WCHAR
#endif
#include <clx/config.h>
#include <clx/tokenizer.h>

#include "dialog.h"
#include "archiver.h"

HINSTANCE AppInstance; // アプリケーションのハンドル

namespace cube {
	namespace cmdline {
		typedef clx::escape_separator<TCHAR> separator;
		typedef clx::basic_tokenizer<clx::escape_separator<TCHAR>, std::basic_string<TCHAR> > splitter;
	}
}

#endif // CUBE_ICE_H