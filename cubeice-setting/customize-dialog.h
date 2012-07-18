// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-setting/customize-dialog.h
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
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_SETTING_CUSTOMIZE_DIALOG_H
#define CUBEICE_SETTING_CUSTOMIZE_DIALOG_H

#include "cubeice-setting.h"
#include <psdotnet/forms/modal-form.h>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// CustomizeDialog
	///
	/// <summary>
	/// コンテキストメニューのカスタマイズのためのダイアログです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class CustomizeDialog : public PsdotNet::Forms::ModalForm {
	private:
		typedef PsdotNet::Forms::ModalForm super;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		CustomizeDialog(UserSetting& data) :
			super(_T("IDD_CUSTOMIZE")), data_(data), serialized_(),
			hfrom_(NULL), hto_(NULL), himages_(NULL), hmenu_(NULL),
			editing_(false), dragging_(reinterpret_cast<handle_type>(NULL), reinterpret_cast<HTREEITEM>(NULL)) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~CustomizeDialog() {}
		
	protected:
		/* ----------------------------------------------------------------- */
		///
		/// OnCreateControl
		///
		/// <summary>
		/// 画面生成直後の初期化処理を行います。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void OnCreateControl() {
			hfrom_ = ::GetDlgItem(this->Handle(), IDC_CUSTOMIZE_AVAILABLE);
			hto_ = ::GetDlgItem(this->Handle(), IDC_CUSTOMIZE_CURRENT);
			this->LoadImageList();
			
			// 追加できるコンテキストメニュー
			ContextContainer src;
			ConvertContext(0x7ffffb, src, data_.InstallDirectory());
			this->LoadContext(src, hfrom_, TVI_ROOT);
			TreeView_Select(hfrom_, TreeView_GetRoot(hfrom_), TVGN_CARET);
			
			// 現在のコンテキストメニュー
			this->LoadCurrentContext();
			
			// ポップアップメニュー
			hmenu_ = ::LoadMenu(::GetModuleHandle(NULL), _T("IDR_CONTEXT_MENU"));
			
			super::OnCreateControl();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// OnOK
		///
		/// <summary>
		/// OK ボタンが押された時の処理を行います。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void OnOK() {
			if (!editing_) {
				data_.Context().IsExtended(true);
				data_.Context().ExtendedContext().clear();
				this->SaveContext(TreeView_GetChild(hto_, TVI_ROOT), data_.Context().ExtendedContext());
				super::OnOK();
			}
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// OnCancel
		///
		/// <summary>
		/// キャンセルボタンが押された時の処理を行います。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void OnCancel() {
			if (!editing_) super::OnCancel();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// OnDestroy
		///
		/// <summary>
		/// 使用したリソースを解放します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void OnDestroy() {
			if (hmenu_) {
				::DestroyMenu(hmenu_);
				hmenu_ = NULL;
			}
			
			if (himages_) {
				ImageList_Destroy(himages_);
				himages_ = NULL;
			}
			
			super::OnDestroy();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// OnContextMenu
		///
		/// <summary>
		/// コンテキストメニューを表示します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void OnContextMenu(PsdotNet::Forms::Message& m) {
			m.Result(FALSE);
			
			handle_type handle = reinterpret_cast<handle_type>(m.WParam());
			TV_HITTESTINFO hit = {};
			hit.pt.x = LOWORD(m.LParam());
			hit.pt.y = HIWORD(m.LParam());
			::ScreenToClient(handle, &hit.pt);
			HTREEITEM item = TreeView_HitTest(handle, &hit);
			if (!item) return;
			
			TreeView_SelectItem(handle, item);
			int index = (handle == hfrom_) ? 2 : (item == TreeView_GetRoot(handle) ? 0 : 1);
			HMENU submenu = ::GetSubMenu(hmenu_, index);
			if (!submenu) return;
			
			int x = static_cast<int>(LOWORD(m.LParam()));
			int y = static_cast<int>(HIWORD(m.LParam()));
			::TrackPopupMenu(submenu, TPM_LEFTALIGN | TPM_TOPALIGN, x, y, 0, this->Handle(), NULL);
			m.Result(TRUE);
		}
		
		/* ----------------------------------------------------------------- */
		/// OnNotify
		/* ----------------------------------------------------------------- */
		virtual void OnNotify(PsdotNet::Forms::Message& m) {
			LPNMHDR header = reinterpret_cast<LPNMHDR>(m.LParam());
			switch (header->code) {
			case TVN_BEGINLABELEDIT: {
				if (header->hwndFrom != hto_) break;
				handle_type edit = reinterpret_cast<handle_type>(::SendMessage(hto_, TVM_GETEDITCONTROL, 0, 0));
				editproc_ = reinterpret_cast<WNDPROC>(::SetWindowLongPtr(edit, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&CustomizeDialog::StaticEditProc)));
				
				LPNMTVDISPINFO info = reinterpret_cast<LPNMTVDISPINFO>(m.LParam());
				if (static_cast<int>(info->item.lParam) == -1) ::SetWindowLongPtr(m.HWnd(), DWLP_MSGRESULT, TRUE);
				else ::SetWindowLongPtr(m.HWnd(), DWLP_MSGRESULT, FALSE);
				
				editing_ = true;
				m.Result(TRUE);
				break;
			}
			case TVN_ENDLABELEDIT:
				TreeView_SetItem(hto_, &reinterpret_cast<TV_DISPINFO*>(m.LParam())->item);
				editing_ = false;
				m.Result(TRUE);
				break;
			case TVN_BEGINDRAG: {
				LPNMTREEVIEW item = reinterpret_cast<LPNMTREEVIEW>(m.LParam());
				m.Result(BeginDrag(item));
				break;
			}
			default:
				m.Result(FALSE);
				break;
			}
		}
		
		/* ----------------------------------------------------------------- */
		/// OnMouseMove
		/* ----------------------------------------------------------------- */
		virtual void OnMouseMove(PsdotNet::Forms::Message& m) {
			if (dragging_.first) {
				ImageList_DragShowNolock(FALSE);
				TreeView_SelectDropTarget(hto_, NULL);
				TreeView_SetInsertMark(hto_, NULL, FALSE);
				
				POINT pt = {};
				::GetCursorPos(&pt);
				m.Result(this->OnDragging(pt));
				
				ImageList_DragShowNolock(TRUE);
				ImageList_DragMove(pt.x, pt.y);
			}
			else m.Result(FALSE);
		}
		
		/* ----------------------------------------------------------------- */
		/// OnLButtonUp
		/* ----------------------------------------------------------------- */
		virtual void OnLButtonUp(PsdotNet::Forms::Message& m) {
			if (dragging_.first) {
				ImageList_DragShowNolock(FALSE);
				TreeView_SetInsertMark(hto_, NULL, FALSE);
				
				POINT pt = {};
				::GetCursorPos(&pt);
				m.Result(this->EndDrag(pt));
				
				TreeView_SelectDropTarget(hto_, NULL);
				ImageList_DragShowNolock(TRUE);
				ImageList_DragLeave(NULL);
				ImageList_EndDrag();
				::ReleaseCapture();
				
				dragging_.first = NULL;
				dragging_.second = NULL;
			}
			else m.Result(FALSE);
		}
		
		/* ----------------------------------------------------------------- */
		/// OnCommand
		/* ----------------------------------------------------------------- */
		virtual void OnCommand(PsdotNet::Forms::Message& m) {
			switch (LOWORD(m.WParam())) {
			case IDC_CUSTOMIZE_INSERT:
				m.Result(this->InsertContext());
				break;
			case IDC_CUSTOMIZE_DELETE:
				m.Result(this->DeleteContext());
				break;
			case IDC_CUSTOMIZE_FOLDER:
				m.Result(this->InsertFolderContext());
				break;
			case IDC_CUSTOMIZE_UP:
				m.Result(this->UpContext());
				break;
			case IDC_CUSTOMIZE_DOWN:
				m.Result(this->DownContext());
				break;
			case IDC_CUSTOMIZE_RENAME:
				m.Result(this->RenameContext());
				break;
			case IDC_CUSTOMIZE_RESET:
				m.Result(this->ResetContext());
				break;
			default:
				super::OnCommand(m);
				break;
			}
		}
		
		/* ----------------------------------------------------------------- */
		/// WndProc
		/* ----------------------------------------------------------------- */
		virtual void WndProc(PsdotNet::Forms::Message& m) {
			switch (m.Msg()) {
			case WM_LBUTTONUP:
				this->OnLButtonUp(m);
				break;
			case WM_MOUSEMOVE:
				this->OnMouseMove(m);
				break;
			default:
				super::WndProc(m);
				break;
			}
		}
		
	private:
		/* ----------------------------------------------------------------- */
		///
		/// LoadImageList
		///
		/// <summary>
		/// ツリービューで使用するアイコンを読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void LoadImageList() {
			himages_ = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 6, 1);
			ImageList_AddIcon(himages_, LoadIcon(GetModuleHandle(NULL), _T("IDI_APP")));
			ImageList_AddIcon(himages_, LoadIcon(GetModuleHandle(NULL), _T("IDI_COMPRESSION")));
			ImageList_AddIcon(himages_, LoadIcon(GetModuleHandle(NULL), _T("IDI_DECOMPRESSION")));
			ImageList_AddIcon(himages_, LoadIcon(GetModuleHandle(NULL), _T("IDI_FOLDER")));
			ImageList_AddIcon(himages_, LoadIcon(GetModuleHandle(NULL), _T("IDI_FOLDER_OPEN")));
			ImageList_AddIcon(himages_, LoadIcon(GetModuleHandle(NULL), _T("IDI_MENUTOP")));
			TreeView_SetImageList(hfrom_, himages_, TVSIL_NORMAL);
			TreeView_SetImageList(hto_, himages_, TVSIL_NORMAL);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// LoadContext
		///
		/// <summary>
		/// コンテキストメニューを読み込んで表示します。
		/// NOTE: 編集中は各項目はシリアライズ化するため、各項目が保持する
		/// 子要素は全て削除します。ただし、フォルダ項目であるかどうかを
		/// 区別するために、削除した後にダミー項目を挿入します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void LoadContext(const ContextContainer& src, handle_type htree, HTREEITEM parent) {
			TVINSERTSTRUCT tvi = {};
			tvi.hParent = parent;
			tvi.hInsertAfter = TVI_LAST;
			tvi.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			
			BOOST_FOREACH(const Context& item, src) {
				serialized_.push_back(item);
				serialized_.back().Index(-1);
				if (!serialized_.back().Children().empty()) {
					serialized_.back().Children().clear();
					serialized_.back().Children().push_back(Context(_T("branch marker"))); // フォルダ識別用
				}
				
				int index = static_cast<int>(serialized_.size() - 1);
				
				tvi.item.pszText = const_cast<char_type*>(item.Name().c_str());
				tvi.item.lParam = index;
				tvi.item.iImage = this->GetImageIndex(index);
				tvi.item.iSelectedImage = this->GetSelectedImageIndex(index);
				
				HTREEITEM node = TreeView_InsertItem(htree, &tvi);
				if (!node) continue;
				if (!item.Children().empty()) this->LoadContext(item.Children(), htree, node);
			}
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// LoadCurrentContext
		///
		/// <summary>
		/// 現在のコンテキストメニューを読み込んで表示します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void LoadCurrentContext() {
			TVINSERTSTRUCT tvi = {};
			tvi.hParent = TVI_ROOT;
			tvi.hInsertAfter = TVI_LAST;
			tvi.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			tvi.item.pszText = _T("メニュートップ");
			tvi.item.lParam = static_cast<LPARAM>(-1);
			tvi.item.iImage = 5;
			tvi.item.iSelectedImage = 5;
			HTREEITEM root = TreeView_InsertItem(hto_, &tvi);
			
			if (!data_.Context().IsExtended()) {
				ContextContainer src;
				ConvertContext(data_.Context().Builtin(), src, data_.InstallDirectory());
				this->LoadContext(src, hto_, root);
			}
			else this->LoadContext(data_.Context().ExtendedContext(), hto_, root);
			
			TreeView_Select(hto_, root, TVGN_CARET);
			TreeView_Expand(hto_, root, TVE_EXPAND);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveContext
		///
		/// <summary>
		/// コンテキストメニューを保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void SaveContext(HTREEITEM src, ContextContainer& dest) {
			HTREEITEM child = TreeView_GetChild(hto_, src);
			while (child) {
				char_type buffer[2048] = {};
				TVITEM tvi = {};
				tvi.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
				tvi.hItem = child;
				tvi.pszText = buffer;
				tvi.cchTextMax = sizeof(buffer) / sizeof(buffer[0]);
				
				TreeView_GetItem(hto_, &tvi);
				int index = static_cast<int>(tvi.lParam);
				if (index >= serialized_.size()) return;
				
				Context cx = serialized_[index];
				cx.Name(tvi.pszText);
				cx.Children().clear();
				if (TreeView_GetChild(hto_, child)) this->SaveContext(child, cx.Children());
				dest.push_back(cx);
				
				child = TreeView_GetNextSibling(hto_, child);
			}
		}
		
	private:
		/* ----------------------------------------------------------------- */
		///
		/// InsertContext
		///
		/// <summary>
		/// コンテキストメニューを追加します
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type InsertContext() {
			HTREEITEM from = TreeView_GetSelection(hfrom_);
			HTREEITEM to = TreeView_GetSelection(hto_);
			if (!from || !to) return FALSE;
			
			int index = static_cast<int>(this->GetTreeViewItem(hto_, to).lParam);
			if (index >= 0 && index < serialized_.size()) {
				if (serialized_[index].Children().empty()) to = TreeView_GetParent(hto_, to);
				if (!to) return FALSE;
			}
			
			HTREEITEM newitem = CopyTreeViewItem(hfrom_, from, hto_, to, TVI_LAST);
			TreeView_Expand(hto_, newitem, TVE_EXPAND);
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// DeleteContext
		///
		/// <summary>
		/// 選択したコンテキストメニューを削除します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type DeleteContext() {
			HTREEITEM item = TreeView_GetSelection(hto_);
			if (item && TreeView_GetParent(hto_, item)) {
				TreeView_DeleteItem(hto_, item);
				return TRUE;
			}
			return FALSE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// InsertFolderContext
		///
		/// <summary>
		/// 新しいフォルダを追加します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type InsertFolderContext() {
			HTREEITEM dest = TreeView_GetSelection(hto_);
			if (!dest) return FALSE;
			
			int index = static_cast<int>(this->GetTreeViewItem(hto_, dest).lParam);
			if (index >= 0 && index < serialized_.size()) {
				if (serialized_[index].Children().empty()) dest = TreeView_GetParent(hto_, dest);
				if (!dest) return FALSE;
			}
			
			Context folder(_T("新しいフォルダ"));
			folder.Children().push_back(Context(_T("branch marker")));
			serialized_.push_back(folder);
			
			index = static_cast<int>(serialized_.size() - 1);
			
			TVINSERTSTRUCT tvi = {};
			tvi.hParent = dest;
			tvi.hInsertAfter = TVI_LAST;
			tvi.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			tvi.item.pszText = const_cast<char_type*>(folder.Name().c_str());
			tvi.item.iImage = this->GetImageIndex(index);
			tvi.item.iSelectedImage = this->GetSelectedImageIndex(index);
			tvi.item.lParam = index;
			HTREEITEM inserted = TreeView_InsertItem(hto_, &tvi);
			
			tvi.item.mask = TVIF_HANDLE | TVIF_PARAM;
			tvi.item.hItem = inserted;
			TreeView_GetItem(hto_, &tvi.item);
			TreeView_Select(hto_, tvi.item.hItem, TVGN_CARET);
			TreeView_EditLabel(hto_, tvi.item.hItem);
			
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// UpContext
		///
		/// <summary>
		/// 選択されたコンテキストメニューを一つ上に移動します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type UpContext() {
			HTREEITEM target = TreeView_GetSelection(hto_);
			if (!target) return FALSE;
			
			HTREEITEM prev = TreeView_GetPrevSibling(hto_, target);
			if (!prev) return FALSE;
			prev = TreeView_GetPrevSibling(hto_, prev);
			if (!prev) prev = TVI_FIRST;
			
			HTREEITEM newitem = this->CopyTreeViewItem(hto_, target, hto_, TreeView_GetParent(hto_, target), prev);
			TreeView_DeleteItem(hto_, target);
			TreeView_SelectItem(hto_, newitem);
			
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// DownContext
		///
		/// <summary>
		/// 選択されたコンテキストメニューを一つ下に移動します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type DownContext() {
			HTREEITEM target = TreeView_GetSelection(hto_);
			if (!target) return FALSE;
			
			HTREEITEM after = TreeView_GetNextSibling(hto_, target);
			if (!after) return FALSE;
			
			HTREEITEM newitem = this->CopyTreeViewItem(hto_, target, hto_, TreeView_GetParent(hto_, target), after);
			TreeView_DeleteItem(hto_, target);
			TreeView_SelectItem(hto_, newitem);
			
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// RenameContext
		///
		/// <summary>
		/// 選択されたコンテキストメニューの名前を変更します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type RenameContext() {
			HTREEITEM selected = TreeView_GetSelection(hto_);
			TreeView_EditLabel(hto_, selected);
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// ResetContext
		///
		/// <summary>
		/// コンテキストメニューへの修正をリセットし、現在保存されている
		/// 設定へ戻します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type ResetContext() {
			TreeView_DeleteAllItems(hfrom_);
			TreeView_DeleteAllItems(hto_);
			serialized_.clear();
			
			ContextContainer src;
			ConvertContext(0x7ffffb, src, data_.InstallDirectory());
			this->LoadContext(src, hfrom_, TVI_ROOT);
			TreeView_Select(hfrom_, TreeView_GetRoot(hfrom_), TVGN_CARET);
			
			this->LoadCurrentContext();
			
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// BeginDrag
		///
		/// <summary>
		/// 項目のドラッグ&ドロップを開始します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type BeginDrag(LPNMTREEVIEW item) {
			dragging_.first = item->hdr.hwndFrom;
			dragging_.second = item->itemNew.hItem;
			TreeView_SelectItem(dragging_.first, dragging_.second);
			
			HIMAGELIST image = TreeView_CreateDragImage(dragging_.first, dragging_.second);
			ImageList_BeginDrag(image, 0, 0, 0);
			::SetCapture(::GetParent(dragging_.first));
			::ClientToScreen(dragging_.first, &(item->ptDrag));
			ImageList_DragEnter(NULL, item->ptDrag.x, item->ptDrag.y);
			
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// EndDrag
		///
		/// <summary>
		/// 項目のドラッグ&ドロップを終了します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type EndDrag(const POINT& pt) {
			if (this->IsInsideArea(pt, hfrom_) && dragging_.first == hto_) {
				TreeView_DeleteItem(hto_, dragging_.second);
				return TRUE;
			}
			
			HTREEITEM root = TreeView_GetRoot(hto_);
			if (dragging_.second == root) return FALSE;
			
			TVHITTESTINFO hit = {};
			hit.flags = TVHT_ONITEM;
			hit.pt.x = pt.x;
			hit.pt.y = pt.y;
			::ScreenToClient(hto_, &hit.pt );
			HTREEITEM target = TreeView_HitTest(hto_, &hit);
			if (!target || target == dragging_.second || this->IsChild(hto_, target, dragging_.second) || (hit.flags & (TVHT_ONITEM | TVHT_ONITEMRIGHT)) == 0) return FALSE;
			
			TVITEM tvi = this->GetTreeViewItem(hto_, target);
			HTREEITEM child = TreeView_GetChild(hto_, target);
			hit.pt.y -= TreeView_GetItemHeight(hto_) / 2;
			hit.hItem = target;
			TreeView_HitTest(hto_, &hit);
			
			HTREEITEM result = NULL;
			int state = TreeView_GetItemState(hto_, target, TVIS_EXPANDED);
			int index = static_cast<int>(tvi.lParam);
			bool branch_node = (index >= 0 && index < serialized_.size()) ? !serialized_[index].Children().empty() : false;
			if ((target == root && ((state & TVIS_EXPANDED) == 0 && !child)) || (branch_node && target != hit.hItem)) {
				result = this->CopyTreeViewItem(dragging_.first, dragging_.second, hto_, target, TVI_LAST);
			}
			else if (target == root || (branch_node && (state & TVIS_EXPANDED) != 0 && child)) {
				result = this->CopyTreeViewItem(dragging_.first, dragging_.second, hto_, target, TVI_FIRST);
			}
			else {
				result = this->CopyTreeViewItem(dragging_.first, dragging_.second, hto_, TreeView_GetParent(hto_, target), target);
			}
			TreeView_SelectItem(hto_, result);
			
			if (dragging_.first == hto_) TreeView_DeleteItem(hto_, dragging_.second);
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// OnDragging
		///
		/// <summary>
		/// 項目のドラッグ&ドロップ中です。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type OnDragging(const POINT& pt) {
			HTREEITEM root = TreeView_GetRoot(hto_);
			if (dragging_.second == root) return FALSE;
			
			TVHITTESTINFO hit = {};
			hit.flags = TVHT_ONITEM;
			hit.pt.x = pt.x;
			hit.pt.y = pt.y;
			::ScreenToClient(hto_, &hit.pt);
			HTREEITEM target = TreeView_HitTest(hto_, &hit);
			
			if ((hit.flags & TVHT_ABOVE) != 0) ::SendMessage(hto_, WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
			if ((hit.flags & TVHT_BELOW) != 0) ::SendMessage(hto_, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
			if (!target || this->IsChild(hto_, target, dragging_.second) || (hit.flags & (TVHT_ONITEM | TVHT_ONITEMRIGHT)) == 0) return FALSE;
			
			TVITEM tvi = this->GetTreeViewItem(hto_, target);
			HTREEITEM child = TreeView_GetChild(hto_, target);
			hit.pt.y -= TreeView_GetItemHeight(hto_) / 2;
			hit.hItem = target;
			TreeView_HitTest(hto_, &hit);
			
			int state = TreeView_GetItemState(hto_, target, TVIS_EXPANDED);
			int index = static_cast<int>(tvi.lParam);
			bool branch_node = (index >= 0 && index < serialized_.size()) ? !serialized_[index].Children().empty() : false;
			if ((target == root && ((state & TVIS_EXPANDED) == 0 || !child)) || (branch_node && target != hit.hItem)) {
				TreeView_SelectDropTarget(hto_, target);
			}
			else if (target == root || (branch_node && (state & TVIS_EXPANDED) != 0 && child)) {
				TreeView_SetInsertMark(hto_, child, FALSE);
			}
			else {
				TreeView_SetInsertMark(hto_, target, FALSE); // NOTE: Windows XP で描画跡が残る問題を回避するため
				TreeView_SetInsertMark(hto_, target, TRUE);
			}
			return TRUE;
		}
		
	private:
		/* ----------------------------------------------------------------- */
		///
		/// IsChild
		///
		/// <summary>
		/// m1 が m2 の子要素かどうかチェックします。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		bool IsChild(handle_type handle, HTREEITEM m1, HTREEITEM m2) {
			HTREEITEM child = TreeView_GetChild(handle, m2);
			while (child) {
				if (m1 == child || IsChild(handle, m1, child)) return true;
				child = TreeView_GetNextSibling(handle, child);
			}
			return false;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// IsInsideArea
		///
		/// <summary>
		/// pt がコントロールの枠内に存在しているかどうかをチェックします。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		bool IsInsideArea(POINT pt, handle_type handle) {
			RECT rc = {};
			if (GetWindowRect(handle, &rc)) {
				if (pt.x >= rc.left && pt.x <= rc.right && pt.y >= rc.top && pt.y <= rc.bottom) return true;
			}
			return false;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// GetTreeViewItem
		///
		/// <summary>
		/// 指定された TreeView の項目を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		TV_ITEM GetTreeViewItem(handle_type handle, HTREEITEM item) {
			char_type buffer[2048] = {};
			TV_ITEM dest = {};
			dest.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			dest.hItem = item;
			dest.pszText = buffer;
			dest.cchTextMax = 1024;
			TreeView_GetItem(handle, &dest);
			return dest;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// CopyTreeViewItem
		///
		/// <summary>
		/// 使用可能なコンテキストメニュー一覧の中から選択された項目を
		/// 現在のコンテキストメニューへコピーします。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		HTREEITEM CopyTreeViewItem(handle_type hfrom, HTREEITEM from, handle_type hto, HTREEITEM to, const HTREEITEM& after) {
			char_type buffer[2048] = {};
			
			TVINSERTSTRUCT tvi = {};
			tvi.hParent = to;
			tvi.hInsertAfter = after;
			tvi.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			tvi.item.hItem = from;
			tvi.item.pszText = buffer;
			tvi.item.cchTextMax = sizeof (buffer) / sizeof (buffer[0]);
			
			TreeView_GetItem(hfrom, &tvi.item);
			tvi.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			
			HTREEITEM dest = TreeView_InsertItem(hto, &tvi);
			HTREEITEM child = TreeView_GetChild(hfrom, from);
			while (child) {
				CopyTreeViewItem(hfrom, child, hto, dest, TVI_LAST);
				child = TreeView_GetNextSibling(hfrom, child);
			}
			return dest;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// GetImageIndex
		///
		/// <summary>
		/// 表示するアイコンのインデックスを取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		int GetImageIndex(int index) {
			if (index >= static_cast<int>(serialized_.size())) return 0;
			
			string_type program = data_.InstallDirectory() + _T("\\") + CUBEICE_NAME;
			string_type path = serialized_[index].TargetPath();
			string_type args = serialized_[index].Arguments();
			if (path == program) {
				if (args.find(_T("/c")) != string_type::npos) return 1;
				else if (args.find(_T("/x")) != string_type::npos) return 2;
			}
			
			if (!serialized_[index].Children().empty()) return 3;
			return 0;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// GetSelectedImageIndex
		///
		/// <summary>
		/// 項目が選択状態の時に表示するアイコンのインデックスを取得します。
		/// 現状では、表示するアイコンがフォルダの場合のみ、別のアイコン
		/// (フォルダが開いてる状態のアイコン) を表示します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		int GetSelectedImageIndex(int index) {
			int dest = this->GetImageIndex(index);
			if (dest == 3) return 4;
			return dest;
		}
		
	private:
		/* ----------------------------------------------------------------- */
		///
		/// StaticEditProc
		///
		/// <summary>
		/// TreeView のラベルの編集中に Enter キーなどを押すと、他のボタン
		/// などにコントロールを奪われてしまい意図した挙動にならない。
		/// そのため、ラベルの編集が始まった段階でサブクラス化します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		static LRESULT CALLBACK StaticEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			switch (uMsg) {
			case WM_GETDLGCODE:
				return DLGC_WANTALLKEYS;
			}
			return ::CallWindowProc(editproc_, hWnd, uMsg, wParam, lParam);
		}
		
	private:
		UserSetting& data_;
		ContextContainer serialized_;
		handle_type hfrom_;
		handle_type hto_;
		HIMAGELIST himages_;
		HMENU hmenu_;
		bool editing_;
		std::pair<handle_type, HTREEITEM> dragging_;
		
	private:
		static WNDPROC editproc_;
	};
	
	WNDPROC CustomizeDialog::editproc_ = NULL;
	
} // namespace CubeICE

#endif // CUBEICE_SETTING_CUSTOMIZE_DIALOG_H
