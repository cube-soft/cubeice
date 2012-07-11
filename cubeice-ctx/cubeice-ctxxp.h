/* ------------------------------------------------------------------------- */
/*
 *  cubeice-ctxxp.h
 *
 *  Copyright (c) 2010 CubeSoft Inc.
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
 *  Last-modified: Wed 12 Jan 2011 14:28:04 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef	CUBEICE_CTXXP_H
#define	CUBEICE_CTXXP_H

#include "cubeice-ctxbase.h"

namespace CubeICE {
		/* ----------------------------------------------------------------- */
		//  CShlCtxMenuXP
		/* ----------------------------------------------------------------- */
		class CShlCtxMenuXP : public CShlCtxMenuBase {
		public:
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			CShlCtxMenuXP( ULONG &dllrc ) : CShlCtxMenuBase( dllrc ) {}

			/* ------------------------------------------------------------- */
			//  QueryInterface
			/* ------------------------------------------------------------- */
			STDMETHODIMP QueryInterface( REFIID riid, LPVOID FAR *ppv ) {
				*ppv = NULL;
				
				if( IsEqualIID( riid, IID_IUnknown ) || IsEqualIID( riid, IID_IContextMenu ) )
					*ppv = static_cast<LPCONTEXTMENU>( this );
				else if( IsEqualIID( riid, IID_IContextMenu2 ) )
					*ppv = static_cast<LPCONTEXTMENU2>( this );
				else if( IsEqualIID( riid, IID_IContextMenu3 ) )
					*ppv = static_cast<LPCONTEXTMENU3>( this );
				//else if( IsEqualIID( riid, IID_IShellExtInit ) )
				//	*ppv = static_cast<LPSHELLEXTINIT>( this );
				else
					return CShlCtxMenuBase::QueryInterface( riid, ppv );
				
				AddRef();
				
				return NOERROR;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  HandleMenuMsg
			 *
			 *  IContextMenu2 members
			 */
			/* ------------------------------------------------------------- */
			STDMETHODIMP HandleMenuMsg( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
				LRESULT		res;
				return HandleMenuMsg2( uMsg, wParam, lParam, &res );
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  HandleMenuMsg2
			 *
			 *  IContextMenu3 members
			 */
			/* ------------------------------------------------------------- */
			STDMETHODIMP HandleMenuMsg2( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *plResult ) {
				LRESULT		res;
				
				if( !plResult )
					plResult = &res;
				*plResult = FALSE;
				
				switch( uMsg ) {
					//case WM_INITMENUPOPUP:
					//	break;
					//case WM_MENUCHAR:
					//	break;
					
					case WM_MEASUREITEM:
					{
						MEASUREITEMSTRUCT	*lpmis = reinterpret_cast<MEASUREITEMSTRUCT*>( lParam );
						
						if( !lpmis )
							return S_OK;
						lpmis->itemWidth	= 16;
						lpmis->itemHeight	= 16;
						*plResult = TRUE;
						break;
					}
					case WM_DRAWITEM:
					{
						DRAWITEMSTRUCT *lpdis = reinterpret_cast<DRAWITEMSTRUCT*>(lParam);
						if(!lpdis || lpdis->CtlType != ODT_MENU) return S_OK;
						
						context_map::iterator pos = this->InsertedItems().find(lpdis->itemData);
						if (pos == this->InsertedItems().end()) return S_OK;
						
						string_type location = pos->second.IconLocation();
						string_type path = location.substr(0, location.find(_T(",")));
						int index = 0;
						try {
							string_type::size_type pos = location.find(_T(","));
							if (pos != string_type::npos) index = boost::lexical_cast<int>(location.substr(pos + 1));
						}
						catch (boost::bad_lexical_cast& /* err */) {}
						
						HICON hIcon = static_cast<HICON>(::ExtractIcon(NULL, path.c_str(), index));
						if( !hIcon ) return S_OK;
						
						DrawIconEx( lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, hIcon, 16, 16, 0, NULL, DI_NORMAL );
						
						DestroyIcon( hIcon );
						
						*plResult = TRUE;
						break;
					}
				}
				
				return S_OK;
			}
			
		protected:
			/* ------------------------------------------------------------- */
			//  SetMenuIcon
			/* ------------------------------------------------------------- */
			void SetMenuIcon(UINT cmdid, MENUITEMINFO &miinfo)
			{
				miinfo.fMask		|= MIIM_BITMAP | MIIM_DATA;
				miinfo.hbmpItem		= HBMMENU_CALLBACK;
				miinfo.dwItemData	= static_cast<DWORD>(cmdid);
				
				return;
			}
		};
}

#endif	// CUBEICE_CTXXP_H
