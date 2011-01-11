#ifndef	CUBEICE_CTXVISTA_H
#define	CUBEICE_CTXVISTA_H

#include <windows.h>
#include <Uxtheme.h>
#include <map>
#include <clx/shared_ptr.h>
#include "cubeice-ctxdata.h"
#include "cubeice-ctxbase.h"

namespace cube {
	namespace shlctxmenu {
		namespace icon {
			class Icon {
			public:
				Icon( HINSTANCE hInstance, WORD iconID ) : hBitmap( NULL ) {

					hUxTheme = LoadLibrary( TEXT( "UXTHEME.DLL" ) );
					if( !hUxTheme )
						return;
					pfnGetBufferedPaintBits	= reinterpret_cast<FN_GetBufferedPaintBits>( GetProcAddress( hUxTheme, "GetBufferedPaintBits" ) );
					pfnBeginBufferedPaint	= reinterpret_cast<FN_BeginBufferedPaint>( GetProcAddress( hUxTheme, "BeginBufferedPaint" ) );
					pfnEndBufferedPaint		= reinterpret_cast<FN_EndBufferedPaint>( GetProcAddress( hUxTheme, "EndBufferedPaint" ) );

					if( !pfnGetBufferedPaintBits || !pfnBeginBufferedPaint || !pfnEndBufferedPaint )
						return;

					HICON	hIcon = static_cast<HICON>( LoadImage( hInstance, MAKEINTRESOURCE( iconID ), IMAGE_ICON, ICON_X_SIZE, ICON_Y_SIZE, LR_DEFAULTCOLOR ) );
					if( !hIcon )
						return;

					HDC		hdc = CreateCompatibleDC( NULL );
					if( !hdc ) {
						DestroyIcon( hIcon );
						return;
					}

					BITMAPINFO	bmi;
					ZeroMemory( &bmi, sizeof( bmi ) );
					bmi.bmiHeader.biSize		= sizeof( bmi );
					bmi.bmiHeader.biPlanes		= 1;
					bmi.bmiHeader.biCompression	= BI_RGB;
					bmi.bmiHeader.biWidth		= ICON_X_SIZE;
					bmi.bmiHeader.biHeight		= ICON_Y_SIZE;
					bmi.bmiHeader.biBitCount	= 32;

					hBitmap = CreateDIBSection( hdc, &bmi, DIB_RGB_COLORS, NULL, NULL, 0 );
					if( !hBitmap ) {
						DeleteDC( hdc );
						DestroyIcon( hIcon );
						return;
					}

					HBITMAP	hOldBmp = static_cast<HBITMAP>( SelectObject( hdc, static_cast<HGDIOBJ>(hBitmap) ) );

					BLENDFUNCTION	bfAlpha = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
					BP_PAINTPARAMS	paintParams = {0};
					paintParams.cbSize			= sizeof(paintParams);
					paintParams.dwFlags			= BPPF_ERASE;
					paintParams.pBlendFunction	= &bfAlpha;

					HDC				hdcBuffer;
					RECT			rc;
					SetRect( &rc, 0, 0, ICON_X_SIZE, ICON_Y_SIZE );
					HPAINTBUFFER	hPaintBuffer = pfnBeginBufferedPaint( hdc, &rc, BPBF_DIB, &paintParams, &hdcBuffer );
					if( hPaintBuffer ) {
						if( DrawIconEx( hdcBuffer, 0, 0, hIcon, ICON_X_SIZE, ICON_Y_SIZE, 0, NULL, DI_NORMAL ) ) {
							SIZE	sz = { ICON_X_SIZE, ICON_Y_SIZE };
							ConvertBufferToPARGB32( hPaintBuffer, hdc, hIcon, sz );
						}
						pfnEndBufferedPaint( hPaintBuffer, TRUE );
					}
					SelectObject( hdc, static_cast<HGDIOBJ>(hOldBmp) );

					DeleteDC( hdc );
					DestroyIcon( hIcon );
				}
				~Icon() {
					if( hBitmap )
						DeleteObject( hBitmap );
					FreeLibrary( hUxTheme );
				}

				HBITMAP GetBitmap() {
					return hBitmap;
				}

			private:
				typedef HRESULT ( WINAPI *FN_GetBufferedPaintBits )( HPAINTBUFFER hBufferedPaint, RGBQUAD **ppbBuffer, int *pcxRow );
				typedef HPAINTBUFFER ( WINAPI *FN_BeginBufferedPaint )( HDC hdcTarget, const RECT *prcTarget, BP_BUFFERFORMAT dwFormat, BP_PAINTPARAMS *pPaintParams, HDC *phdc );
				typedef HRESULT ( WINAPI *FN_EndBufferedPaint )( HPAINTBUFFER hBufferedPaint, BOOL fUpdateTarget );
				typedef DWORD ARGB;

