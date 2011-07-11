/* ------------------------------------------------------------------------- */
/*
 *  customize-dialog.cpp
 *
 *  Copyright (c) 2010 CubeSoft Inc.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see < http://www.gnu.org/licenses/ >.
 *
 *  Last-modified: Tue 28 Dec 2010 20:42:00 JST
 */
/* ------------------------------------------------------------------------- */
#include "dialog.h"
#include <Windows.h>
#include <CommCtrl.h>
#include <imm.h>
#include <vector>
#include <map>
#include "../cubeice-ctx/cubeice-ctxdata.h"

#pragma comment(lib, "imm32.lib")

#define		SUBMENU_ROOT_ID		static_cast<LPARAM>(-1)
#define		SUBMENU_DIR_ID		static_cast<LPARAM>(0)

namespace cubeice {
	namespace dialog {
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
		/*
		 *  IsChild
		 *
		 *  m1 が m2 の子要素かどうかをチェックする．
		 */
		/* ----------------------------------------------------------------- */
		bool IsChild(HWND hTreeMenu, HTREEITEM m1, HTREEITEM m2) {
			HTREEITEM hChildItem = TreeView_GetChild(hTreeMenu, m2);
			while(hChildItem) {
				if (m1 == hChildItem || IsChild(hTreeMenu, m1, hChildItem)) return true;
				hChildItem = TreeView_GetNextSibling(hTreeMenu, hChildItem);
			}
			return false;
		}

		/* ----------------------------------------------------------------- */
		//  GetItemImageIndex
		/* ----------------------------------------------------------------- */
		int GetItemImageIndex(int id) {
			for (int i = 0; SubMenuCompress[i].stringA; ++i) {
				if (id == SubMenuCompress[i].dispSetting) return 0;
			}
			
			for (int i = 0; SubMenuCompAndMail[i].stringA; ++i) {
				if (id == SubMenuCompAndMail[i].dispSetting) return 0;
			}

			for (int i = 0; SubMenuDecompress[i].stringA; ++i) {
				if (id == SubMenuDecompress[i].dispSetting) return 1;
			}
			return 2;
		}

		/* ----------------------------------------------------------------- */
		//  GetSelectedItemImageIndex
		/* ----------------------------------------------------------------- */
		int GetSelectedItemImageIndex(int id) {
			for (int i = 0; SubMenuCompress[i].stringA; ++i) {
				if (id == SubMenuCompress[i].dispSetting) return 0;
			}
			
			for (int i = 0; SubMenuCompAndMail[i].stringA; ++i) {
				if (id == SubMenuCompAndMail[i].dispSetting) return 0;
			}

			for (int i = 0; SubMenuDecompress[i].stringA; ++i) {
				if (id == SubMenuDecompress[i].dispSetting) return 1;
			}
			return 3;
		}

		/* ----------------------------------------------------------------- */
		//  GetAlternativeString
		/* ----------------------------------------------------------------- */
		TCHAR* GetAlternativeString(int id) {
			for (int i = 0; SubMenuCompress[i].stringA; ++i) {
#ifdef UNICODE
				if (id == SubMenuCompress[i].dispSetting) return const_cast<TCHAR*>(SubMenuCompress[i].explanationW);
#else
				if (id == SubMenuCompress[i].dispSetting) return const_cast<TCHAR*>(SubMenuCompress[i].explanationA);
#endif
			}
			
			for (int i = 0; SubMenuCompAndMail[i].stringA; ++i) {
#ifdef UNICODE
				if (id == SubMenuCompAndMail[i].dispSetting) return const_cast<TCHAR*>(SubMenuCompAndMail[i].explanationW);
#else
				if (id == SubMenuCompAndMail[i].dispSetting) return const_cast<TCHAR*>(SubMenuCompAndMail[i].explanationA);
#endif
			}

			for (int i = 0; SubMenuDecompress[i].stringA; ++i) {
#ifdef UNICODE
				if (id == SubMenuDecompress[i].dispSetting) return const_cast<TCHAR*>(SubMenuDecompress[i].explanationW);
#else
				if (id == SubMenuDecompress[i].dispSetting) return const_cast<TCHAR*>(SubMenuDecompress[i].explanationA);
#endif
			}

			return NULL;
		}

