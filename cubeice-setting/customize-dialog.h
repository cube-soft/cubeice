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
			hfrom_(NULL), hto_(NULL), himages_(NULL), hmenu_(NULL) {}
		
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
			hfrom_ = ::GetDlgItem(this->Handle(), IDC_ADD_TREEVIEW);
			hto_ = ::GetDlgItem(this->Handle(), IDC_CURRENT_TREEVIEW);
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
		
	private:
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
			if (index >= serialized_.size()) return 0;
			
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
		UserSetting& data_;
		ContextContainer serialized_;
		handle_type hfrom_;
		handle_type hto_;
		HIMAGELIST himages_;
		HMENU hmenu_;
	};
} // namespace CubeICE

#endif // CUBEICE_SETTING_CUSTOMIZE_DIALOG_H