				HMODULE					hUxTheme;
				FN_GetBufferedPaintBits	pfnGetBufferedPaintBits;
				FN_BeginBufferedPaint	pfnBeginBufferedPaint;
				FN_EndBufferedPaint		pfnEndBufferedPaint;
				HBITMAP					hBitmap;

				HRESULT ConvertBufferToPARGB32( HPAINTBUFFER hPaintBuffer, HDC hdc, HICON hicon, SIZE &sizIcon )
				{
					RGBQUAD		*prgbQuad;
					int			cxRow;
					HRESULT		hr = pfnGetBufferedPaintBits( hPaintBuffer, &prgbQuad, &cxRow );

					if( SUCCEEDED( hr ) ) {
						ARGB	*pargb = reinterpret_cast<ARGB *>(prgbQuad);
						if( !HasAlpha( pargb, sizIcon, cxRow ) ) {
							ICONINFO	info;
							if( GetIconInfo( hicon, &info ) ) {
								if( info.hbmMask ) {
									hr = ConvertToPARGB32( hdc, pargb, info.hbmMask, sizIcon, cxRow );
								}

								DeleteObject( info.hbmColor );
								DeleteObject( info.hbmMask );
							}
						}
					}

					return hr;
				}

				bool HasAlpha( ARGB *pargb, SIZE& sizImage, int cxRow )
				{
					ULONG	cxDelta = cxRow - sizImage.cx;
					for( ULONG y = sizImage.cy ; y ; --y ) {
						for( ULONG x = sizImage.cx ; x ; --x ) {
							if( *pargb++ & 0xFF000000 )
								return true;
						}
						pargb += cxDelta;
					}

					return false;
				}

				HRESULT ConvertToPARGB32( HDC hdc, ARGB *pargb, HBITMAP hbmp, SIZE &sizImage, int cxRow )
				{
					BITMAPINFO	bmi;
					ZeroMemory( &bmi, sizeof( bmi ) );
					bmi.bmiHeader.biSize		= sizeof( bmi );
					bmi.bmiHeader.biPlanes		= 1;
					bmi.bmiHeader.biCompression	= BI_RGB;
					bmi.bmiHeader.biWidth		= ICON_X_SIZE;
					bmi.bmiHeader.biHeight		= ICON_Y_SIZE;
					bmi.bmiHeader.biBitCount	= 32;

					HRESULT		hr = E_OUTOFMEMORY;
					HANDLE		hHeap = GetProcessHeap();
					void *pvBits = HeapAlloc( hHeap, 0, bmi.bmiHeader.biWidth * 4 * bmi.bmiHeader.biHeight );
					if( pvBits )
					{
						hr = E_UNEXPECTED;
						if( GetDIBits( hdc, hbmp, 0, bmi.bmiHeader.biHeight, pvBits, &bmi, DIB_RGB_COLORS) == bmi.bmiHeader.biHeight ) {
							ULONG	cxDelta = cxRow - bmi.bmiHeader.biWidth;
							ARGB	*pargbMask = static_cast<ARGB *>(pvBits);

							for( ULONG y = bmi.bmiHeader.biHeight ; y ; --y ) {
								for( ULONG x = bmi.bmiHeader.biWidth ; x ; --x ) {
									if( *pargbMask++ )
										*pargb++ = 0;
									else
										*pargb++ |= 0xFF000000;
								}
								pargb += cxDelta;
							}
							hr = S_OK;
						}
						HeapFree( hHeap, 0, pvBits );
					}

					return hr;
				}

			};
		}

		class CShlCtxMenuVista : public CShlCtxMenuBase {
		public:
			CShlCtxMenuVista() {
				InitIcon( MenuItem );
			}

			~CShlCtxMenuVista() {
			}

		protected:
			void SetMenuIcon( WORD iconID, MENUITEMINFO &miinfo )
			{
				miinfo.fMask		|= MIIM_BITMAP;
				miinfo.hbmpItem		= bitmaps[iconID]->GetBitmap();
			}

		private:
			std::map<WORD,clx::shared_ptr<icon::Icon> >		bitmaps;

			void InitIcon( const SUB_MENU_ITEM *smi ) {
				for( unsigned int i = 0 ; smi[i].stringA ; ++i ) {
					if( smi[i].iconID != ICON_NOT_USED )
						bitmaps[smi[i].iconID] = clx::shared_ptr<icon::Icon>( new icon::Icon( hInstance, smi[i].iconID ) );
					if( smi[i].submenu )
						InitIcon( smi[i].submenu );
				}
			}
		};
	}
}

#endif	// CUBEICE_CTXVISTA_H