		/* ----------------------------------------------------------------- */
		//  GetTreeViewItem
		/* ----------------------------------------------------------------- */
		TV_ITEM GetTreeViewItem(HWND handle, HTREEITEM item) {
			static TCHAR buffer[1024] = {};
			TV_ITEM dest = {};
			dest.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			dest.hItem = item;
			dest.pszText = buffer;
			dest.cchTextMax = 1024;
			TreeView_GetItem(handle, &dest);
			return dest;
		}
		
		/* ----------------------------------------------------------------- */
		//  InitMenuList
		/* ----------------------------------------------------------------- */
		void InitMenuList(HWND hTreeView, HTREEITEM hParent, const SUB_MENU_ITEM *smi)
		{
			TVINSERTSTRUCT	tvi;

			tvi.hParent			= hParent;
			tvi.hInsertAfter	= TVI_LAST;
			tvi.item.mask		= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

			for(int i = 0 ; smi[i].stringA ; ++i) {
				HTREEITEM	hTreeItem;

#ifdef	UNICODE
				tvi.item.pszText	= const_cast<LPWSTR>(smi[i].stringW);
#else
				tvi.item.pszText	= const_cast<LPSTR>(smi[i].stringA);
#endif
				tvi.item.lParam		= smi[i].dispSetting;
				tvi.item.iImage		= GetItemImageIndex(smi[i].dispSetting);
				tvi.item.iSelectedImage = GetSelectedItemImageIndex(smi[i].dispSetting);

				hTreeItem = TreeView_InsertItem(hTreeView, &tvi);
				if(smi[i].submenu) InitMenuList(hTreeView, hTreeItem, smi[i].submenu);
			}

		}

		/* ----------------------------------------------------------------- */
		//  SerializeMenuItemData
		/* ----------------------------------------------------------------- */
		void SerializeMenuItemData(std::map<cubeice::user_setting::size_type, const TCHAR*> &table, const SUB_MENU_ITEM *smi)
		{
			for(int i = 0 ; smi[i].stringA ; ++i) {
#ifdef	UNICODE
				table[smi[i].dispSetting] = smi[i].stringW;
#else
				table[smi[i].dispSetting] = smi[i].stringA;
#endif
				if(smi[i].submenu) SerializeMenuItemData(table, smi[i].submenu);
			}
		}

		/* ----------------------------------------------------------------- */
		//  SetMenuItem
		/* ----------------------------------------------------------------- */
		void SetMenuItem(HWND hMenuTreeView, HTREEITEM hParentItem, const std::vector<cubeice::user_setting::SUBMENU> &submenu, const std::map<cubeice::user_setting::size_type, const TCHAR*> &table)
		{
			TVINSERTSTRUCT	tvi;
			tvi.hParent			= hParentItem;
			tvi.hInsertAfter	= TVI_LAST;
			tvi.item.mask		= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

			BOOST_FOREACH(const cubeice::user_setting::SUBMENU &s, submenu) {
				if(s.id == SUBMENU_DIR_ID || table.find(s.id) != table.end()) {
					HTREEITEM					hItem;

					// NOTE: 規定の項目もユーザがリネーム可能に変更したため，
					// どのような場合でも SUBMENU に保存されている文字列を使用する．
					// tvi.item.pszText	= (s.id == SUBMENU_DIR_ID) ? const_cast<LPTSTR>(s.str.c_str()) : const_cast<LPTSTR>(table.find(s.id)->second);
					tvi.item.pszText = const_cast<LPTSTR>(s.str.c_str());
					tvi.item.lParam		= s.id;
					tvi.item.iImage = GetItemImageIndex(s.id);
					tvi.item.iSelectedImage = GetSelectedItemImageIndex(s.id);
					hItem = TreeView_InsertItem(hMenuTreeView, &tvi);

					SetMenuItem(hMenuTreeView, hItem, s.children, table);
				}
			}
		}

