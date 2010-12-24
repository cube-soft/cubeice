#ifndef		SHL_CTX_MENU_H
#define		SHL_CTX_MENU_H

#include <windows.h>
#include <string>
#include <vector>
#include "user-setting.h"
#include "cubeice-ctxdata.h"

extern HINSTANCE	hDllInstance;

namespace cube {
	namespace shlctxmenu {

		class CShlCtxMenu : public IContextMenu, IShellExtInit, IMenuInfo {
		public:
			CShlCtxMenu() : refCount( 0L ) {}
			~CShlCtxMenu() {}

			// IUnknown members
			STDMETHODIMP QueryInterface( REFIID riid, LPVOID FAR *ppv ) {
				*ppv = NULL;

				if( IsEqualIID( riid, IID_IUnknown ) || IsEqualIID( riid, IID_IContextMenu ) )
					*ppv = static_cast<LPCONTEXTMENU>( this );
				else if( IsEqualIID( riid, IID_IShellExtInit ) )
					*ppv = static_cast<LPSHELLEXTINIT>( this );
				else
					return E_NOINTERFACE;

				AddRef();

				return NOERROR;
			}

			STDMETHODIMP_(ULONG) AddRef() {
				return ++refCount;
			}

			STDMETHODIMP_(ULONG) Release() {
				if( --refCount )
					return refCount;

				delete this;

				return 0L;
			}

			// IContextMenu members
			STDMETHODIMP QueryContextMenu( HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags ) {
				UINT						idCmd = idCmdFirst;

				if( uFlags & CMF_DEFAULTONLY )
					return NO_ERROR;

				if( !ctxSetting )
					return NO_ERROR;

				// Add separator
				InsertMenu( hMenu, indexMenu++, MF_BYPOSITION | MF_SEPARATOR, 0, NULL );

				for( unsigned int i = 0 ; MenuItem[i].stringA ; ++i ) {
					if( MenuItem[i].dispSetting && !( ctxSetting & MenuItem[i].dispSetting ) )
						continue;

					MENUITEMINFO		miinfo;

					ZeroMemory( &miinfo, sizeof( miinfo ) );
					miinfo.cbSize		= sizeof( miinfo );
					miinfo.fMask		= MIIM_ID | MIIM_TYPE;
					miinfo.fType		= MFT_STRING;
					miinfo.wID			= idCmd++;
#ifndef	UNICODE
					miinfo.dwTypeData	= const_cast<LPSTR>( MenuItem[i].stringA );
#else	// UNICODE
					miinfo.dwTypeData	= const_cast<LPWSTR>( MenuItem[i].stringW );
#endif	// UNICODE

					if( MenuItem[i].submenu ) {
						miinfo.fMask	|= MIIM_SUBMENU;
						miinfo.hSubMenu	= CreateSubMenu( MenuItem[i].submenu, idCmd );
					}
					InsertMenuItem( hMenu, indexMenu++, TRUE, &miinfo );

					if( MenuItem[i].iconID != ICON_NOT_USED ) {
						HBITMAP		hBitmap = static_cast<HBITMAP>( LoadImage( hDllInstance, MAKEINTRESOURCE( MenuItem[i].iconID ), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_SHARED ) );
						SetMenuItemBitmaps( hMenu, indexMenu - 1, MF_BYPOSITION, hBitmap, hBitmap );
					}
				}

				// Add separator
				InsertMenu( hMenu, indexMenu++, MF_BYPOSITION | MF_SEPARATOR, 0, NULL );

				return MAKE_SCODE( SEVERITY_SUCCESS, FACILITY_NULL, idCmd - idCmdFirst );
			}

			STDMETHODIMP InvokeCommand( LPCMINVOKECOMMANDINFO lpcmi ) {
				if( HIWORD( lpcmi->lpVerb ) )
					return E_INVALIDARG;

				DWORD	index = 0;

				RecursiveInvokeCommand( MenuItem, lpcmi, index );

				return S_OK;
			}

			STDMETHODIMP GetCommandString( UINT_PTR idCmd, UINT uFlags, UINT FAR *reserved, LPSTR pszName, UINT cchMax ) {
				unsigned int	index = 0;

				RecursiveGetCommandString( MenuItem, index, idCmd, uFlags, pszName, cchMax );

				if( index < idCmd ) {
					if( uFlags == GCS_VALIDATEA || uFlags == GCS_VALIDATEW )
						return S_FALSE;
				}
				return S_OK;
			}

			// IShellExtInit methods
			STDMETHODIMP Initialize( LPCITEMIDLIST pIDFolder, LPDATAOBJECT pDataObj, HKEY hKeyID ) {
				HRESULT			hr;
				FORMATETC		fme;
				STGMEDIUM		stgm;
				HDROP			hDrop;
				UINT			numoffiles;

				cubeice::user_setting	setting;
				ctxSetting = setting.context_flags();

				fme.cfFormat	= CF_HDROP;
				fme.ptd			= NULL;
				fme.dwAspect	= DVASPECT_CONTENT;
				fme.lindex		= -1;
				fme.tymed		= TYMED_HGLOBAL;

				hr = pDataObj->GetData( &fme, &stgm );
				if( FAILED( hr ) )
					return hr;

				hDrop = static_cast<HDROP>( GlobalLock( stgm.hGlobal ) );
				numoffiles = DragQueryFile( hDrop, static_cast<UINT>( -1 ), NULL, 0 );

				for( UINT i = 0 ; i < numoffiles ; ++i ) {
					UINT	bufsize;
					TCHAR	*buffer;

					bufsize = DragQueryFile( hDrop, i, NULL, 0 );
					buffer = new TCHAR[bufsize+5];
					DragQueryFile( hDrop, i, buffer, bufsize + 3 );

					fileList.push_back( tstring( buffer ) );

					delete[] buffer;
				}

				GlobalUnlock( hDrop );
				ReleaseStgMedium( &stgm );

				return S_OK;
			}

