// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/user-setting/user-compression-setting.h
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
#ifndef CUBEICE_USER_SETTING_USER_COMPRESSION_SETTING_H
#define CUBEICE_USER_SETTING_USER_COMPRESSION_SETTING_H

#include <cubeice/config.h>
#include <cubeice/user-setting/user-setting-base.h>
#include <cubeice/user-setting/user-setting-value.h>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// UserCompressionSetting
	///
	/// <summary>
	/// CubeICE 圧縮に関連する各種ユーザ設定を取得・保存するクラスです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class UserCompressionSetting : public UserSettingBase {
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		UserCompressionSetting() :
			UserSettingBase() {
			this->Reset();
		}
		
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		UserCompressionSetting(const UserCompressionSetting& cp) :
			UserSettingBase(cp) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~UserCompressionSetting() {}
		
		/* ----------------------------------------------------------------- */
		///
		/// Reset
		///
		/// <summary>
		/// CubeICE の圧縮に関連する設定可能な項目をここで定義します。
		/// 設定項目ごとに、名前、および初期値を定義します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void Reset() {
			UserSettingBase::Reset();
			this->Parameters().push_back(Node(_T("OutputCondition"), OutputKind::Specific));
			this->Parameters().push_back(Node(_T("OutputPath"), _T("")));
			this->Parameters().push_back(Node(_T("DefaultType"), _T("zip")));
			this->Parameters().push_back(Node(_T("DefaultEnablePassword"), false));
			this->Parameters().push_back(Node(_T("Overwrite"), OverwriteKind::Confirm));
			this->Parameters().push_back(Node(_T("Filtering"), false));
			this->Parameters().push_back(Node(_T("Open"), OpenKind::Skip));
			this->Parameters().push_back(Node(_T("DeleteOnMail"), false));
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Upgrade
		///
		/// <summary>
		/// 旧バージョンのデータ構造から現在のものに変換します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void Upgrade(const Version1& old) {
			this->OutputCondition(old.compression().output_condition());
			this->OutputPath(old.compression().output_path());
			
			int details = old.compression().details();
			if ((details & DETAIL_OVERWRITE) != 0) this->Overwrite(OverwriteKind::Confirm);
			else this->Overwrite(OverwriteKind::Force);
			
			if ((details & DETAIL_OPEN) == 0) this->Open(OpenKind::Skip);
			else if ((details & DETAIL_SKIP_DESKTOP) != 0) this->Open(OpenKind::SkipDesktop);
			else this->Open(OpenKind::Open);
			
			this->Filtering((details & DETAIL_FILTER) != 0);
			this->DeleteOnMail((details & DETAIL_MAIL_REMOVE) != 0);
			
			switch (old.shortcut_compress_index()) {
			case 0:
			case 1:
				this->DefaultType(_T("zip"));
				break;
			case 2:
				this->DefaultType(_T("7z"));
				break;
			case 3:
				this->DefaultType(_T("gz"));
				break;
			case 4:
				this->DefaultType(_T("bz2"));
				break;
			
			}
			this->DefaultEnablePassword(old.shortcut_compress_index() == 1);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// OutputCondition
		///
		/// <summary>
		/// 圧縮ファイルの保存先の決定方法を取得します。
		/// 決定方法の詳細については、CubeICE::OutputKind を参照下さい。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const int& OutputCondition() const {
			return this->Find(_T("OutputCondition")).Value<ValueKind::Number>();
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  OutputCondition
		 *
		 *  圧縮ファイルの保存先の決定方法を設定します。
		 *  決定方法の詳細については、CubeICE::OutputKind を参照下さい。
		 */
		/* ----------------------------------------------------------------- */
		void OutputCondition(int condition) {
			this->Find(_T("OutputCondition")).Value<ValueKind::Number>() = condition;
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  OutputPath
		 *
		 *  圧縮ファイルの保存先パスを取得します。
		 *  OutputPath() は、OutputCondition() 関数が OutputKind::Specific
		 *  に設定されている時のみ使用されます。取得できる値が空文字の場合、
		 *  デスクトップを保存先とします。
		 */
		/* ----------------------------------------------------------------- */
		const string_type& OutputPath() const {
			return this->Find(_T("OutputPath")).Value<ValueKind::String>();
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  OutputPath
		 *
		 *  圧縮ファイルの保存先パスを設定します。
		 *  OutputPath() は、OutputCondition() 関数が OutputKind::Specific
		 *  に設定されている時のみ使用されます。取得できる値が空文字の場合、
		 *  デスクトップを保存先とします。
		 */
		/* ----------------------------------------------------------------- */
		void OutputPath(const string_type& path) {
			this->Find(_T("OutputPath")).Value<ValueKind::String>() = path;
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  DefaultType
		 *
		 *  「ここに圧縮」など、明示的に圧縮形式を指定されなかった場合に
		 *  使用される圧縮形式を取得します。
		 */
		/* ----------------------------------------------------------------- */
		const string_type& DefaultType() const {
			return this->Find(_T("DefaultType")).Value<ValueKind::String>();
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  DefaultType
		 *
		 *  「ここに圧縮」など、明示的に圧縮形式を指定されなかった場合に
		 *  使用される圧縮形式を設定します。
		 */
		/* ----------------------------------------------------------------- */
		void DefaultType(const string_type& type) {
			this->Find(_T("DefaultType")).Value<ValueKind::String>() = type;
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  DefaultEnablePassword
		 *
		 *  「ここに圧縮」など、明示的に圧縮形式を指定されなかった場合に
		 *  パスワードを設定するかどうかを取得します。DefaultType() で設定
		 *  されている圧縮形式がパスワードによる暗号化に対応していない場合、
		 *  この設定は無視されます。
		 */
		/* ----------------------------------------------------------------- */
		const bool& DefaultEnablePassword() const {
			return this->Find(_T("DefaultEnablePassword")).Value<ValueKind::Bool>();
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  DefaultEnablePassword
		 *
		 *  「ここに圧縮」など、明示的に圧縮形式を指定されなかった場合に
		 *  パスワードを設定するかどうかを設定します。DefaultType() で設定
		 *  されている圧縮形式がパスワードによる暗号化に対応していない場合、
		 *  この設定は無視されます。
		 */
		/* ----------------------------------------------------------------- */
		void DefaultEnablePassword(bool enable) {
			this->Find(_T("DefaultEnablePassword")).Value<ValueKind::Bool>() = enable;
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  Overwrite
		 *
		 *  同名の圧縮ファイルが存在する場合に、上書き確認用のダイアログを
		 *  表示するかどうかを取得します。
		 *  詳細については、CubeICE::OverwriteKind を参照下さい。
		 */
		/* ----------------------------------------------------------------- */
		const int& Overwrite() const {
			return this->Find(_T("Overwrite")).Value<ValueKind::Number>();
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  Overwrite
		 *
		 *  同名の圧縮ファイルが存在する場合に、上書き確認用のダイアログを
		 *  表示するかどうかを設定します。
		 *  詳細については、CubeICE::OverwriteKind を参照下さい。
		 */
		/* ----------------------------------------------------------------- */
		void Overwrite(int kind) {
			this->Find(_T("Overwrite")).Value<ValueKind::Number>() = kind;
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  Filtering
		 *
		 *  圧縮する際に、特定のファイルやフォルダをフィルタリングするか
		 *  どうかを取得します。
		 */
		/* ----------------------------------------------------------------- */
		const bool& Filtering() const {
			return this->Find(_T("Filtering")).Value<ValueKind::Bool>();
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  Filtering
		 *
		 *  圧縮する際に、特定のファイルやフォルダをフィルタリングするか
		 *  どうかを設定します。
		 */
		/* ----------------------------------------------------------------- */
		void Filtering(bool enable) {
			this->Find(_T("Filtering")).Value<ValueKind::Bool>() = enable;
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  Open
		 *
		 *  圧縮処理終了後、圧縮ファイルを保存したフォルダを開くかどうかを
		 *  取得します。詳細は、CubeICE::OpenKind を参照下さい。
		 */
		/* ----------------------------------------------------------------- */
		const int& Open() const {
			return this->Find(_T("Open")).Value<ValueKind::Number>();
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  Open
		 *
		 *  圧縮処理終了後、圧縮ファイルを保存したフォルダを開くかどうかを
		 *  設定します。詳細は、CubeICE::OpenKind を参照下さい。
		 */
		/* ----------------------------------------------------------------- */
		void Open(int kind) {
			this->Find(_T("Open")).Value<ValueKind::Number>() = kind;
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  DeleteOnMail
		 *
		 *  「圧縮してメール送信」を選択時、処理終了後に圧縮ファイルを削除
		 *  するかどうかを取得します。
		 */
		/* ----------------------------------------------------------------- */
		const bool& DeleteOnMail() const {
			return this->Find(_T("DeleteOnMail")).Value<ValueKind::Bool>();
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  DeleteOnMail
		 *
		 *  「圧縮してメール送信」を選択時、処理終了後に圧縮ファイルを削除
		 *  するかどうかを設定します。
		 */
		/* ----------------------------------------------------------------- */
		void DeleteOnMail(bool enable) {
			this->Find(_T("DeleteOnMail")).Value<ValueKind::Bool>() = enable;
		}
	};
} // namespace CubeICE

#endif // CUBEICE_USER_SETTING_USER_COMPRESSION_SETTING_H
