/* ------------------------------------------------------------------------- */
/*
 *  cubeice-ctxfactory.h
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
#ifndef		CUBEICE_CTXFACTORY_H
#define		CUBEICE_CTXFACTORY_H

#include <windows.h>
#include <string>
#include <vector>
#include <set>
#include "user-setting.h"
#include "cubeice-ctxdata.h"
#include "cubeice-ctxxp.h"
#include "cubeice-ctxvista.h"

namespace cube {
	namespace shlctxmenu {
		/* ----------------------------------------------------------------- */
		//  CShlCtxMenuFactory
		/* ----------------------------------------------------------------- */
		class CShlCtxMenuFactory : public IClassFactory
		{
		public:
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			CShlCtxMenuFactory( UINT &dllrc ) : refCount( 0L ), dllRefCount( dllrc ) {
				++dllRefCount;
			}
			
			/* ------------------------------------------------------------- */
			//  destructor
			/* ------------------------------------------------------------- */
			virtual ~CShlCtxMenuFactory() {
				--dllRefCount;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  QueryInterface
			 *
			 *  IUnknown members
			 */
			/* ------------------------------------------------------------- */
			STDMETHODIMP QueryInterface( REFIID riid, LPVOID FAR *ppv ) {
				*ppv = NULL;
				
				if( IsEqualIID( riid, IID_IUnknown ) || IsEqualIID( riid, IID_IClassFactory ) ) {
					*ppv = static_cast<LPCLASSFACTORY>( this );
					
					AddRef();
					
					return NOERROR;
				}
				
				return E_NOINTERFACE;
			}
			
			/* ------------------------------------------------------------- */
			//  AddRef
			/* ------------------------------------------------------------- */
			STDMETHODIMP_(ULONG) AddRef() {
				return InterlockedIncrement( reinterpret_cast<LONG*>(&refCount) );
			}
			
			/* ------------------------------------------------------------- */
			//  Release
			/* ------------------------------------------------------------- */
			STDMETHODIMP_(ULONG) Release() {
				ULONG res = InterlockedDecrement( reinterpret_cast<LONG*>(&refCount) );
				
				if( res )
					return res;
				
				delete this;
				
				return 0L;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  CreateInstance
			 *
			 *  IClassFactory members
			 */
			/* ------------------------------------------------------------- */
			STDMETHODIMP CreateInstance( LPUNKNOWN pUnkOuter, REFIID riid, LPVOID FAR *ppvObj ) {
				*ppvObj = NULL;
				
				if( pUnkOuter )
					return CLASS_E_NOAGGREGATION;
				
				OSVERSIONINFOEX		osver;
				SecureZeroMemory( &osver, sizeof( osver ) );
				osver.dwOSVersionInfoSize	= sizeof( osver );
				GetVersionEx( reinterpret_cast<OSVERSIONINFO*>(&osver) );
				
				CShlCtxMenuBase	*pSCMenu;
				if( MAKEWORD( osver.dwMinorVersion, osver.dwMajorVersion ) >= 0x0600 )
					pSCMenu = new CShlCtxMenuVista();
				else
					pSCMenu = new CShlCtxMenuXP();
				
				if( !pSCMenu )
					return E_OUTOFMEMORY;
				
				return pSCMenu->QueryInterface( riid, ppvObj );
			}
			
			/* ------------------------------------------------------------- */
			//  LockServer
			/* ------------------------------------------------------------- */
			STDMETHODIMP LockServer( BOOL ) {
				return NOERROR;
			}
			
		private:
			ULONG		refCount;
			UINT		&dllRefCount;
		};
	}
}

#endif	// CUBEICE_CTXFACTORY_H
