// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-setting/general-page.h
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
#ifndef CUBEICE_SETTING_GENERAL_SETTING_PAGE_H
#define CUBEICE_SETTING_GENERAL_SETTING_PAGE_H

#include "cubeice-setting.h"
#include <vector>
#include <map>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <psdotnet/forms/utility.h>
#include "setting-page.h"
#include "customize-dialog.h"

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// GeneralPage
	///
	/// <summary>
	/// SettingDialog に表示させる「一般」ページです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class GeneralPage : public SettingPage {
	private:
		typedef SettingPage super;
		typedef std::map<int, string_type> assoc_map;
		typedef std::map<int, int> cx_map;
		typedef std::map<int, Shortcut> sc_map;
		typedef std::vector<boost::tuple<string_type, string_type, bool> > type_list;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		GeneralPage(UserSetting& data, int_ptr changed) :
			super(_T("IDD_GENERAL"), data, changed),
			association_(), root_(), compression_(), decompression_(), mail_(), shortcut_(), type_() {
			this->InitializeComponent();
		}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~GeneralPage() {}
		
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
			LOG_DEBUG(_T("GeneralPage::OnCreateControl"));
			
			this->LoadAssociation();
			this->LoadContext();
			this->LoadShortcut();
			this->LoadDefaultType();
		}
		
		/* ----------------------------------------------------------------- */
		/// OnCommand
		/* ----------------------------------------------------------------- */
		virtual void OnCommand(PsdotNet::Forms::Message& m) {
			int id = LOWORD(m.WParam());
			if (this->SaveAssociation(id) || this->SaveContext(id) || this->SaveShortcut(id)) {
				m.Result(TRUE);
				return;
			}
			
			// その他のイベント
			switch (id) {
			case IDC_ASSOC_CHECKALL:
				m.Result(this->SaveAssociation(true));
				break;
			case IDC_ASSOC_RESETALL:
				m.Result(this->SaveAssociation(false));
				break;
			case IDC_CX_CUSTOMIZE:
				m.Result(this->EnableExtendedContext());
				break;
			case IDC_CX_RESET:
				m.Result(this->DisableExtendedContext());
				break;
			case IDC_DETAIL_DEFAULTTYPE:
				m.Result(this->SaveDefaultType());
				break;
			default:
				super::OnCommand(m);
				break;
			}
		}
		
	private:
		/* ----------------------------------------------------------------- */
		///
		/// LoadAssociation
		///
		/// <summary>
		/// 現在の関連付けの設定を画面に表示します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type LoadAssociation() {
			UserAssociationSetting& assoc = this->Data().Association();
			BOOST_FOREACH(assoc_map::value_type& item, association_) {
				if (!assoc.Find(item.second).Value<PsdotNet::Parameter::ValueKind::Bool>()) continue;
				::CheckDlgButton(this->Handle(), item.first, BST_CHECKED);
			}
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// LoadContext
		///
		/// <summary>
		/// 現在のコンテキストメニューの設定を画面に表示します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type LoadContext() {
			int flags = this->Data().Context().Builtin();
			bool extended = this->Data().Context().IsExtended();
			
			bool enabled = !extended;
			BOOST_FOREACH(cx_map::value_type& item, root_) {
				bool checked = (flags & item.second) != 0;
				this->SetCheckCondition(item.first, checked, enabled);
			}
			
			enabled = !extended && (flags & ContextKind::Compression) != 0;
			BOOST_FOREACH(cx_map::value_type& item, compression_) {
				bool checked = (flags & item.second) != 0;
				this->SetCheckCondition(item.first, checked, enabled);
			}
			
			enabled = !extended && (flags & ContextKind::Decompression) != 0;
			BOOST_FOREACH(cx_map::value_type& item, decompression_) {
				bool checked = (flags & item.second) != 0;
				this->SetCheckCondition(item.first, checked, enabled);
			}
			
			enabled = !extended && (flags & ContextKind::Mail) != 0;
			BOOST_FOREACH(cx_map::value_type& item, mail_) {
				bool checked = (flags & item.second) != 0;
				this->SetCheckCondition(item.first, checked, enabled);
			}
			
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// LoadShortcut
		///
		/// <summary>
		/// 現在のショートカットの設定を画面に表示します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type LoadShortcut() {
			UserShortcutSetting& sc = this->Data().Shortcut();
			BOOST_FOREACH(sc_map::value_type& item, shortcut_) {
				if (!sc.GetOptional(item.second.Name())) continue;
				::CheckDlgButton(this->Handle(), item.first, BST_CHECKED);
			}
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// LoadDefaultType
		///
		/// <summary>
		/// デフォルトの圧縮形式として選択可能な一覧をコンボボックスに
		/// 挿入します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type LoadDefaultType() {
			UserCompressionSetting& data = this->Data().Compression();
			
			int index = 0;
			handle_type handle = ::GetDlgItem(this->Handle(), IDC_DETAIL_DEFAULTTYPE);
			for (type_list::size_type i = 0; i < type_.size(); ++i) {
				::SendMessage(handle, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(type_[i].get<0>().c_str()));
				if (type_[i].get<1>() == data.DefaultType() && type_[i].get<2>() == data.DefaultEnablePassword()) index = i;
			}
			::SendMessage(handle, CB_SETCURSEL, index, 0);
			
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveAssociation
		///
		/// <summary>
		/// 画面上の関連付けの設定をデータに反映します。設定が変更された
		/// 場合、ChangeKind::RequireAdmin を設定して管理者権限での保存
		/// 処理が必要な事を通知します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type SaveAssociation(int id) {
			assoc_map::iterator pos = association_.find(id);
			if (pos == association_.end()) return FALSE;
			
			LOG_DEBUG(_T("GeneralPage::SaveAssociation"));
			bool checked = ::IsDlgButtonChecked(this->Handle(), id) == BST_CHECKED;
			this->Data().Association().Find(pos->second).Value(checked);
			this->Changed(ChangeKind::RequireAdmin);
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveAssociation
		///
		/// <summary>
		/// 画面上の全ての関連付けの設定を引数に指定した状態に変更します。
		/// 設定が変更された場合、ChangeKind::RequireAdmin を設定して
		/// 管理者権限での保存処理が必要な事を通知します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type SaveAssociation(bool enable) {
			LOG_DEBUG(_T("GeneralPage::SaveAssociation(%s)"), (enable ? _T("CheckAll") : _T("ResetAll")));
			UserAssociationSetting& assoc = this->Data().Association();
			BOOST_FOREACH(assoc_map::value_type& item, association_) {
				::CheckDlgButton(this->Handle(), item.first, enable ? BST_CHECKED : BST_UNCHECKED);
				assoc.Find(item.second).Value(enable);
			}
			this->Changed(ChangeKind::RequireAdmin);
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveContext
		///
		/// <summary>
		/// 画面のコンテキストメニューの設定をデータに反映します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type SaveContext(int id) {
			switch (id) {
			case IDC_CX_COMPRESSION:
				return this->SaveContext(id, compression_);
			case IDC_CX_DECOMPRESSION:
				return this->SaveContext(id, decompression_);
			case IDC_CX_MAIL:
				return this->SaveContext(id, mail_);
			default:
				return this->SaveContextSub(id);
			}
			return FALSE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveContext
		///
		/// <summary>
		/// 画面のコンテキストメニューの設定をデータに反映します。
		/// サブメニューは、ルートメニューの選択状況によって選択可能かどうか
		/// が変化します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type SaveContext(int id, cx_map& submenu) {
			LOG_DEBUG(_T("GeneralPage::SaveContext(%d, submenu)"), id);
			bool checked = ::IsDlgButtonChecked(this->Handle(), id) == BST_CHECKED;
			int flags = this->Data().Context().Builtin();
			if (checked) flags |= root_[id];
			else flags &= ~root_[id];
			this->Data().Context().Builtin(flags);
			
			bool enabled = checked && !this->Data().Context().IsExtended();
			BOOST_FOREACH(cx_map::value_type& item, submenu) {
				::EnableWindow(::GetDlgItem(this->Handle(), item.first), enabled ? TRUE : FALSE);
			}
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveContextSub
		///
		/// <summary>
		/// 画面のコンテキストメニューのサブメニュー部分の設定をデータに
		/// 反映します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type SaveContextSub(int id) {
			bool found = false;
			cx_map::iterator pos = compression_.find(id);
			if (!found && pos != compression_.end()) found = true;
			else pos = decompression_.find(id);
			if (!found && pos != decompression_.end()) found = true;
			else pos = mail_.find(id);
			if (!found) return FALSE;
			
			LOG_DEBUG(_T("GeneralPage::SaveContextSub"));
			bool checked = ::IsDlgButtonChecked(this->Handle(), id) == BST_CHECKED;
			int flags = this->Data().Context().Builtin();
			if (checked) flags |= pos->second;
			else flags &= ~(pos->second);
			this->Data().Context().Builtin(flags);
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// EnableExtendedContext
		///
		/// <summary>
		/// 拡張コンテキストメニューを使用し、組み込みのコンテキストメニュー
		/// は設定不可能にします。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type EnableExtendedContext() {
			LOG_DEBUG(_T("GeneralPage::EnableExtendedContext"));
			CustomizeDialog dialog(this->Data());
			dialog.Icon(PsdotNet::Drawing::Icon(PsdotNet::Drawing::Icon::Type::Resource, _T("IDI_APP")));
			if (dialog.ShowDialog(*this) == PsdotNet::Forms::DialogResult::OK) {
				LOG_TRACE(_T("ExtendedContext enabled"));
				this->Data().Context().IsExtended(true);
				this->LoadContext();
				return TRUE;
			}
			return FALSE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// DisableExtendedContext
		///
		/// <summary>
		/// 拡張コンテキストメニューの使用を中止し、組み込みのコンテキスト
		/// メニューを設定可能にします。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type DisableExtendedContext() {
			LOG_DEBUG(_T("GeneralPage::DisableExtendedContext"));
			this->Data().Context().IsExtended(false);
			this->LoadContext();
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveShortcut
		///
		/// <summary>
		/// 画面のショートカットの設定をデータに反映します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type SaveShortcut(int id) {
			LOG_DEBUG(_T("GeneralPage::SaveShortcut"));
			sc_map::iterator pos = shortcut_.find(id);
			if (pos == shortcut_.end()) return FALSE;
			
			bool checked = ::IsDlgButtonChecked(this->Handle(), id) == BST_CHECKED;
			LOG_TRACE(_T("Shortcut: %s (%s)"), pos->second.Name().c_str(), (checked ? _T("Checked") : _T("Unchecked")));
			this->Data().Shortcut().Delete(pos->second.Name());
			if (checked) this->Data().Shortcut().Add(pos->second);
			return TRUE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveDefaultType
		///
		/// <summary>
		/// デフォルトの圧縮形式とデフォルトでパスワードを設定するかどうか
		/// を更新します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		result_type SaveDefaultType() {
			LOG_DEBUG(_T("GeneralPage::SaveDefaultType"));
			string_type name = PsdotNet::Forms::Utility::GetText(::GetDlgItem(this->Handle(), IDC_DETAIL_DEFAULTTYPE));
			BOOST_FOREACH(type_list::value_type& item, type_) {
				if (item.get<0>() != name) continue;
				this->Data().Compression().DefaultType(item.get<1>());
				this->Data().Compression().DefaultEnablePassword(item.get<2>());
				LOG_TRACE(_T("DefaultType: %s"), name.c_str());
				return TRUE;
			}
			return FALSE;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SetCheckCondition
		///
		/// <summary>
		/// チェックボックスの状態を選択/非選択、および有効/無効に設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void SetCheckCondition(int id, bool checked, bool enabled) {
			::CheckDlgButton(this->Handle(), id, checked ? BST_CHECKED : BST_UNCHECKED);
			::EnableWindow(::GetDlgItem(this->Handle(), id), enabled ? TRUE : FALSE);
		}
		
	private:
		/* ----------------------------------------------------------------- */
		///
		/// InitializeComponent
		///
		/// <summary>
		/// 初期処理を行います。ここでは、主に、画面の各種項目 ID と
		/// UserSetting で保持しているデータのマッピングを行います。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void InitializeComponent() {
			// 関連付けのウィンドウ ID と文字列のマッピング
			association_[IDC_ASSOC_ZIP]  = _T("zip");
			association_[IDC_ASSOC_LZH]  = _T("lzh");
			association_[IDC_ASSOC_RAR]  = _T("rar");
			association_[IDC_ASSOC_TAR]  = _T("tar");
			association_[IDC_ASSOC_GZ]   = _T("gz");
			association_[IDC_ASSOC_7Z]   = _T("7z");
			association_[IDC_ASSOC_ARJ]  = _T("arj");
			association_[IDC_ASSOC_BZ2]  = _T("bz2");
			association_[IDC_ASSOC_CAB]  = _T("cab");
			association_[IDC_ASSOC_CHM]  = _T("chm");
			association_[IDC_ASSOC_CPIO] = _T("cpio");
			association_[IDC_ASSOC_DEB]  = _T("deb");
			association_[IDC_ASSOC_DMG]  = _T("dmg");
			association_[IDC_ASSOC_ISO]  = _T("iso");
			association_[IDC_ASSOC_RPM]  = _T("rpm");
			association_[IDC_ASSOC_TBZ]  = _T("tbz");
			association_[IDC_ASSOC_TGZ]  = _T("tgz");
			association_[IDC_ASSOC_WIM]  = _T("wim");
			association_[IDC_ASSOC_XAR]  = _T("xar");
			association_[IDC_ASSOC_XZ]   = _T("xz");
			association_[IDC_ASSOC_JAR]  = _T("jar");
			
			// コンテキストメニューのマッピング
			root_[IDC_CX_COMPRESSION]   = ContextKind::Compression;
			root_[IDC_CX_DECOMPRESSION] = ContextKind::Decompression;
			root_[IDC_CX_MAIL]          = ContextKind::Mail;
			
			// 「圧縮」コンテキストメニューのマッピング
			compression_[IDC_CX_ZIP]    = ContextKind::CompressionZip;
			compression_[IDC_CX_ZIP_P]  = ContextKind::CompressionZipPassword;
			compression_[IDC_CX_7Z]     = ContextKind::Compression7z;
			compression_[IDC_CX_BZIP2]  = ContextKind::CompressionBzip2;
			compression_[IDC_CX_GZIP]   = ContextKind::CompressionGzip;
			compression_[IDC_CX_EXE]    = ContextKind::CompressionSelfExecutable;
			compression_[IDC_CX_DETAIL] = ContextKind::CompressionDetail;
			
			// 「解凍」コンテキストメニューのマッピング
			decompression_[IDC_CX_HERE]        = ContextKind::DecompressionHere;
			decompression_[IDC_CX_DESKTOP]     = ContextKind::DecompressionDesktop;
			decompression_[IDC_CX_RUNTIME]     = ContextKind::DecompressionRuntime;
			decompression_[IDC_CX_MYDOCUMENTS] = ContextKind::DecompressionMyDocuments;
			
			// 「圧縮してメール送信」コンテキストメニューのマッピング
			mail_[IDC_CX_ZIP_M]    = ContextKind::MailZip;
			mail_[IDC_CX_ZIP_P_M]  = ContextKind::MailZipPassword;
			mail_[IDC_CX_7Z_M]     = ContextKind::Mail7z;
			mail_[IDC_CX_BZIP2_M]  = ContextKind::MailBzip2;
			mail_[IDC_CX_GZIP_M]   = ContextKind::MailGzip;
			mail_[IDC_CX_EXE_M]    = ContextKind::MailSelfExecutable;
			mail_[IDC_CX_DETAIL_M] = ContextKind::MailDetail;
			
			// ショーカットのマッピング
			string_type desktop = PsdotNet::Environment::GetFolderPath(PsdotNet::Environment::SpecialFolder::DesktopDirectory);
			string_type program = this->Data().InstallDirectory() + _T("\\") + CUBEICE_NAME;
			shortcut_[IDC_SC_COMPRESSION] = Shortcut(_T("CubeICE 圧縮"), desktop, program, _T("/c"), program + _T(",1"));
			shortcut_[IDC_SC_DECOMPRESSION] = Shortcut(_T("CubeICE 解凍"), desktop, program, _T("/x"), program + _T(",2"));
			program = this->Data().InstallDirectory() + _T("\\") + CUBEICE_SETTING_NAME;
			shortcut_[IDC_SC_SETTING] = Shortcut(_T("CubeICE 設定"), desktop, program, _T(""), program + _T(",0"));
			
			// デフォルトの圧縮形式のマッピング
			type_.push_back(boost::make_tuple(_T("zip"), _T("zip"), false));
			type_.push_back(boost::make_tuple(_T("zip (パスワード)"), _T("zip"), true));
			type_.push_back(boost::make_tuple(_T("7z"), _T("7z"), false));
			type_.push_back(boost::make_tuple(_T("bzip2"), _T("bzip2"), false));
			type_.push_back(boost::make_tuple(_T("gzip"), _T("gzip"), false));
			type_.push_back(boost::make_tuple(_T("exe"), _T("exe"), false));
			type_.push_back(boost::make_tuple(_T("詳細設定"), _T("detail"), false));
		}
		
	private:
		assoc_map association_;
		cx_map root_;
		cx_map compression_;
		cx_map decompression_;
		cx_map mail_;
		sc_map shortcut_;
		type_list type_;
	}; // class GeneralPage
} // namespace CubeICE

#endif // CUBEICE_SETTING_GENERAL_SETTING_PAGE_H
