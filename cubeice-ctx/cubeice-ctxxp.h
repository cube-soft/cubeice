#ifndef	CUBEICE_CTXXP_H
#define	CUBEICE_CTXXP_H

#include "cubeice-ctxbase.h"

namespace cube {
	namespace shlctxmenu {

		class CShlCtxMenuXP : public CShlCtxMenuBase {
		public:
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
						lpmis->itemWidth	= ICON_X_SIZE;
						lpmis->itemHeight	= ICON_Y_SIZE;
						*plResult = TRUE;
						break;
					}
					case WM_DRAWITEM:
					{
						DRAWITEMSTRUCT		*lpdis = reinterpret_cast<DRAWITEMSTRUCT*>( lParam );

						if( !lpdis || lpdis->CtlType != ODT_MENU )
							return S_OK;

						HICON	hIcon = static_cast<HICON>( LoadImage( hInstance, MAKEINTRESOURCE( lpdis->itemData ), IMAGE_ICON, ICON_X_SIZE, ICON_Y_SIZE, LR_DEFAULTCOLOR ) );

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

		protected:
			void SetMenuIcon( WORD iconID, MENUITEMINFO &miinfo )
			{
				miinfo.fMask		|= MIIM_BITMAP | MIIM_DATA;
				miinfo.hbmpItem		= HBMMENU_CALLBACK;
				miinfo.dwItemData	= iconID;

				return;
			}
		};

	}
}

#endif	// CUBEICE_CTXXP_H
