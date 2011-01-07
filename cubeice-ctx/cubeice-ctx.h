#ifndef		SHL_CTX_MENU_H
#define		SHL_CTX_MENU_H

#include <windows.h>
#include <string>
#include <vector>
#include <set>
#include "user-setting.h"
#include "cubeice-ctxdata.h"

extern HINSTANCE	hDllInstance;

namespace cube {
	namespace shlctxmenu {

		class CShlCtxMenu : public IContextMenu3, IShellExtInit, IMenuInfo {
		public:
			CShlCtxMenu() : refCount( 0L ) {}
			~CShlCtxMenu() {}

			// IUnknown members
			STDMETHODIMP QueryInterface( REFIID riid, LPVOID FAR *ppv ) {
				*ppv = NULL;

				if( IsEqualIID( riid, IID_IUnknown ) || IsEqualIID( riid, IID_IContextMenu ) )
					*ppv = static_cast<LPCONTEXTMENU>( this );
				else if( IsEqualIID( riid, IID_IContextMenu2 ) )
					*ppv = static_cast<LPCONTEXTMENU2>( this );
				else if( IsEqualIID( riid, IID_IContextMenu3 ) )
					*ppv = static_cast<LPCONTEXTMENU3>( this );
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
					
					if( MenuItem[i].dispSetting == DECOMPRESS_FLAG && !this->is_decompress(fileList.begin(), fileList.end()))
						continue;

					MENUITEMINFO		miinfo;

					ZeroMemory( &miinfo, sizeof( miinfo ) );
					miinfo.cbSize			= sizeof( miinfo );
					miinfo.fMask			= MIIM_FTYPE | MIIM_STRING | MIIM_ID;
					miinfo.fType			= MFT_STRING;
					miinfo.wID				= idCmd++;
#ifndef	UNICODE
					miinfo.dwTypeData		= const_cast<LPSTR>( MenuItem[i].stringA );
#else	// UNICODE
					miinfo.dwTypeData		= const_cast<LPWSTR>( MenuItem[i].stringW );
#endif	// UNICODE

					if( MenuItem[i].submenu ) {
						miinfo.fMask		|= MIIM_SUBMENU;
						miinfo.hSubMenu		= CreateSubMenu( MenuItem[i].submenu, idCmd );
					}
					if( MenuItem[i].iconID != ICON_NOT_USED ) {
						miinfo.fMask		|= MIIM_BITMAP | MIIM_DATA;
						miinfo.hbmpItem		= HBMMENU_CALLBACK;
						miinfo.dwItemData	= MenuItem[i].iconID;
					}

					InsertMenuItem( hMenu, indexMenu++, TRUE, &miinfo );
				}

				// Add separator
				InsertMenu( hMenu, indexMenu++, MF_BYPOSITION | MF_SEPARATOR, 0, NULL );

				MENUINFO	mi;
				ZeroMemory( &mi, sizeof( mi ) );
				mi.cbSize	= sizeof( mi );
				mi.fMask	= MIM_STYLE;
				GetMenuInfo( hMenu, &mi );
				mi.dwStyle	= ( mi.dwStyle & ~MNS_NOCHECK ) | MNS_CHECKORBMP;
				mi.fMask	= MIM_STYLE | MIM_APPLYTOSUBMENUS;
				SetMenuInfo( hMenu, &mi );

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

			// IContextMenu2 members
			STDMETHODIMP HandleMenuMsg( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
				HRESULT		res;
				return HandleMenuMsg2( uMsg, wParam, lParam, &res );
			}

			// IContextMenu3 members
			STDMETHODIMP HandleMenuMsg2( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *plResult ) {
				HRESULT		res;

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
						lpmis->itemHeight	= 16;
						lpmis->itemWidth	= 16;
						*plResult = TRUE;
						break;
					}

