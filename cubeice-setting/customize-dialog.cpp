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
#include <vector>
#include <map>
#include "../cubeice-ctx/cubeice-ctxdata.h"

namespace cubeice {
	namespace dialog {
		/* ----------------------------------------------------------------- */
		//  InitMenuList
		/* ----------------------------------------------------------------- */
		void InitMenuList(HWND hTreeView, HTREEITEM hParent, const SUB_MENU_ITEM *smi)
		{
			TVINSERTSTRUCT	tvi;

			tvi.hParent			= hParent;
			tvi.hInsertAfter	= TVI_LAST;
			tvi.item.mask		= TVIF_TEXT | TVIF_PARAM;

			for(int i = 0 ; smi[i].stringA ; ++i) {
				HTREEITEM	hTreeItem;

#ifdef	UNICODE
				tvi.item.pszText	= const_cast<LPWSTR>(smi[i].stringW);
#else
				tvi.item.pszText	= const_cast<LPSTR>(smi[i].stringA);
#endif
				tvi.item.lParam		= smi[i].dispSetting;

				hTreeItem = TreeView_InsertItem(hTreeView, &tvi);
				if(smi[i].submenu)
					InitMenuList(hTreeView, hTreeItem, smi[i].submenu);
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
				if(smi[i].submenu)
					SerializeMenuItemData(table, smi[i].submenu);
			}
		}

		/* ----------------------------------------------------------------- */
		//  SetMenuItem
		/* ----------------------------------------------------------------- */
		void SetMenuItem(HWND hMenuTreeView, HTREEITEM hTreeItem, const std::vector<cubeice::user_setting::SUBMENU> &submenu)
		{
			BOOST_FOREACH(const cubeice::user_setting::SUBMENU &s, submenu) {

			}
		}

		/* ----------------------------------------------------------------- */
		//  InitTreeViewItem
		/* ----------------------------------------------------------------- */
		void InitTreeViewItem(HWND hMenuTreeView, HWND hItemListTreeView, const std::vector<cubeice::user_setting::SUBMENU> &submenu)
		{
			HTREEITEM	hItem;

			InitMenuList(hItemListTreeView, TVI_ROOT, MenuItem);

			TVINSERTSTRUCT	tvi;
			tvi.hParent			= TVI_ROOT;
			tvi.hInsertAfter	= TVI_LAST;
			tvi.item.mask		= TVIF_TEXT | TVIF_PARAM;
			tvi.item.pszText	= TEXT("新しいフォルダを追加");
			tvi.item.lParam		= static_cast<LPARAM>(0);
			TreeView_InsertItem(hItemListTreeView, &tvi);

			tvi.item.mask		= TVIF_TEXT | TVIF_PARAM;
			tvi.item.pszText	= TEXT("ルート");
			tvi.item.lParam		= static_cast<LPARAM>(-1);
			hItem = TreeView_InsertItem(hMenuTreeView, &tvi);
			TreeView_Select(hMenuTreeView, hItem, TVGN_CARET);

		//	std::map<cubeice::user_setting::size_type, TCHAR*>	table;
		//	SerializeMenuItemData(table, MenuItem);
		//	BOOST_FOREACH(const cubeice::user_setting::SUBMENU &s, submenu) {
		//		if(!s.id) {
		//			// ---------------------------------------------------------------------------------------------------------------------
		//			// ここでUNICODEビルドでは通らない・・・
		////			tvi.item.pszText = s.str.c_str();
		//		} else if(table.find(s.id) != table.end()) {
		//			tvi.item.pszText = table[s.id];
		//		}
		//	}
		}

		/* ----------------------------------------------------------------- */
		//  CopyTreeViewItem
		/* ----------------------------------------------------------------- */
		HTREEITEM CopyTreeViewItem(HWND &hTo, HTREEITEM hToParentItem, HWND &hFrom, HTREEITEM hFromParentItem)
		{
			static	TCHAR	str[2048];
			TVINSERTSTRUCT	tvi;
			HTREEITEM	hInsertItem, hChildItem;

			tvi.hParent			= hToParentItem;
			tvi.hInsertAfter	= TVI_LAST;
			tvi.item.mask		= TVIF_HANDLE | TVIF_TEXT | TVIF_PARAM;
			tvi.item.hItem		= hFromParentItem;
			tvi.item.pszText	= str;
			tvi.item.cchTextMax	= sizeof(str) / sizeof(str[0]);

			TreeView_GetItem(hFrom, &tvi.item);
			tvi.item.mask		= TVIF_TEXT | TVIF_PARAM;
			hInsertItem = TreeView_InsertItem(hTo, &tvi);

			hChildItem = TreeView_GetChild(hFrom, hFromParentItem);
			while(hChildItem) {
				CopyTreeViewItem(hTo, hInsertItem, hFrom, hChildItem);
				hChildItem = TreeView_GetNextSibling(hFrom, hChildItem);
			}
			return hInsertItem;
		}

		/* ----------------------------------------------------------------- */
		//  customize_wndproc
		/* ----------------------------------------------------------------- */
		static INT_PTR CALLBACK customize_wndproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
			static cubeice::user_setting* setting = NULL;
			HWND hTreeMenu = GetDlgItem(hWnd, IDC_CURRENT_TREEVIEW);
			HWND hTreeOrg = GetDlgItem(hWnd, IDC_ADD_TREEVIEW);
			
			switch (msg) {
			case WM_INITDIALOG:
			{
				setting = reinterpret_cast<cubeice::user_setting*>(lp);
				InitTreeViewItem(hTreeMenu, hTreeOrg, std::vector<cubeice::user_setting::SUBMENU>());
				
				// アイコン
				HICON app = LoadIcon(GetModuleHandle(NULL), _T("IDI_APP"));
				SendMessage(hWnd, WM_SETICON, 0, LPARAM(app));
				
				// 画面中央に表示
				RECT rect = {};
				GetWindowRect(hWnd, (LPRECT)&rect);
				int x = (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2;
				int y = (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2;
				SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
				
				return FALSE;
			}
			case WM_COMMAND:
				if (HIWORD(wp) != BN_CLICKED) break;
				
				switch (LOWORD(wp)) {
				case IDOK:
				case IDCANCEL:
					EndDialog(hWnd, 0);
					break;
				case IDC_ADD_BUTTON: // 追加
				{
					HTREEITEM	hInsertTo;
					HTREEITEM	hInsertFrom;
					HTREEITEM	hInsertedItem;
					TV_ITEM		tvitem;

					hInsertTo = TreeView_GetSelection(hTreeMenu);
					hInsertFrom = TreeView_GetSelection(hTreeOrg);

					if(!hInsertTo || !hInsertFrom)
						break;

					hInsertedItem = CopyTreeViewItem(hTreeMenu, hInsertTo, hTreeOrg, hInsertFrom);
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
				{
					HTREEITEM	hItem;
					hItem = TreeView_GetSelection(hTreeMenu);
					if(hItem && TreeView_GetParent(hTreeMenu, hItem))
						TreeView_DeleteItem(hTreeMenu, hItem);
					break;
				}
				case IDC_UP_BUTTON: // 上へ
				case IDC_DOWN_BUTTON: // 下へ
				case IDC_RESET_BUTTON: // リセット
				default:
					break;
				}
				break;
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