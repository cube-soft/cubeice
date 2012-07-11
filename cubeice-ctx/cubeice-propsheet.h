#pragma once
#include <Windows.h>
#include <shlobj.h>
#include <tchar.h>
#include <string>

extern HINSTANCE	hDllInstance;

namespace CubeICE {
		/* ----------------------------------------------------------------- */
		//  CShellPropSheetExt
		/* ----------------------------------------------------------------- */
		class CShellPropSheetExt : public IShellPropSheetExt, public IShellExtInit
		{
		public:
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			CShellPropSheetExt( ULONG &dllrc ) : refCount( 1UL ), dllRefCount( dllrc ) {
				InterlockedIncrement( reinterpret_cast<LONG*>(&dllRefCount) );
			}

			/* ------------------------------------------------------------- */
			//  destructor
			/* ------------------------------------------------------------- */
			virtual ~CShellPropSheetExt(void) {
				InterlockedDecrement( reinterpret_cast<LONG*>(&dllRefCount) );
			}

			/* ------------------------------------------------------------- */
			//  IUnknown members
			/* ------------------------------------------------------------- */
			STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject) {
				*ppvObject = NULL;
				
				if( IsEqualIID( riid, IID_IUnknown ) || IsEqualIID( riid, IID_IShellPropSheetExt ) )
					*ppvObject = static_cast<LPSHELLPROPSHEETEXT>( this );
				else if( IsEqualIID( riid, IID_IShellExtInit ) )
					*ppvObject = static_cast<LPSHELLEXTINIT>( this );
				else
					return E_NOINTERFACE;
				
				AddRef();
				
				return NOERROR;
			}
			STDMETHODIMP_(ULONG) AddRef() {
				return InterlockedIncrement( reinterpret_cast<LONG*>(&refCount) );
			}
			STDMETHODIMP_(ULONG) Release() {
				ULONG res = InterlockedDecrement( reinterpret_cast<LONG*>(&refCount) );
				
				if( res )
					return res;
				
				delete this;
				
				return 0L;
			}

			/* ------------------------------------------------------------- */
			//  IShellPropSheetExt members
			/* ------------------------------------------------------------- */
			STDMETHODIMP AddPages(LPFNADDPROPSHEETPAGE pfnAddPage, LPARAM lParam);
			STDMETHODIMP ReplacePage(UINT uPageID, LPFNADDPROPSHEETPAGE pfnReplacePage, LPARAM lParam);

			/* ------------------------------------------------------------- */
			//  IShellExtInit members
			/* ------------------------------------------------------------- */
			STDMETHODIMP Initialize(PCIDLIST_ABSOLUTE pidlFolder, IDataObject *pdtobj, HKEY hkeyProgID);

			std::basic_string<TCHAR>	filepath;

		private:
			ULONG		refCount;
			ULONG		&dllRefCount;
		};

		/* ----------------------------------------------------------------- */
		//  CShellPropSheetFactory
		/* ----------------------------------------------------------------- */
		class CShellPropSheetFactory : public IClassFactory
		{
		public:
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			CShellPropSheetFactory( ULONG &dllrc ) : refCount( 1UL ), dllRefCount( dllrc ) {
				InterlockedIncrement( reinterpret_cast<LONG*>(&dllRefCount) );
			}
			
			/* ------------------------------------------------------------- */
			//  destructor
			/* ------------------------------------------------------------- */
			virtual ~CShellPropSheetFactory() {
				InterlockedDecrement( reinterpret_cast<LONG*>(&dllRefCount) );
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
				
				CShellPropSheetExt	*p;
				HRESULT				hr;
				p = new CShellPropSheetExt( dllRefCount );
				
				if( !p )
					return E_OUTOFMEMORY;
				
				hr = p->QueryInterface( riid, ppvObj );
				p->Release();

				return hr;
			}
			
			/* ------------------------------------------------------------- */
			//  LockServer
			/* ------------------------------------------------------------- */
			STDMETHODIMP LockServer( BOOL ) {
				return NOERROR;
			}
			
		private:
			ULONG		refCount;
			ULONG		&dllRefCount;
		};
}
