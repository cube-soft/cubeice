// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/user-setting/user-decompression-setting.h
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
#ifndef CUBEICE_USER_SETTING_USER_DECOMPRESSION_SETTING_H
#define CUBEICE_USER_SETTING_USER_DECOMPRESSION_SETTING_H

#include <cubeice/config.h>
#include <cubeice/user-setting/user-setting-base.h>
#include <cubeice/user-setting/user-setting-value.h>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// UserDecompressionSetting
	///
	/// <summary>
	/// CubeICE 圧縮に関連する各種ユーザ設定を取得・保存するクラスです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class UserDecompressionSetting : public UserSettingBase {
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		UserDecompressionSetting() :
			UserSettingBase() {
			this->Reset();
		}
		
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		UserDecompressionSetting(const UserDecompressionSetting& cp) :
			UserSettingBase(cp) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~UserDecompressionSetting() {}
		
		/* ----------------------------------------------------------------- */
		///
		/// Reset
		///
		/// <summary>
		/// CubeICE の解凍に関連する設定可能な項目をここで定義します。
		/// 設定項目ごとに、名前、および初期値を定義します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void Reset() {
			UserSettingBase::Reset();
			this->Parameters().push_back(Node(_T("OutputCondition"), OutputKind::Specific));
			this->Parameters().push_back(Node(_T("OutputPath"), _T("")));
			this->Parameters().push_back(Node(_T("SourceEncoding"), EncodingKind::Auto));
			this->Parameters().push_back(Node(_T("Overwrite"), OverwriteKind::Confirm));
			this->Parameters().push_back(Node(_T("CreateFolder"), CreateFolderKind::SkipSingleFolder));
			this->Parameters().push_back(Node(_T("Filtering"), true));
			this->Parameters().push_back(Node(_T("Open"), OpenKind::SkipDesktop));
			this->Parameters().push_back(Node(_T("DeleteOnExtract"), false));
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
			this->OutputCondition(old.decompression().output_condition());
			this->OutputPath(old.decompression().output_path());
			
			int details = old.decompression().details();
			if ((details & DETAIL_OVERWRITE) == 0) this->Overwrite(OverwriteKind::Force);
			else if ((details & DETAIL_IGNORE_NEWER) != 0) this->Overwrite(OverwriteKind::ConfirmOlder);
			else this->Overwrite(OverwriteKind::Confirm);
			
			if ((details & DETAIL_CREATE_FOLDER) == 0) this->CreateFolder(CreateFolderKind::Skip);
			else if ((details & DETAIL_SINGLE_FILE) != 0) this->CreateFolder(CreateFolderKind::SkipSingleFile);
			else if ((details & DETAIL_SINGLE_FOLDER) != 0) this->CreateFolder(CreateFolderKind::SkipSingleFolder);
			else this->CreateFolder(CreateFolderKind::Create);
			
			if ((details & DETAIL_OPEN) == 0) this->Open(OpenKind::Skip);
			else if ((details & DETAIL_SKIP_DESKTOP) != 0) this->Open(OpenKind::SkipDesktop);
			else this->Open(OpenKind::Open);
			
			this->Filtering((details & DETAIL_FILTER) != 0);
			this->DeleteOnExtract((details & DETAIL_REMOVE_SRC) != 0);
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  OutputCondition
		 *
		 *  圧縮ファイルの保存先の決定方法を取得します。
		 *  決定方法の詳細については、CubeICE::OutputKind を参照下さい。
		 */
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
		 *  SourceEncoding
		 *
		 *  文字エンコーディングを変換する際の、入力元の文字エンコーディング
		 *  の指定を取得します。詳細については、CubeICE::EncodingKind を
		 *  参照下さい。
		 */
		/* ----------------------------------------------------------------- */
		const int& SourceEncoding() const {
			return this->Find(_T("SourceEncoding")).Value<ValueKind::Number>();
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  SourceEncoding
		 *
		 *  文字エンコーディングを変換する際の、入力元の文字エンコーディング
		 *  の指定を設定します。詳細については、CubeICE::EncodingKind を
		 *  参照下さい。
		 */
		/* ----------------------------------------------------------------- */
		void SourceEncoding(int kind) {
			this->Find(_T("SourceEncoding")).Value<ValueKind::Number>() = kind;
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
		 *  CreateFolder
		 *
		 *  解凍する際にフォルダを作成するかどうかを取得します。
		 *  詳細については、CubeICE::CreateFolderKind を参照下さい。
		 */
		/* ----------------------------------------------------------------- */
		const int& CreateFolder() const {
			return this->Find(_T("CreateFolder")).Value<ValueKind::Number>();
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  CreateFolder
		 *
		 *  解凍する際にフォルダを作成するかどうかを設定します。
		 *  詳細については、CubeICE::CreateFolderKind を参照下さい。
		 */
		/* ----------------------------------------------------------------- */
		void CreateFolder(int kind) {
			this->Find(_T("CreateFolder")).Value<ValueKind::Number>() = kind;
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
		 *  DeleteOnExtract
		 *
		 *  解凍後、入力ファイルを削除するかどうかを取得します。
		 */
		/* ----------------------------------------------------------------- */
		const bool& DeleteOnExtract() const {
			return this->Find(_T("DeleteOnExtract")).Value<ValueKind::Bool>();
		}
		
		/* ----------------------------------------------------------------- */
		/*!
		 *  DeleteOnExtract
		 *
		 *  解凍後、入力ファイルを削除するかどうかを設定します。
		 */
		/* ----------------------------------------------------------------- */
		void DeleteOnExtract(bool enable) {
			this->Find(_T("DeleteOnExtract")).Value<ValueKind::Bool>() = enable;
		}
	}; // class UserDecompressionSetting
} // namespace CubeICE

#endif // CUBEICE_USER_SETTING_USER_DECOMPRESSION_SETTING_H