		/* ----------------------------------------------------------------- */
		//  InitTreeViewItem
		/* ----------------------------------------------------------------- */
		void InitTreeViewItem(HWND hMenuTreeView, HWND hItemListTreeView, const std::vector<cubeice::user_setting::SUBMENU> &submenu)
		{
			HTREEITEM	hRootItem;

			InitMenuList(hItemListTreeView, TVI_ROOT, MenuItem);
			TreeView_Select(hItemListTreeView, TreeView_GetRoot(hItemListTreeView), TVGN_CARET);

			TVINSERTSTRUCT	tvi;
			tvi.hParent			= TVI_ROOT;
			tvi.hInsertAfter	= TVI_LAST;
			tvi.item.mask		= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			tvi.item.pszText	= TEXT("メニュートップ");
			tvi.item.lParam		= SUBMENU_ROOT_ID;
			tvi.item.iImage = 4;
			tvi.item.iSelectedImage = 4;
			hRootItem = TreeView_InsertItem(hMenuTreeView, &tvi);

			std::map<cubeice::user_setting::size_type, const TCHAR*>	table;
			SerializeMenuItemData(table, MenuItem);
			SetMenuItem(hMenuTreeView, hRootItem, submenu, table);

			TreeView_Select(hMenuTreeView, hRootItem, TVGN_CARET);
			TreeView_Expand(hMenuTreeView, hRootItem, TVE_EXPAND);
		}
		
		/* ----------------------------------------------------------------- */
		//  CopyTreeViewItem
		/* ----------------------------------------------------------------- */
		HTREEITEM CopyTreeViewItem(HWND &hTo, HTREEITEM hToParentItem, HWND &hFrom, HTREEITEM hFromParentItem, const HTREEITEM &InsertAfter = TVI_LAST, bool use_altname = false)
		{
			static	TCHAR	str[2048];
			TVINSERTSTRUCT	tvi;
			HTREEITEM	hInsertItem, hChildItem;

			tvi.hParent			= hToParentItem;
			tvi.hInsertAfter	= InsertAfter;
			tvi.item.mask		= TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			tvi.item.hItem		= hFromParentItem;
			tvi.item.pszText	= str;
			tvi.item.cchTextMax	= sizeof(str) / sizeof(str[0]);

			TreeView_GetItem(hFrom, &tvi.item);
			tvi.item.mask		= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

			// ルート直下にリーフ要素を追加する場合は，表示する文字列を変える．
			if (use_altname && hToParentItem == TreeView_GetRoot(hTo)) {
				TCHAR* alt = GetAlternativeString(static_cast<int>(tvi.item.lParam));
				if (alt) tvi.item.pszText = alt;
			}
			
			hInsertItem = TreeView_InsertItem(hTo, &tvi);
			hChildItem = TreeView_GetChild(hFrom, hFromParentItem);
			while(hChildItem) {
				CopyTreeViewItem(hTo, hInsertItem, hFrom, hChildItem);
				hChildItem = TreeView_GetNextSibling(hFrom, hChildItem);
			}
			return hInsertItem;
		}

