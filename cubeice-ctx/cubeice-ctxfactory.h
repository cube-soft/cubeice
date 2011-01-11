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

		class CShlCtxMenuFactory : public IClassFactory
		{
		public:
			CShlCtxMenuFactory( UINT &dllrc ) : refCount( 0L ), dllRefCount( dllrc ) {
				++dllRefCount;
			}
			~CShlCtxMenuFactory() {
				--dllRefCount;
			}

			// IUnknown members
			STDMETHODIMP QueryInterface( REFIID riid, LPVOID FAR *ppv ) {
				*ppv = NULL;

				if( IsEqualIID( riid, IID_IUnknown ) || IsEqualIID( riid, IID_IClassFactory ) ) {
					*ppv = static_cast<LPCLASSFACTORY>( this );

					AddRef();

					return NOERROR;
				}

				return E_NOINTERFACE;
			}

			STDMETHODIMP_(ULONG) AddRef() {
				return InterlockedIncrement( &refCount );
			}

			STDMETHODIMP_(ULONG) Release() {
				ULONG res = InterlockedDecrement( &refCount );

				if( res )
					return res;

				delete this;

				return 0L;
			}

			// IClassFactory members
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