			// IMenuInfo methods
			const std::vector<tstring> &GetFileList() {
				return fileList;
			}

			const tstring &GetFilePath( const std::size_t &index ) {
				return fileList.at( index );
			}

			std::size_t GetSize() {
				return fileList.size();
			}

		private:
			// Create sub menu
			HMENU CreateSubMenu( const SUB_MENU_ITEM *smi, UINT &idCmd ) {
				HMENU		hMenu = CreateMenu();

				for( unsigned int i = 0 ; smi[i].stringA ; ++i ) {
					MENUITEMINFO		miinfo;

					if( smi[i].dispSetting && !( ctxSetting & smi[i].dispSetting ) )
						continue;

					ZeroMemory( &miinfo, sizeof( miinfo ) );
					miinfo.cbSize		= sizeof( miinfo );
					miinfo.fMask		= MIIM_ID | MIIM_TYPE;
					miinfo.fType		= MFT_STRING;
					miinfo.wID			= idCmd++;
#ifndef	UNICODE
					miinfo.dwTypeData	= const_cast<LPSTR>( smi[i].stringA );
#else	// UNICODE
					miinfo.dwTypeData	= const_cast<LPWSTR>( smi[i].stringW );
#endif	// UNICODE

					if( smi[i].submenu ) {
						miinfo.fMask	|= MIIM_SUBMENU;
						miinfo.hSubMenu	= CreateSubMenu( smi[i].submenu, idCmd );
					}
					InsertMenuItem( hMenu, i, TRUE, &miinfo );

					if( smi[i].iconID != ICON_NOT_USED ) {
						HBITMAP		hBitmap = static_cast<HBITMAP>( LoadImage( hDllInstance, MAKEINTRESOURCE( smi[i].iconID ), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_SHARED ) );
						SetMenuItemBitmaps( hMenu, i, MF_BYPOSITION, hBitmap, hBitmap );
					}
				}

				return hMenu;
			}

			// Recursive invoke command
			void RecursiveInvokeCommand( const SUB_MENU_ITEM *smi, LPCMINVOKECOMMANDINFO lpcmi, DWORD &index ) {
				for( unsigned int i = 0 ; smi[i].stringA && index <= LOWORD( lpcmi->lpVerb ) ; ++i ) {
					if( smi[i].dispSetting && !( ctxSetting & smi[i].dispSetting ) )
						continue;

					if( index == LOWORD( lpcmi->lpVerb ) ) {
						MenuSelectedCallback( smi[i].arg, this );
						++index;
						break;
					}
					++index;
					if( smi[i].submenu )
						RecursiveInvokeCommand( smi[i].submenu, lpcmi, index );
				}

				return;
			}

			// Recursive get command string
			void RecursiveGetCommandString( const SUB_MENU_ITEM *smi, unsigned int &index, UINT_PTR idCmd, UINT uFlags, LPSTR pszName, UINT cchMax ) {
				for( unsigned int i = 0 ; smi[i].stringA && index <= idCmd ; ++i ) {
					if( smi[i].dispSetting && !( ctxSetting & smi[i].dispSetting ) )
						continue;

					if( index == idCmd ) {
						switch( uFlags ) {
							case GCS_HELPTEXTA:
								strncpy_s( pszName, cchMax, smi[i].explanationA, _TRUNCATE );
								break;
							case GCS_HELPTEXTW:
								wcsncpy_s( reinterpret_cast<LPWSTR>( pszName ), cchMax, smi[i].explanationW, _TRUNCATE );
								break;
//							case GCS_VALIDATEA:
//							case GCS_VALIDATEW:
							case GCS_VERBA:
								strncpy_s( pszName, cchMax, smi[i].stringA, _TRUNCATE );
								break;
							case GCS_VERBW:
								wcsncpy_s( reinterpret_cast<LPWSTR>( pszName ), cchMax, smi[i].stringW, _TRUNCATE );
								break;
						}
						++index;
						break;
					}
					++index;
					if( smi[i].submenu )
						RecursiveGetCommandString( smi[i].submenu, index, idCmd, uFlags, pszName, cchMax );
				}

				return;
			}

			std::vector<tstring>				fileList;
			cubeice::user_setting::size_type	ctxSetting;
			ULONG								refCount;
		};

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
				return ++refCount;
			}

			STDMETHODIMP_(ULONG) Release() {
				if( --refCount )
					return refCount;

				delete this;

				return 0L;
			}

			// IClassFactory members
			STDMETHODIMP CreateInstance( LPUNKNOWN pUnkOuter, REFIID riid, LPVOID FAR *ppvObj ) {
				*ppvObj = NULL;

				if( pUnkOuter )
					return CLASS_E_NOAGGREGATION;

				CShlCtxMenu *pSCMenu = new CShlCtxMenu();

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

#endif	// SHL_CTX_MENU_H
