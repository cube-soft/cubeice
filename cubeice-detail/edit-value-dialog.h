// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice-detail/common-edit-dialog.h
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
#ifndef CUBEICE_DETAIL_COMMON_EDIT_DIALOG_H
#define CUBEICE_DETAIL_COMMON_EDIT_DIALOG_H

#include <cubeice/config.h>
#include <psdotnet/forms.h>
#include "listview-helper.h"
#include "resource.h"

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	/// EditDialog
	/* --------------------------------------------------------------------- */
	class EditDialog : public PsdotNet::Forms::ModalForm {
	private:
		typedef PsdotNet::Forms::ModalForm super;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		EditDialog(const string_type& name, const string_type& kind, const string_type& value) :
			super(_T("IDD_EDIT")),
			hname_(NULL), hkind_(NULL), hvalue_(NULL),
			name_(name), kind_(kind), value_(value) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~EditDialog() {}
		
		/* ----------------------------------------------------------------- */
		/// Value
		/* ----------------------------------------------------------------- */
		const string_type& Value() const { return value_; }
		
	protected:
		/* ----------------------------------------------------------------- */
		/// OnCreateControl
		/* ----------------------------------------------------------------- */
		virtual void OnCreateControl() {
			hname_ = ::GetDlgItem(this->Handle(), IDC_EDIT_NAME);
			::SendMessage(hname_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(name_.c_str()));
			
			hkind_ = ::GetDlgItem(this->Handle(), IDC_EDIT_KIND);
			::SendMessage(hkind_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(kind_.c_str()));
			
			hvalue_ = ::GetDlgItem(this->Handle(), IDC_EDIT_VALUE);
			::SendMessage(hvalue_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(value_.c_str()));
			if (GetValueKind(kind_) == PsdotNet::Parameter::ValueKind::Number) this->AllowOnlyNumber();
			
			super::OnCreateControl();
		}
		
		/* ----------------------------------------------------------------- */
		/// OnCommand
		/* ----------------------------------------------------------------- */
		virtual void OnCommand(PsdotNet::Forms::Message& m) {
			switch (m.WParam()) {
			case IDC_EDIT_VALUE | (EN_KILLFOCUS << 16):
				this->OnKillFocus();
				m.Result(FALSE);
				break;
			default:
				break;
			}
			super::OnCommand(m);
		}
		
	private:
		/* ----------------------------------------------------------------- */
		/// OnKillFocus
		/* ----------------------------------------------------------------- */
		void OnKillFocus() {
			value_ = PsdotNet::Forms::Utility::GetText(hvalue_);
		}
		
		/* ----------------------------------------------------------------- */
		/// AllowOnlyNumber
		/* ----------------------------------------------------------------- */
		void AllowOnlyNumber() {
			LONG style = ::GetWindowLong(hvalue_, GWL_STYLE);
			::SetWindowLong(hvalue_, GWL_STYLE, style | ES_NUMBER & ~(ES_LOWERCASE | ES_UPPERCASE));
		}
		
	private:
		handle_type hname_;
		handle_type hkind_;
		handle_type hvalue_;
		string_type name_;
		string_type kind_;
		string_type value_;
	}; // class EditDialog
} // namespace CubeICE

#endif // CUBEICE_DETAIL_EDIT_DIALOG_H
