// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-detail/context-edit-dialog.h
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
#ifndef CUBEICE_DETAIL_CONTEXT_EDIT_DIALOG_H
#define CUBEICE_DETAIL_CONTEXT_EDIT_DIALOG_H

#include <cubeice/config.h>
#include <psdotnet/forms.h>
#include "resource.h"

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	/// EditContextDialog
	/* --------------------------------------------------------------------- */
	class EditContextDialog : public PsdotNet::Forms::ModalForm {
	private:
		typedef PsdotNet::Forms::ModalForm super;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		EditContextDialog() :
			super(_T("IDD_EDITCONTEXT")),
			kind_(0), selectable_(true),
			index_(0), name_(),
			target_(), arguments_(), icon_(),
			hkind_(NULL), hindex_(NULL), hname_(NULL), htarget_(NULL), harguments_(NULL), hicon_(NULL) {}
		
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		explicit EditContextDialog(const Context& cx) :
			super(_T("IDD_EDITCONTEXT")),
			kind_(0), selectable_(true),
			index_(cx.Index()), name_(cx.Name()),
			target_(cx.TargetPath()), arguments_(cx.Arguments()), icon_(cx.IconLocation()),
			hkind_(NULL), hindex_(NULL), hname_(NULL), htarget_(NULL), harguments_(NULL), hicon_(NULL) {}
		
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		EditContextDialog(int kind, const Context& cx) :
			super(_T("IDD_EDITCONTEXT")),
			kind_(kind), selectable_(false),
			index_(cx.Index()), name_(cx.Name()),
			target_(cx.TargetPath()), arguments_(cx.Arguments()), icon_(cx.IconLocation()),
			hkind_(NULL), hindex_(NULL), hname_(NULL), htarget_(NULL), harguments_(NULL), hicon_(NULL) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~EditContextDialog() {}
		
		/* ----------------------------------------------------------------- */
		/// Kind
		/* ----------------------------------------------------------------- */
		const int& Kind() const { return kind_; }
		
		/* ----------------------------------------------------------------- */
		/// Index
		/* ----------------------------------------------------------------- */
		const int& Index() const { return index_; }
		
		/* ----------------------------------------------------------------- */
		/// Name
		/* ----------------------------------------------------------------- */
		const string_type& Name() const { return name_; }
		
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
			hkind_ = ::GetDlgItem(this->Handle(), IDC_EDITCONTEXT_KIND);
			::SendMessage(hkind_, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("Context")));
			::SendMessage(hkind_, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T("DragDrop")));
			::SendMessage(hkind_, CB_SETCURSEL, kind_, 0);
			::EnableWindow(hkind_, selectable_ ? TRUE : FALSE);
			
			hindex_ = ::GetDlgItem(this->Handle(), IDC_EDITCONTEXT_INDEX);
			::SendMessage(hindex_, UDM_SETRANGE, 0, MAKELONG(999, 0));
			::SendMessage(hindex_, UDM_SETPOS, 0, index_);
			
			hname_ = ::GetDlgItem(this->Handle(), IDC_EDITCONTEXT_NAME);
			::SendMessage(hname_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(name_.c_str()));
			
			htarget_ = ::GetDlgItem(this->Handle(), IDC_EDITCONTEXT_TARGET);
			::SendMessage(htarget_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(target_.c_str()));
			
			harguments_ = ::GetDlgItem(this->Handle(), IDC_EDITCONTEXT_ARGUMENTS);
			::SendMessage(harguments_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(arguments_.c_str()));
			
			hicon_ = ::GetDlgItem(this->Handle(), IDC_EDITCONTEXT_ICON);
			::SendMessage(hicon_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(icon_.c_str()));
			
			super::OnCreateControl();
		}
		
		/* ----------------------------------------------------------------- */
		/// OnOK
		/* ----------------------------------------------------------------- */
		virtual void OnOK() {
			kind_ = static_cast<int>(::SendMessage(hkind_, CB_GETCURSEL, 0, 0));
			index_ = (::SendMessage(hindex_, UDM_GETPOS, 0, 0) & 0xffff);
			name_ = PsdotNet::Forms::Utility::GetWindowText(hname_);
			target_ = PsdotNet::Forms::Utility::GetWindowText(htarget_);
			arguments_ = PsdotNet::Forms::Utility::GetWindowText(harguments_);
			icon_ = PsdotNet::Forms::Utility::GetWindowText(hicon_);
			
			super::OnOK();
		}
		
	private:
		int kind_;
		bool selectable_;
		
		int index_;
		string_type name_;
		string_type target_;
		string_type arguments_;
		string_type icon_;
		
		handle_type hkind_;
		handle_type hindex_;
		handle_type hname_;
		handle_type htarget_;
		handle_type harguments_;
		handle_type hicon_;
	}; // class EditContextDialog
} // namespace CubeICE

#endif // CUBEICE_DETAIL_CONTEXT_EDIT_DIALOG_H
