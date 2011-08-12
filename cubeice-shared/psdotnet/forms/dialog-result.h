// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  psdotnet/forms/dialog-result.h
 *
 *  Copyright (c) 2011, clown.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#ifndef PSDOTNET_FORMS_DIALOG_RESULT_H
#define PSDOTNET_FORMS_DIALOG_RESULT_H

#include <psdotnet/typesafe-enum.h>

namespace PsdotNet {
	namespace Forms {
		/* ----------------------------------------------------------------- */
		//  DialogResult
		/* ----------------------------------------------------------------- */
		namespace Detail {
			struct DialogResult_Def {
				enum type {
					None                    = 0,
					OK                      = IDOK,
					Cancel                  = IDCANCEL,
					Abort                   = IDABORT,
					Retry                   = IDRETRY,
					Ignore                  = IDIGNORE,
					Yes                     = IDYES,
					No                      = IDNO
				};
			};
		}
		typedef TypesafeEnum<Detail::DialogResult_Def> DialogResult;
		
		namespace Detail {
			/* ------------------------------------------------------------- */
			/*
			 *  ToDialogResult
			 *
			 *  Win32 API が返した整数値を DialogResult 型に変換する．
			 */
			/* ------------------------------------------------------------- */
			inline DialogResult ToDialogResult(std::size_t x) {
				switch (x) {
					case DialogResult::OK:     return DialogResult::OK;
					case DialogResult::Cancel: return DialogResult::Cancel;
					case DialogResult::Abort:  return DialogResult::Abort;
					case DialogResult::Retry:  return DialogResult::Retry;
					case DialogResult::Ignore: return DialogResult::Ignore;
					case DialogResult::Yes:    return DialogResult::Yes;
					case DialogResult::No:     return DialogResult::No;
					default: break;
				}
				return DialogResult::None;
			}
		}
	} // namespace Forms
} // namespace PsdotNet

#endif // PSDOTNET_FORMS_DIALOG_RESULT_H
