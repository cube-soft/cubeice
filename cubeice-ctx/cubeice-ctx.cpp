/* ------------------------------------------------------------------------- */
/*
 *  cubeice-ctx.cpp
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
#include <initguid.h>
#include <cubeice/config.h>
#include <shlguid.h>
#include <shlobj.h>
#include "cubeice-ctxfactory.h"
//#include "cubeice-propsheet.h"

namespace {
	ULONG	dllRefCount = 0;
}
HINSTANCE		hDllInstance;

/* ------------------------------------------------------------------------- */
//  DllMain
/* ------------------------------------------------------------------------- */
extern "C" int APIENTRY
DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved )
{
	switch( dwReason ) {
		case DLL_PROCESS_ATTACH:
			hDllInstance = hInstance;
			break;

		case DLL_PROCESS_DETACH:
			break;

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;
	}
	return TRUE;
}

/* ------------------------------------------------------------------------- */
//  DllCanUnloadNow
/* ------------------------------------------------------------------------- */
STDAPI DllCanUnloadNow( void )
{
	return ( dllRefCount == 0 ? S_OK : S_FALSE );
}

/* ------------------------------------------------------------------------- */
//  DllGetClassObject
/* ------------------------------------------------------------------------- */
STDAPI DllGetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppvOut )
{
	using CubeICE::CShlCtxMenuFactory;
	//using CubeICE::CShellPropSheetFactory;

	*ppvOut = NULL;

	if( IsEqualCLSID( rclsid, CLSID_CubeICE_CTX ) ) {
		CShlCtxMenuFactory	*pscmf = new CShlCtxMenuFactory( dllRefCount );
		HRESULT				hr;

		if( !pscmf )
			return E_OUTOFMEMORY;

		hr = pscmf->QueryInterface( riid, ppvOut );
		pscmf->Release();

		return hr;
	}
#ifdef CUBEICE_USE_PROPSHEET
	else if( IsEqualCLSID( rclsid, CLSID_CubeICE_PROPSHEET ) ) {
		CShellPropSheetFactory	*pscmf = new CShellPropSheetFactory( dllRefCount );
		HRESULT				hr;

		if( !pscmf )
			return E_OUTOFMEMORY;

		hr = pscmf->QueryInterface( riid, ppvOut );
		pscmf->Release();

		return hr;
	}
#endif
	return CLASS_E_CLASSNOTAVAILABLE;
}