		/* ----------------------------------------------------------------- */
		//  GetSubmenuStruct
		/* ----------------------------------------------------------------- */
		void GetSubmenuStruct(std::vector<cubeice::user_setting::SUBMENU> &submenu, HWND hMenuTreeView, HTREEITEM hParentItem)
		{
			HTREEITEM	hChildItem;

			submenu.clear();
			hChildItem = TreeView_GetChild(hMenuTreeView, hParentItem);
			while(hChildItem) {
				static	TCHAR	str[2048];
				TVITEM	tvi;
				tvi.mask		= TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
				tvi.hItem		= hChildItem;
				tvi.pszText		= str;
				tvi.cchTextMax	= sizeof(str) / sizeof(str[0]);

				TreeView_GetItem(hMenuTreeView, &tvi);
				submenu.push_back(cubeice::user_setting::SUBMENU(static_cast<int>(tvi.lParam), str));

				GetSubmenuStruct(submenu.back().children, hMenuTreeView, hChildItem);
				hChildItem = TreeView_GetNextSibling(hMenuTreeView, hChildItem);
			}
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
				if(s.id == SUBMENU_DIR_ID) {
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
		
		/* ----------------------------------------------------------------- */
		/*
		 *  TreeViewProc
		 *
		 *  TreeView 独自の処理を行うためにサブクラス化する．
		 */
		/* ----------------------------------------------------------------- */
		static WNDPROC DefaultTreeViewProc;
		LRESULT CALLBACK TreeViewProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			HWND dialog = GetParent(hWnd);
			switch(msg){
			case WM_KEYDOWN:
				switch(wParam){
					case VK_DELETE:
						SendMessage(dialog, WM_COMMAND, MAKEWPARAM(IDC_DELETE_BUTTON, BN_CLICKED), lParam);
						break;
					case VK_F2:
						SendMessage(dialog, WM_COMMAND, MAKEWPARAM(IDC_RENAME_BUTTON, BN_CLICKED), lParam);
						break;
					default:
						break;
				}
				break;
			}
			return  CallWindowProc((WNDPROC)DefaultTreeViewProc, hWnd, msg, wParam, lParam);
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  EditProc
		 *
		 *  TreeView のラベルの編集中に Enter キーなどを押すと，他のボタン
		 *  などにコントロールを奪われてしまい意図した挙動にならない．
		 *  そのため，ラベルの編集が始まった段階でサブクラス化する．
		 */
		/* ----------------------------------------------------------------- */
		static WNDPROC DefaultEditProc;
		static LRESULT CALLBACK EditProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
			switch(msg){
			case WM_GETDLGCODE:
				return DLGC_WANTALLKEYS;
			}
			return CallWindowProc((WNDPROC)DefaultEditProc, hWnd, msg, wParam, lParam);
		}
		
		/* ----------------------------------------------------------------- */
		//  BeginDrag
		/* ----------------------------------------------------------------- */
		static BOOL bDragging;
		static std::pair<HWND, HTREEITEM> hDragging;
		static void BeginDrag(LPNMTREEVIEW p_nmtree) {
			hDragging.first = p_nmtree->hdr.hwndFrom;
			hDragging.second = p_nmtree->itemNew.hItem;
			TreeView_SelectItem(hDragging.first, hDragging.second);
			
			HIMAGELIST hDragImage = TreeView_CreateDragImage(hDragging.first, hDragging.second) ;
			ImageList_BeginDrag(hDragImage, 0, 0, 0) ;
			SetCapture(GetParent(hDragging.first)) ;
			bDragging = TRUE ;
			ClientToScreen(hDragging.first, &(p_nmtree->ptDrag)) ;
			ImageList_DragEnter(NULL, p_nmtree->ptDrag.x, p_nmtree->ptDrag.y) ;
		}
		
		/* ----------------------------------------------------------------- */
		//  MouseMove
		/* ----------------------------------------------------------------- */
		static void MouseMove(HWND hWnd, HWND hTreeMenu, HWND hTreeOrg) {
			//ImageList_DragLeave(NULL);
			
			POINT pt;
			TVHITTESTINFO hittest = {};
			
			GetCursorPos(&pt);
			
			hittest.flags = TVHT_ONITEM;
			hittest.pt.x = pt.x;
			hittest.pt.y = pt.y;
			ScreenToClient(hTreeMenu, &hittest.pt);
			
			ImageList_DragShowNolock(FALSE);

			TreeView_SelectDropTarget(hTreeMenu, NULL);
			TreeView_SetInsertMark(hTreeMenu, NULL, FALSE);
			hittest.hItem = TreeView_HitTest(hTreeMenu, &hittest);
			if (hittest.hItem &&  hDragging.second != TreeView_GetRoot(hTreeMenu) && !IsChild(hTreeMenu, hittest.hItem, hDragging.second) && (hittest.flags & (TVHT_ONITEM|TVHT_ONITEMRIGHT))) {
				TVITEM		tvitem = GetTreeViewItem(hTreeMenu, hittest.hItem);
				HTREEITEM	hOverItem = hittest.hItem;
				HTREEITEM	hChild = TreeView_GetChild(hTreeMenu, hOverItem);
				hittest.pt.y -= TreeView_GetItemHeight(hTreeMenu) / 2;
				TreeView_HitTest(hTreeMenu, &hittest);
				if ((hOverItem == TreeView_GetRoot(hTreeMenu) && (!(TreeView_GetItemState(hTreeMenu, hOverItem, TVIS_EXPANDED) & TVIS_EXPANDED) || !hChild)) || (!IsLeaf(tvitem.lParam) && hOverItem != hittest.hItem))
					TreeView_SelectDropTarget(hTreeMenu, hOverItem);
				else if (hOverItem == TreeView_GetRoot(hTreeMenu) || (!IsLeaf(tvitem.lParam) && (TreeView_GetItemState(hTreeMenu, hOverItem, TVIS_EXPANDED) & TVIS_EXPANDED) && hChild))
					TreeView_SetInsertMark(hTreeMenu, hChild, FALSE);
				else
					TreeView_SetInsertMark(hTreeMenu, hOverItem, TRUE);
			} else {
				if(hittest.flags & TVHT_ABOVE) SendMessage(hTreeMenu, WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
				if(hittest.flags & TVHT_BELOW) SendMessage(hTreeMenu, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
			}
			ImageList_DragShowNolock(TRUE);
			
			ImageList_DragMove(pt.x, pt.y);
			//ImageList_DragEnter(NULL, pt.x, pt.y);
		}
		
		/* ----------------------------------------------------------------- */
		//  LButtonUp
		/* ----------------------------------------------------------------- */
		static void LButtonUp(HWND hWnd, HWND hTreeMenu, HWND hTreeOrg) {
			ImageList_DragShowNolock(FALSE);
			TreeView_SetInsertMark(hTreeMenu, NULL, FALSE);
			
			POINT pt;
			TVHITTESTINFO hittest = {};
			GetCursorPos(&pt);
			hittest.flags = TVHT_ONITEM;
			hittest.pt.x = pt.x;
			hittest.pt.y = pt.y;
			ScreenToClient(hTreeMenu, &hittest.pt );

			hittest.hItem = TreeView_HitTest(hTreeMenu, &hittest);
			if (hittest.hItem && hittest.hItem != hDragging.second && hDragging.second != TreeView_GetRoot(hTreeMenu) && !IsChild(hTreeMenu, hittest.hItem, hDragging.second) && (hittest.flags & (TVHT_ONITEM|TVHT_ONITEMRIGHT))) {
				TVITEM		tvitem = GetTreeViewItem(hTreeMenu, hittest.hItem);
				HTREEITEM	hOverItem = hittest.hItem;
				HTREEITEM	hChild = TreeView_GetChild(hTreeMenu, hOverItem);
				HTREEITEM	hItem;
				hittest.pt.y -= TreeView_GetItemHeight(hTreeMenu) / 2;
				TreeView_HitTest(hTreeMenu, &hittest);
				if ((hOverItem == TreeView_GetRoot(hTreeMenu) && (!(TreeView_GetItemState(hTreeMenu, hOverItem, TVIS_EXPANDED) & TVIS_EXPANDED) || !hChild)) || (!IsLeaf(tvitem.lParam) && hOverItem != hittest.hItem))
					hItem = CopyTreeViewItem(hTreeMenu, hOverItem, hDragging.first, hDragging.second);
				else if (hOverItem == TreeView_GetRoot(hTreeMenu) || (!IsLeaf(tvitem.lParam) && (TreeView_GetItemState(hTreeMenu, hOverItem, TVIS_EXPANDED) & TVIS_EXPANDED) && hChild))
					hItem = CopyTreeViewItem(hTreeMenu, hOverItem, hDragging.first, hDragging.second, TVI_FIRST);
				else
					hItem = CopyTreeViewItem(hTreeMenu, TreeView_GetParent(hTreeMenu, hOverItem), hDragging.first, hDragging.second, hOverItem);
				TreeView_SelectItem(hTreeMenu, hItem);
				if (hDragging.first == hTreeMenu)
					TreeView_DeleteItem(hTreeMenu, hDragging.second);
			}
			
			TreeView_SelectDropTarget(hTreeMenu, NULL);
			ImageList_DragShowNolock(TRUE);
			
			ImageList_DragLeave(NULL);
			ImageList_EndDrag();
			ReleaseCapture();
			
			bDragging = FALSE;
			hDragging = std::make_pair(reinterpret_cast<HWND>(NULL), reinterpret_cast<HTREEITEM>(NULL));
		}
		
		/* ----------------------------------------------------------------- */
		//  customize_wndproc
		/* ----------------------------------------------------------------- */
		static INT_PTR CALLBACK customize_wndproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			static cubeice::user_setting *setting = NULL;
			static HWND hTreeMenu = NULL;
			static HWND hTreeOrg = NULL;
			static HMENU popup = NULL;
			
			switch (msg) {
			case WM_INITDIALOG:
			{
				hTreeMenu = GetDlgItem(hWnd, IDC_CURRENT_TREEVIEW);
				hTreeOrg = GetDlgItem(hWnd, IDC_ADD_TREEVIEW);
				bDragging = FALSE;

				// TreeView で使用するアイコンの初期化
				HIMAGELIST imagelist = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 5, 1);
				ImageList_AddIcon(imagelist, LoadIcon(GetModuleHandle(NULL), _T("IDI_COMPRESS")));
				ImageList_AddIcon(imagelist, LoadIcon(GetModuleHandle(NULL), _T("IDI_DECOMPRESS")));
				ImageList_AddIcon(imagelist, LoadIcon(GetModuleHandle(NULL), _T("IDI_FOLDER")));
				ImageList_AddIcon(imagelist, LoadIcon(GetModuleHandle(NULL), _T("IDI_FOLDER_OPEN")));
				ImageList_AddIcon(imagelist, LoadIcon(GetModuleHandle(NULL), _T("IDI_MENUTOP")));
				TreeView_SetImageList(hTreeMenu, imagelist , TVSIL_NORMAL);
				TreeView_SetImageList(hTreeOrg, imagelist, TVSIL_NORMAL);
				
				setting = reinterpret_cast<cubeice::user_setting*>(lp);
				InitTreeViewItem(hTreeMenu, hTreeOrg, setting->context_submenu());
				
				// アイコン
				HICON app = LoadIcon(GetModuleHandle(NULL), _T("IDI_APP"));
				SendMessage(hWnd, WM_SETICON, 0, LPARAM(app));
				
				// 画面中央に表示
				RECT rect = {};
				GetWindowRect(hWnd, (LPRECT)&rect);
				int x = (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2;
				int y = (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2;
				SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
				
				// TreeView (hTreeMenu) のウィンドウプロシージャのサブクラス化
				DefaultTreeViewProc = (WNDPROC)SetWindowLongPtr(hTreeMenu, GWLP_WNDPROC, (LONG_PTR)TreeViewProc);
				
				return FALSE;
			}
			case WM_MOUSEMOVE:
				if (bDragging) MouseMove(hWnd, hTreeMenu, hTreeOrg);
				return TRUE;
			case WM_LBUTTONUP:
				if (bDragging) LButtonUp(hWnd, hTreeMenu, hTreeOrg);
				return TRUE;
			case WM_NOTIFY:
				if(reinterpret_cast<LPNMHDR>(lp)->code == TVN_BEGINLABELEDIT && reinterpret_cast<LPNMHDR>(lp)->hwndFrom == hTreeMenu) {
					// Enter, Esc キーの処理のためにサブクラス化
					HWND edit = (HWND)SendMessage(hTreeMenu, TVM_GETEDITCONTROL, 0, 0);
					DefaultEditProc = (WNDPROC)SetWindowLongPtr(edit, GWLP_WNDPROC, (LONG_PTR)&EditProc);
					
					if(reinterpret_cast<LPNMTVDISPINFO>(lp)->item.lParam == SUBMENU_ROOT_ID) {
						SetWindowLongPtr(hWnd, DWLP_MSGRESULT, TRUE);
					}
					else SetWindowLongPtr(hWnd, DWLP_MSGRESULT, FALSE);
					return TRUE;
				} else if(reinterpret_cast<LPNMHDR>(lp)->code == TVN_ENDLABELEDIT && reinterpret_cast<LPNMHDR>(lp)->hwndFrom == hTreeMenu) {
					TreeView_SetItem(hTreeMenu, &reinterpret_cast<TV_DISPINFO*>(lp)->item);
					return TRUE;
				}
				else if (reinterpret_cast<LPNMHDR>(lp)->code == TVN_BEGINDRAG) {
					LPNMTREEVIEW pitem = reinterpret_cast<LPNMTREEVIEW>(lp);
					//if (pitem->hdr.hwndFrom == hTreeMenu) BeginDrag(pitem);
					BeginDrag(pitem);
					break;
				}
				break;
			case WM_CONTEXTMENU:
				{
					HWND handle = reinterpret_cast<HWND>(wp);
					TV_HITTESTINFO hit = {};
					hit.pt.x = LOWORD(lp); 
					hit.pt.y = HIWORD(lp);
					ScreenToClient(handle, &hit.pt);
					HTREEITEM item = TreeView_HitTest(handle, &hit);
					if (item) {
						TreeView_SelectItem(handle, item);
						// 選択されている項目によって表示するポップアップメニューを変える．
						const int index = (handle == hTreeOrg) ? 2 : (item == TreeView_GetRoot(handle) ? 0 : 1);
						popup = LoadMenu(GetModuleHandle(NULL), _T("IDR_CONTEXT_MENU"));
						HMENU submenu = GetSubMenu(popup, index);
						TrackPopupMenu(submenu, TPM_LEFTALIGN | TPM_TOPALIGN, (int)LOWORD(lp), (int)HIWORD(lp), 0, hWnd, NULL);
					}
					break;
				}
			case WM_COMMAND:
				if (HIWORD(wp) != BN_CLICKED) break;
				
				switch (LOWORD(wp)) {
				case IDOK:
				{
					std::vector<cubeice::user_setting::SUBMENU> v;
					GetSubmenuStruct(v, hTreeMenu, TreeView_GetChild(hTreeMenu, TVI_ROOT));
					if(!CheckValidation(v)) {
						static const TCHAR* message = _T("空のフォルダが存在します。空のフォルダはコンテキストメニューには表示されません。\r\n続行しますか？");
						if (MessageBox(hWnd, message, _T("警告"), MB_YESNO | MB_ICONWARNING) == IDNO) break;
					}
					setting->context_submenu() = v;
					setting->context_customize() = true;
				}
				case IDCANCEL:
					if (popup) DestroyMenu(popup);
					popup = NULL;
					hTreeMenu = NULL;
					hTreeOrg = NULL;
					EndDialog(hWnd, LOWORD(wp));
					break;
				case IDC_ADD_BUTTON: // 追加
				case IDM_ADD_MENU:
				{
					HTREEITEM	hInsertTo;
					HTREEITEM	hInsertFrom;
					HTREEITEM	hInsertedItem;
					TV_ITEM		tvitem;

					hInsertTo = TreeView_GetSelection(hTreeMenu);
					hInsertFrom = TreeView_GetSelection(hTreeOrg);
					if(!hInsertTo || !hInsertFrom) break;

					// 挿入不可な項目 (Leaf) が選択されている場合は，その親に挿入する．
					if (IsLeaf(GetTreeViewItem(hTreeMenu, hInsertTo).lParam)) hInsertTo = TreeView_GetParent(hTreeMenu, hInsertTo);
					if (!hInsertTo) break;

					bool f = false;
					tvitem.mask = TVIF_HANDLE | TVIF_PARAM;
					tvitem.hItem = hInsertTo;
					TreeView_GetItem(hTreeMenu, &tvitem);
					for(int i = 0 ; MenuItem[i].stringA ; ++i) {
						if(tvitem.lParam == MenuItem[i].dispSetting) {
							f = true;
							break;
						}
					}
					if(tvitem.lParam != SUBMENU_ROOT_ID && tvitem.lParam != SUBMENU_DIR_ID && !f) return 0;

					hInsertedItem = CopyTreeViewItem(hTreeMenu, hInsertTo, hTreeOrg, hInsertFrom, TVI_LAST, true);
					TreeView_Expand(hTreeMenu, hInsertTo, TVE_EXPAND);
					tvitem.mask		= TVIF_HANDLE | TVIF_PARAM;
					tvitem.hItem	= hInsertedItem;
					TreeView_GetItem(hTreeMenu, &tvitem);
					if(tvitem.lParam == static_cast<LPARAM>(0)) {
						TreeView_Select(hTreeMenu, tvitem.hItem, TVGN_CARET);
						TreeView_EditLabel(hTreeMenu, tvitem.hItem);
					}
					break;
				}
				case IDC_DELETE_BUTTON: // 削除
				case IDM_DELETE_MENU:
				{
					HTREEITEM	hItem;
					hItem = TreeView_GetSelection(hTreeMenu);
					if(hItem && TreeView_GetParent(hTreeMenu, hItem))
						TreeView_DeleteItem(hTreeMenu, hItem);
					break;
				}
				case IDC_FOLDER_BUTTON: // 新しいフォルダを追加
				case IDM_FOLDER_MENU:
				{
					// 挿入不可な項目 (Leaf) が選択されている場合は，その親に挿入する．
					HTREEITEM hInsertTo = TreeView_GetSelection(hTreeMenu);
					if (hInsertTo && IsLeaf(GetTreeViewItem(hTreeMenu, hInsertTo).lParam)) hInsertTo = TreeView_GetParent(hTreeMenu, hInsertTo);
					if (!hInsertTo) break;
					
					TVINSERTSTRUCT tvi = {};
					tvi.hParent = hInsertTo;
					tvi.hInsertAfter = TVI_LAST;
					tvi.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
					tvi.item.pszText = _T("新しいフォルダ");
					tvi.item.iImage = GetItemImageIndex(SUBMENU_DIR_ID);
					tvi.item.iSelectedImage = GetSelectedItemImageIndex(SUBMENU_DIR_ID);
					tvi.item.lParam = SUBMENU_DIR_ID;
					HTREEITEM inserted = TreeView_InsertItem(hTreeMenu, &tvi);
					
					tvi.item.mask = TVIF_HANDLE | TVIF_PARAM;
					tvi.item.hItem = inserted;
					TreeView_GetItem(hTreeMenu, &tvi.item);
					TreeView_Select(hTreeMenu, tvi.item.hItem, TVGN_CARET);
					TreeView_EditLabel(hTreeMenu, tvi.item.hItem);
					break;
				}
				case IDC_UP_BUTTON: // 上へ
				case IDM_UP_MENU:
				{
					HTREEITEM hTreeItem = TreeView_GetSelection(hTreeMenu);
					if(!hTreeItem) break;

					HTREEITEM hPrevItem = TreeView_GetPrevSibling(hTreeMenu, hTreeItem);
					if(!hPrevItem) break;

					hPrevItem = TreeView_GetPrevSibling(hTreeMenu, hPrevItem);
					if(!hPrevItem) hPrevItem = TVI_FIRST;

					HTREEITEM hNewItem = CopyTreeViewItem(hTreeMenu, TreeView_GetParent(hTreeMenu, hTreeItem), hTreeMenu, hTreeItem, hPrevItem);
					TreeView_DeleteItem(hTreeMenu, hTreeItem);
					TreeView_SelectItem(hTreeMenu, hNewItem);
					break;
				}
				case IDC_DOWN_BUTTON: // 下へ
				case IDM_DOWN_MENU:
				{
					HTREEITEM hTreeItem = TreeView_GetSelection(hTreeMenu);
					if(!hTreeItem) break;

					HTREEITEM hNextItem = TreeView_GetNextSibling(hTreeMenu, hTreeItem);
					if(!hNextItem) break;

					HTREEITEM hNewItem = CopyTreeViewItem(hTreeMenu, TreeView_GetParent(hTreeMenu, hTreeItem), hTreeMenu, hTreeItem, hNextItem);
					TreeView_DeleteItem(hTreeMenu, hTreeItem);
					TreeView_SelectItem(hTreeMenu, hNewItem);
					break;
				}
				case IDC_RENAME_BUTTON: // リネーム
				case IDM_RENAME_MENU:
				{
					HTREEITEM hTreeItem = TreeView_GetSelection(hTreeMenu);
					TreeView_EditLabel(hTreeMenu, hTreeItem);
					break;
				}
				case IDC_RESET_BUTTON: // リセット
				case IDM_RESET_MENU:
					TreeView_DeleteAllItems(hTreeMenu);
					TreeView_DeleteAllItems(hTreeOrg);
					InitTreeViewItem(hTreeMenu, hTreeOrg, setting->context_submenu());
					break;
				default:
					return FALSE;
				}
				return TRUE;
			default:
				break;
			}
			
			return FALSE;
		}

		/* ----------------------------------------------------------------- */
		//  customize
		/* ----------------------------------------------------------------- */
		int customize(HWND owner, cubeice::user_setting& setting) {
			return DialogBoxParam(GetModuleHandle(NULL), _T("IDD_CONTEXT_MENU"), owner, customize_wndproc, (LPARAM)&setting);
		}
	}
}