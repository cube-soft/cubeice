// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-detail/edit-shortcut-dialog.h
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
#ifndef CUBEICE_DETAIL_EDIT_SHORTCUT_DIALOG_H
#define CUBEICE_DETAIL_EDIT_SHORTCUT_DIALOG_H

#include <cubeice/config.h>
#include <cubeice/shortcut.h>
#include <psdotnet/forms.h>
#include "resource.h"

namespace CubeICE {
	class EditShortcutDialog : public PsdotNet::Forms::ModalForm {
	private:
		typedef PsdotNet::Forms::ModalForm super;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		EditShortcutDialog() :
			super(_T("IDD_EDITSHORTCUT")),
			name_(), directory_(),
			target_(), arguments_(), icon_(),
			hname_(NULL), hdirectory_(NULL),
			htarget_(NULL), harguments_(NULL), hicon_(NULL) {}
		
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		explicit EditShortcutDialog(Shortcut& sc) :
			super(_T("IDD_EDITSHORTCUT")),
			name_(sc.Name()), directory_(sc.Directory()),
			target_(sc.TargetPath()), arguments_(sc.Arguments()), icon_(sc.IconLocation()),
			hname_(NULL), hdirectory_(NULL),
			htarget_(NULL), harguments_(NULL), hicon_(NULL) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~EditShortcutDialog() {}
		
		/* ----------------------------------------------------------------- */
		/// Name
		/* ----------------------------------------------------------------- */
		const string_type& Name() const { return name_; }
		
		/* ----------------------------------------------------------------- */
		/// Directory
		/* ----------------------------------------------------------------- */
		const string_type& Directory() const { return directory_; }
		
		/* ----------------------------------------------------------------- */
		/// TargetPath
		/* ----------------------------------------------------------------- */
		const string_type& TargetPath() const { return target_; }
		
		/* ----------------------------------------------------------------- */
		/// Arguments
		/* ----------------------------------------------------------------- */
		const string_type& Arguments() const { return arguments_; }
		
		/* ----------------------------------------------------------------- */
		/// IconLocation
		/* ----------------------------------------------------------------- */
		const string_type& IconLocation() const { return icon_; }
		
	protected:
		/* ----------------------------------------------------------------- */
		/// OnCreateControl
		/* ----------------------------------------------------------------- */
		virtual void OnCreateControl() {
			hname_ = ::GetDlgItem(this->Handle(), IDC_EDITSHORTCUT_NAME);
			::SendMessage(hname_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(name_.c_str()));
			
			hdirectory_ = ::GetDlgItem(this->Handle(), IDC_EDITSHORTCUT_DIRECTORY);
			::SendMessage(hdirectory_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(directory_.c_str()));
			
			htarget_ = ::GetDlgItem(this->Handle(), IDC_EDITSHORTCUT_TARGET);
			::SendMessage(htarget_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(target_.c_str()));
			
			harguments_ = ::GetDlgItem(this->Handle(), IDC_EDITSHORTCUT_ARGUMENTS);
			::SendMessage(harguments_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(arguments_.c_str()));
			
			hicon_ = ::GetDlgItem(this->Handle(), IDC_EDITSHORTCUT_ICON);
			::SendMessage(hicon_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(icon_.c_str()));
			
			super::OnCreateControl();
		}
		
		/* ----------------------------------------------------------------- */
		/// OnOK
		/* ----------------------------------------------------------------- */
		virtual void OnOK() {
			name_ = PsdotNet::Forms::Utility::GetText(hname_);
			directory_ = PsdotNet::Forms::Utility::GetText(hdirectory_);
			target_ = PsdotNet::Forms::Utility::GetText(htarget_);
			arguments_ = PsdotNet::Forms::Utility::GetText(harguments_);
			icon_ = PsdotNet::Forms::Utility::GetText(hicon_);
		}
		
	private:
		string_type name_;
		string_type directory_;
		string_type target_;
		string_type arguments_;
		string_type icon_;
		
		handle_type hname_;
		handle_type hdirectory_;
		handle_type htarget_;
		handle_type harguments_;
		handle_type hicon_;

	}; // class EditShortcutDialog
} // namespace CubeICE

#endif // CUBEICE_DETAIL_EDIT_SHORTCUT_DIALOG_H