					case WM_DRAWITEM:
					{
						DRAWITEMSTRUCT		*lpdis = reinterpret_cast<DRAWITEMSTRUCT*>( lParam );

						if( !lpdis || lpdis->CtlType != ODT_MENU )
							return S_OK;

						HICON	hIcon = static_cast<HICON>( LoadImage( hDllInstance, MAKEINTRESOURCE( lpdis->itemData ), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR ) );

						if( !hIcon )
							return S_OK;

						DrawIconEx( lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, hIcon, 16, 16, 0, NULL, DI_NORMAL );

						DestroyIcon( hIcon );

						*plResult = TRUE;
						break;
					}
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
					miinfo.cbSize			= sizeof( miinfo );
					miinfo.fMask			= MIIM_FTYPE | MIIM_STRING | MIIM_ID;
					miinfo.fType			= MFT_STRING;
					miinfo.wID				= idCmd++;
#ifndef	UNICODE
					miinfo.dwTypeData		= const_cast<LPSTR>( smi[i].stringA );
#else	// UNICODE
					miinfo.dwTypeData		= const_cast<LPWSTR>( smi[i].stringW );
#endif	// UNICODE

					if( smi[i].submenu ) {
						miinfo.fMask		|= MIIM_SUBMENU;
						miinfo.hSubMenu		= CreateSubMenu( smi[i].submenu, idCmd );
					}
					if( smi[i].iconID != ICON_NOT_USED ) {
						miinfo.fMask		|= MIIM_BITMAP | MIIM_DATA;
						miinfo.hbmpItem		= HBMMENU_CALLBACK;
						miinfo.dwItemData	= smi[i].iconID;
					}

					InsertMenuItem( hMenu, i, TRUE, &miinfo );
				}

				return hMenu;
			}

			// Recursive invoke command
			void RecursiveInvokeCommand( const SUB_MENU_ITEM *smi, LPCMINVOKECOMMANDINFO lpcmi, DWORD &index ) {
				for( unsigned int i = 0 ; smi[i].stringA && index <= LOWORD( lpcmi->lpVerb ) ; ++i ) {
					if( smi[i].dispSetting && !( ctxSetting & smi[i].dispSetting ) )
						continue;

					if( smi[i].dispSetting == DECOMPRESS_FLAG && !this->is_decompress(fileList.begin(), fileList.end()))
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

					if( smi[i].dispSetting == DECOMPRESS_FLAG && !this->is_decompress(fileList.begin(), fileList.end()))
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
			
			template <class InputIterator>
			bool is_decompress(InputIterator first, InputIterator last) {
				static bool init_ = false;
				std::set<tstring> lists_;
				if (!init_) {
					lists_.insert(_T(".zip"));
					lists_.insert(_T(".lzh"));
					lists_.insert(_T(".rar"));
					lists_.insert(_T(".tar"));
					lists_.insert(_T(".gz"));
					lists_.insert(_T(".7z"));
					lists_.insert(_T(".arj"));
					lists_.insert(_T(".bz2"));
					lists_.insert(_T(".cab"));
					lists_.insert(_T(".chm"));
					lists_.insert(_T(".cpio"));
					lists_.insert(_T(".deb"));
					lists_.insert(_T(".dmg"));
					lists_.insert(_T(".iso"));
					lists_.insert(_T(".msi"));
					lists_.insert(_T(".rpm"));
					lists_.insert(_T(".tbz"));
					lists_.insert(_T(".tgz"));
					lists_.insert(_T(".udf"));
					lists_.insert(_T(".vhd"));
					lists_.insert(_T(".wim"));
					lists_.insert(_T(".xar"));
					lists_.insert(_T(".xz"));
					lists_.insert(_T(".z"));
				}
				
				for( ; first != last; ++first ) {
					tstring src = *first;
					tstring::size_type pos = src.find_last_of(_T("."));
					if( pos != tstring::npos && lists_.find(src.substr(pos)) != lists_.end() ) return true;
				}
				return false;
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
