/* ------------------------------------------------------------------------- */
/*
 *  cubeice-ctxbase.h
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
#ifndef	CUBEICE_CTXBASE_H
#define	CUBEICE_CTXBASE_H

#include <tchar.h>
#include <string>
#include <vector>
#include <clx/date_time.h>
#include <clx/tokenizer.h>
#include "format.h"
#include "wpopen.h"

#define CUBEICE_ENGINE _T("cubeice-exec.exe")
#define CUBEICE_MAXFILESIZE (1024 * 1024 * 1024)

extern HINSTANCE	hDllInstance;

namespace cube {
	namespace shlctxmenu {
		/* ----------------------------------------------------------------- */
		//  CShlCtxMenuBase
		/* ----------------------------------------------------------------- */
		class CShlCtxMenuBase : public IContextMenu3, IShellExtInit, IQueryInfo, IPersistFile, IMenuInfo {
		private:
			typedef TCHAR char_type;
			typedef std::basic_string<TCHAR> string_type;
			typedef std::size_t size_type;

			struct fileinfo {
			public:
				string_type name;
				size_type size;
				clx::date_time time;
				bool directory;
				
				fileinfo() : name(), size(0), time(), directory(false) {}
			};

		public:
			/* ------------------------------------------------------------- */
			//  constructor
			/* ------------------------------------------------------------- */
			CShlCtxMenuBase( ULONG &dllrc ) :
				ctxSetting(), refCount( 1UL ), hInstance( hDllInstance ), dllRefCount( dllrc ),
				fileNum( 0 ), folderNum( 0 ) {
				InterlockedIncrement( reinterpret_cast<LONG*>(&dllRefCount) );
				
				TCHAR	path[4096];
				
				GetModuleFileName( hDllInstance, path, sizeof( path ) );
				PathRemoveFileSpec( path );
				cubeiceEnginePath = path;
				cubeiceEnginePath += _T( "\\" ) CUBEICE_ENGINE;
			}
			
			/* ------------------------------------------------------------- */
			//  destructor
			/* ------------------------------------------------------------- */
			virtual ~CShlCtxMenuBase() {
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
				
				if( IsEqualIID( riid, IID_IUnknown ) || IsEqualIID( riid, IID_IContextMenu ) )
					*ppv = static_cast<LPCONTEXTMENU>( this );
				else if( IsEqualIID( riid, IID_IShellExtInit ) )
					*ppv = static_cast<LPSHELLEXTINIT>( this );
				else if( IsEqualIID( riid, IID_IQueryInfo ) )
					*ppv = static_cast<IQueryInfo*>( this );
				else if( IsEqualIID( riid, IID_IPersistFile ) )
					*ppv = static_cast<LPPERSISTFILE>( this );
				else
					return E_NOINTERFACE;
				
				AddRef();
				
				return NOERROR;
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
			 *  QueryContextMenu
			 *
			 *  IContextMenu members
			 */
			/* ------------------------------------------------------------- */
			STDMETHODIMP QueryContextMenu( HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags ) {
				UINT						idCmd = idCmdFirst;
				
				if( uFlags & CMF_DEFAULTONLY )
					return NO_ERROR;
				
				if( ( !ctxSetting.context_customize() && !ctxSetting.context_flags() ) || ( ctxSetting.context_customize() && ctxSetting.context_submenu().size() == 0 ) )
					return NO_ERROR;
				
				// Add separator
				if( !isDragAndDrop )
					InsertMenu( hMenu, indexMenu++, MF_BYPOSITION | MF_SEPARATOR, 0, NULL );
				
				if( !ctxSetting.context_customize() ) {

					for( unsigned int i = 0 ; MenuItem[i].stringA ; ++i ) {
						if( MenuItem[i].dispSetting && !this->is_visible( MenuItem[i].dispSetting ) )
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
						
						if( MenuItem[i].iconID != ICON_NOT_USED )
							SetMenuIcon( MenuItem[i].iconID, miinfo );
						
						InsertMenuItem( hMenu, indexMenu++, TRUE, &miinfo );
					}
				} else {
					std::vector<const SUB_MENU_ITEM*>		v;
					const std::vector<cubeice::user_setting::SUBMENU>		&submenu = ctxSetting.context_submenu();

					SerSubMenu( v, MenuItem );

					ConstructSubMenu( hMenu, v, submenu, idCmd, idCmdFirst, indexMenu );
				}
					
				// Add separator
				if( idCmd - idCmdFirst )
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
			
			/* ------------------------------------------------------------- */
			//  InvokeCommand
			/* ------------------------------------------------------------- */
			STDMETHODIMP InvokeCommand( LPCMINVOKECOMMANDINFO lpcmi ) {
				if( HIWORD( lpcmi->lpVerb ) )
					return E_INVALIDARG;
				
				DWORD	index = 0;
				
				if( !ctxSetting.context_customize() )
					RecursiveInvokeCommand( MenuItem, lpcmi, index );
				else
					MenuSelectedCallback( table[LOWORD(lpcmi->lpVerb)]->arg, this );
				
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
			
			/* ------------------------------------------------------------- */
			/*
			 *  HandleMenuMsg
			 *
			 *  IContextMenu2 members
			 */
			/* ------------------------------------------------------------- */
			STDMETHODIMP HandleMenuMsg( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
				return S_OK;
			}
			
			/* ------------------------------------------------------------- */
			/* 
			 *  HandleMenuMsg2
			 *
			 *  IContextMenu3 members
			 */
			/* ------------------------------------------------------------- */
			STDMETHODIMP HandleMenuMsg2( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *plResult ) {
				return S_OK;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  Initialize
			 *
			 *  IShellExtInit methods
			 */
			/* ------------------------------------------------------------- */
			STDMETHODIMP Initialize( LPCITEMIDLIST pIDFolder, LPDATAOBJECT pDataObj, HKEY hKeyID ) {
				HRESULT			hr;
				FORMATETC		fme;
				STGMEDIUM		stgm;
				HDROP			hDrop;
				UINT			numoffiles;
				
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

				if( pIDFolder ) {
					isDragAndDrop = true;
					TCHAR buffer[MAX_PATH] = {};
					::SHGetPathFromIDList(pIDFolder, buffer);
					this->dropPath = buffer;
				}
				else isDragAndDrop = false;

				return S_OK;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  GetInfoFlags
			 *
			 *  IQueryInfo members
			 */
			/* ------------------------------------------------------------- */
			STDMETHODIMP GetInfoFlags( DWORD * )
			{
				return E_NOTIMPL;
			}

			/* ------------------------------------------------------------- */
			//  GetInfoTip
			/* ------------------------------------------------------------- */
			STDMETHODIMP GetInfoTip( DWORD dwFlags, WCHAR **ppwszTip ) {
				static const size_type maxcolumn = 50; // 1行に出力する最大文字数
				tstring		tooltip;
				TCHAR		*ext;
				TCHAR		tmp[512];

				tooltip = _T( "種類: " );
				ext = PathFindExtension( compFileName.c_str() );
				if( ext ) {
					++ext;
					tooltip += ext;
				}
				tooltip += _T( " ファイル\r\n" );
				
				HANDLE			hFile = CreateFile( compFileName.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
				LARGE_INTEGER	li = { 0 };
				SYSTEMTIME		st = { 0 };
				if( hFile != INVALID_HANDLE_VALUE ) {
					FILETIME		ft, localft;
					
					GetFileSizeEx( hFile, &li );
					GetFileTime( hFile, NULL, NULL, &ft );
					
					FileTimeToLocalFileTime(&ft, &localft);
					FileTimeToSystemTime( &localft, &st );
					CloseHandle( hFile );
				}
				
				tooltip += _T( "更新日時: " );
				wsprintf( tmp, _T( "%d/%02d/%02d %d:%02d" ), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute );
				tooltip += tmp;
				
				tooltip += _T("\r\n");
				
				tooltip += _T( "サイズ: " );
				tooltip += cubeice::punct( li.QuadPart );
				tooltip += _T( " バイト" );
				
				if( dwFlags & QITIPF_USESLOWTIP &&
					(ctxSetting.decompression().details() & DETAIL_TOOLTIP) &&
					ctxSetting.decompression().max_filelist() > 0 &&
					compFileList.size() > 0)
				{
					tooltip += _T( "\r\n" );
					tooltip += _T( "ファイルリスト" );
					
					for( size_t i = 0 ; i < compFileList.size() ; ++i ) {
						if (i >= ctxSetting.decompression().max_filelist()) break;
						tooltip += _T( "\r\n" );
						string_type name = compFileList[i].name;
						if (name.size() > maxcolumn) {
							name = name.substr(name.size() - maxcolumn + 3, maxcolumn - 3);
							string_type::size_type pos = name.find_first_of(_T('\\'));
							if (pos != string_type::npos) name = _T("...") + name.substr(pos);
							else name = _T("...") + name;
						}
						tooltip += _T( "  " ) + name;
					}
					
					tooltip += _T("\r\n");
					if (compFileList.size() > ctxSetting.decompression().max_filelist()) tooltip += _T("ほか、");
					tooltip += _T("全 ") + clx::lexical_cast<string_type>(fileNum) + _T(" ファイル ");
					tooltip += clx::lexical_cast<string_type>(folderNum) + _T(" フォルダ");
				}

				*ppwszTip = static_cast<wchar_t*>( CoTaskMemAlloc( ( tooltip.size() + 5 ) * sizeof( wchar_t ) ) );
#ifdef	UNICODE
				lstrcpyW( *ppwszTip, tooltip.c_str() );
#else	// UNICODE
				MultiByteToWideChar( CP_OEMCP, 0, tooltip.c_str(), -1, *ppwszTip, ( tooltip.size() + 5 ) * sizeof( wchar_t ) );
#endif

				return S_OK;
			}

			/* ------------------------------------------------------------- */
			/*
			 *  GetClassID
			 *
			 *  IPersistFile members
			 */
			/* ------------------------------------------------------------- */
			STDMETHODIMP GetClassID(CLSID *pClassID) {
				return E_NOTIMPL;
			}

			/* ------------------------------------------------------------- */
			//  IsDirty
			/* ------------------------------------------------------------- */
			STDMETHODIMP IsDirty() {
				return E_NOTIMPL;
			}

			/* ------------------------------------------------------------- */
			//  Load
			/* ------------------------------------------------------------- */
			STDMETHODIMP Load(LPCOLESTR pszFileName, DWORD dwMode) {
				compFileList.clear();
#ifdef	UNICODE
				const wchar_t	*fname = pszFileName;
#else	// UNICODE
				char	fname[4096];
				WideCharToMultiByte( CP_OEMCP, 0, pszFileName, -1, fname, sizeof( fname ), NULL, NULL );
#endif	// UNICODE
				compFileName = pszFileName;
				HANDLE hFile = CreateFile( compFileName.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
				if( hFile != INVALID_HANDLE_VALUE ) {
					LARGE_INTEGER li = {};
					GetFileSizeEx( hFile, &li );
					CloseHandle( hFile );
					if (li.QuadPart < CUBEICE_MAXFILESIZE) decompress_filelist( fname, compFileList );
				}
				return S_OK;
			}

			/* ------------------------------------------------------------- */
			//  Save
			/* ------------------------------------------------------------- */
			STDMETHODIMP Save(LPCOLESTR pszFileName, BOOL fRemember) {
				return E_NOTIMPL;
			}

			/* ------------------------------------------------------------- */
			//  SaveCompleted
			/* ------------------------------------------------------------- */
			STDMETHODIMP SaveCompleted(LPCOLESTR pszFileName) {
				return E_NOTIMPL;
			}

			/* ------------------------------------------------------------- */
			//  GetCurFile
			/* ------------------------------------------------------------- */
			STDMETHODIMP GetCurFile(LPOLESTR *ppszFileName) {
				return E_NOTIMPL;
			}

			/* ------------------------------------------------------------- */
			/*
			 *  GetFileList
			 *
			 *  IMenuInfo members
			*/
			/* ------------------------------------------------------------- */
			const std::vector<tstring> &GetFileList() {
				return fileList;
			}
			
			/* ------------------------------------------------------------- */
			//  GetFilePath
			/* ------------------------------------------------------------- */
			const tstring &GetFilePath( const std::size_t &index ) {
				return fileList.at( index );
			}
			
			/* ------------------------------------------------------------- */
			//  GetSize
			/* ------------------------------------------------------------- */
			std::size_t GetSize() {
				return fileList.size();
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  GetOption
			 *
			 *  NOTE: 拡張する必要が出た場合，保持しているメンバ変数等から
			 *  cubeice.exe へ送るためのオプション用コマンドライン文字列を
			 *  生成する．
			 */
			/* ------------------------------------------------------------- */
			tstring GetOption() {
				tstring dest = _T(" ");
				if (this->isDragAndDrop && !this->dropPath.empty()) {
					dest += _T('"');
					dest += _T("/drop:") + this->dropPath;
					dest += _T('"');
				}
				dest += _T(" ");

				return dest;
			}

		protected:
			HINSTANCE	hInstance;
			virtual void SetMenuIcon( WORD iconID, MENUITEMINFO &miinfo ) = 0;
			
		private:
			/* ------------------------------------------------------------- */
			//  CreateSubMenu
			/* ------------------------------------------------------------- */
			HMENU CreateSubMenu( const SUB_MENU_ITEM *smi, UINT &idCmd ) {
				HMENU		hMenu = CreateMenu();
				
				for( unsigned int i = 0 ; smi[i].stringA ; ++i ) {
					MENUITEMINFO		miinfo;
					
					if( smi[i].dispSetting && !this->is_visible( smi[i].dispSetting ) )
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
					if( smi[i].iconID != ICON_NOT_USED )
						SetMenuIcon( smi[i].iconID, miinfo );
					
					InsertMenuItem( hMenu, i, TRUE, &miinfo );
				}
				
				return hMenu;
			}
			
			/* ------------------------------------------------------------- */
			//  RecursiveInvokeCommand
			/* ------------------------------------------------------------- */
			void RecursiveInvokeCommand( const SUB_MENU_ITEM *smi, LPCMINVOKECOMMANDINFO lpcmi, DWORD &index ) {
				for( unsigned int i = 0 ; smi[i].stringA && index <= LOWORD( lpcmi->lpVerb ) ; ++i ) {
					if( smi[i].dispSetting && !this->is_visible( smi[i].dispSetting ) )
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
			
			/* ------------------------------------------------------------- */
			//  RecursiveGetCommandString
			/* ------------------------------------------------------------- */
			void RecursiveGetCommandString( const SUB_MENU_ITEM *smi, unsigned int &index, UINT_PTR idCmd, UINT uFlags, LPSTR pszName, UINT cchMax ) {
				for( unsigned int i = 0 ; smi[i].stringA && index <= idCmd ; ++i ) {
					if( smi[i].dispSetting && !this->is_visible( smi[i].dispSetting ) )
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
			
			/* ------------------------------------------------------------- */
			//  is_visible
			/* ------------------------------------------------------------- */
			bool is_visible(std::size_t flag) {
				bool dest = (ctxSetting.context_flags() & flag) != 0;
				switch (flag) {
				case COMPRESS_FLAG:
					dest &= (ctxSetting.context_flags() & COMP_ALL_FLAG) != 0;
					break;
				case DECOMPRESS_FLAG:
					dest &= (ctxSetting.context_flags() & DECOMP_ALL_FLAG) != 0;
					break;
				case MAIL_FLAG:
					dest &= (ctxSetting.context_flags() & MAIL_ALL_FLAG) != 0;
					break;
				default:
					break;
				}
				return dest;
			}
			
			/* ------------------------------------------------------------- */
			/*
			 *  is_decompress
			 *
			 *  NOTE: 7zip はフォルダを指定すると中身を再帰的にチェックして
			 *  しまう模様．そのため，フォルダかどうかを最初にチェックして
			 *  フォルダの場合は false を返す．
			 */
			/* ------------------------------------------------------------- */
			bool is_decompress(const string_type& path) {
				if (PathIsDirectory(path.c_str())) return false;
				
				string_type cmdline = cubeiceEnginePath;
				cmdline += _T(" l ");
				cmdline += _T("\"") + path + _T("\"");
				
				cube::popen proc;
				if (!proc.open(cmdline.c_str(), _T("r"))) return false;
				string_type buffer;
				int status = 0;
				while ((status = proc.gets(buffer)) >= 0) {
					if (status == 2) break; // pipe closed
					else if (status == 0 || buffer.empty()) continue;
					
					clx::escape_separator<TCHAR> sep(_T(" \t"), _T("\""), _T(""));
					clx::basic_tokenizer<clx::escape_separator<TCHAR>, std::basic_string<TCHAR> > v(buffer, sep);
					
					buffer.clear();
					if (v.size() >= 3 && v.at(0) == _T("Type") && v.at(1) == _T("=")) {
						// 対象外のファイルタイプを記述する．
						if (v.at(2) == _T("Compound")) continue;
						
						proc.close();
						return true;
					}
				}
				return false;
			}
			
			/* ------------------------------------------------------------- */
			//  is_decompress
			/* ------------------------------------------------------------- */
			template <class InputIterator>
			bool is_decompress(InputIterator first, InputIterator last) {
				for (; first != last; ++first) {
					if (is_decompress(*first)) return true;
				}
				return false;
			}
			
			/* ----------------------------------------------------------------- */
			//  decompress_filelist
			/* ----------------------------------------------------------------- */
			void decompress_filelist(const string_type& path, std::vector<fileinfo> &flist) {
				string_type cmdline = cubeiceEnginePath;
				cmdline += _T(" l -scsIGNORE ");
				cmdline += _T("\"") + path + _T("\"");
				
				flist.clear();
				
				cube::popen proc;
				if (!proc.open(cmdline.c_str(), _T("r"))) return;
				string_type buffer, src;
				int status = 0;
				while ((status = proc.gets(buffer)) >= 0) {
					if (status == 2) break; // pipe closed
					else if (status == 1 && !buffer.empty()) src = buffer;
					
					clx::escape_separator<TCHAR> sep(_T(" \t"), _T("\""), _T(""));
					clx::basic_tokenizer<clx::escape_separator<TCHAR>, std::basic_string<TCHAR> > v(buffer, sep);
					
					if (buffer.find(_T("files")) != string_type::npos && buffer.find(_T("folders")) != string_type::npos && v.size() == 6) {
						try {
							fileNum = clx::lexical_cast<size_type>(v.at(2));
							folderNum = clx::lexical_cast<size_type>(v.at(4));
						}
						catch (clx::bad_lexical_cast&) {}
					}
					
					if (v.empty() || v.at(0) != _T("<>")) {
						buffer.clear();
						continue;
					}
					
					if (v.size() == 6) {
						// ファイルリストの更新
						fileinfo elem;
						elem.name = v.at(5);
						elem.size = v.at(3) != _T("-") ? clx::lexical_cast<std::size_t>(v.at(3)) : 0;
						if (v.at(1) != _T("-")) elem.time.from_string(v.at(1), string_type(_T("%Y-%m-d %H:%M:%S")));
						elem.directory = (v.at(2).find(_T('D')) != string_type::npos);
						flist.push_back( elem );
					}
					buffer.clear();
				}
				return;
			}

			void SerSubMenu( std::vector<const SUB_MENU_ITEM*> &v, const SUB_MENU_ITEM *smi ) {
				for( int i = 0 ; smi[i].stringA ; ++i ) {
					v.push_back( &smi[i] );
					if( smi[i].submenu )
						SerSubMenu( v, smi[i].submenu );
				}
			}

			const SUB_MENU_ITEM *GetSubMenuItem( const std::vector<const SUB_MENU_ITEM*> &v, const cubeice::user_setting::size_type &id ) {
				BOOST_FOREACH(const SUB_MENU_ITEM *smi, v) {
					if( smi->dispSetting == id )
						return smi;
				}
				return NULL;
			}

			/* ----------------------------------------------------------------- */
			//  IsLeaf
			/* ----------------------------------------------------------------- */
			bool IsLeaf(int id) {
				for (int i = 0; SubMenuCompress[i].stringA; ++i) {
					if (id == SubMenuCompress[i].dispSetting) return true;
				}
				
				for (int i = 0; SubMenuCompAndMail[i].stringA; ++i) {
					if (id == SubMenuCompAndMail[i].dispSetting) return true;
				}

				for (int i = 0; SubMenuDecompress[i].stringA; ++i) {
					if (id == SubMenuDecompress[i].dispSetting) return true;
				}
				return false;
			}

			/* ----------------------------------------------------------------- */
			//  CheckValidation
			/* ----------------------------------------------------------------- */
			bool CheckValidation(const std::vector<cubeice::user_setting::SUBMENU> &v)
			{
				if(!v.size())
					return false;
				BOOST_FOREACH(const cubeice::user_setting::SUBMENU &s, v) {
					bool	f = false;
					if(s.id == 0) {
						f = true;
					} else {
						for(int i = 0 ; MenuItem[i].stringA ; ++i) {
							if(MenuItem[i].dispSetting == s.id) {
								f = true;
								break;
							}
						}
					}
					if(f && !CheckValidation(s.children))
						return false;
				}
				return true;
			}
			
			HMENU ConstructSubMenu( HMENU hMenu, const std::vector<const SUB_MENU_ITEM*> &v, const std::vector<cubeice::user_setting::SUBMENU> &submenu, UINT &idCmd, const UINT &idCmdFirst, UINT &indexMenu ) {
				BOOST_FOREACH(const cubeice::user_setting::SUBMENU &s, submenu) {
					MENUITEMINFO			miinfo;
					const SUB_MENU_ITEM		*smi = GetSubMenuItem( v, s.id );

					//if( !smi && ( s.id != 0 || !s.children.size() ) ) continue;
					if (!IsLeaf(s.id) && !CheckValidation(s.children)) continue;

					table[idCmd-idCmdFirst] = smi;

					ZeroMemory( &miinfo, sizeof( miinfo ) );
					miinfo.cbSize			= sizeof( miinfo );
					miinfo.fMask			= MIIM_FTYPE | MIIM_STRING | MIIM_ID;
					miinfo.fType			= MFT_STRING;
					miinfo.wID				= idCmd++;
#ifndef	UNICODE
					miinfo.dwTypeData		= const_cast<LPSTR>( s.str.c_str() );
#else	// UNICODE
					miinfo.dwTypeData		= const_cast<LPWSTR>( s.str.c_str() );
#endif	// UNICODE
					
					if( s.children.size() ) {
						UINT		tmp = 0;

						miinfo.fMask		|= MIIM_SUBMENU;
						miinfo.hSubMenu		= ConstructSubMenu( CreateMenu(), v, s.children, idCmd, idCmdFirst, tmp );
					}
					
					if( s.id != 0 && smi->iconID != ICON_NOT_USED )
						SetMenuIcon( smi->iconID, miinfo );
					
					InsertMenuItem( hMenu, indexMenu++, TRUE, &miinfo );
				}
				return hMenu;
			}

			std::vector<tstring>				fileList;
			cubeice::user_setting				ctxSetting;
			ULONG								refCount;
			ULONG								&dllRefCount;
			tstring								compFileName;
			std::vector<fileinfo>				compFileList;
			size_type							fileNum;
			size_type							folderNum;
			tstring								cubeiceEnginePath;
			bool								isDragAndDrop;
			std::map<UINT, const SUB_MENU_ITEM*>		table;
			tstring								dropPath;
		};

	}
}

#endif	// CUBEICE_CTXBASE_H
