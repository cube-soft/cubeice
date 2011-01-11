#include <initguid.h>
#include <shlguid.h>
#include <shlobj.h>
#include "cubeice-ctxfactory.h"
#include "guid.h"

namespace {
	UINT	dllRefCount = 0;
}
HINSTANCE		hDllInstance;

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

STDAPI DllCanUnloadNow( void )
{
	return ( dllRefCount == 0 ? S_OK : S_FALSE );
}

STDAPI DllGetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppvOut )
{
	using cube::shlctxmenu::CShlCtxMenuFactory;

	*ppvOut = NULL;

	if( IsEqualIID( rclsid, CLSID_CubeICE ) ) {
		CShlCtxMenuFactory *pscmf = new CShlCtxMenuFactory( dllRefCount );

		return pscmf->QueryInterface( riid, ppvOut );
	}

	return CLASS_E_CLASSNOTAVAILABLE;
}
