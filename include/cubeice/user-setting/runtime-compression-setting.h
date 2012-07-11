// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/user-setting/runtime-compression-setting.h
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
#ifndef CUBEICE_USER_SETTING_RUNTIME_COMPRESSION_SETTING_H
#define CUBEICE_USER_SETTING_RUNTIME_COMPRESSION_SETTING_H

#include <cubeice/config.h>
#include <cubeice/user-setting/user-setting-base.h>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	/// RuntimeCompressionSetting
	/* --------------------------------------------------------------------- */
	class RuntimeCompressionSetting : public UserSettingBase {
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		RuntimeCompressionSetting() :
			UserSettingBase(), path_(), password_() {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~RuntimeCompressionSetting() {}
		
		/* ----------------------------------------------------------------- */
		///
		/// Reset
		///
		/// <summary>
		/// CubeICE のランタイム時の各種設定を初期化します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void Reset() {
			UserSettingBase::Reset();
			this->Parameters().push_back(Node(_T("Type"), _T("zip")));
			this->Parameters().push_back(Node(_T("Level"), CompressionLevel::Normal));
			this->Parameters().push_back(Node(_T("Method"), _T("Deflate")));
			this->Parameters().push_back(Node(_T("EnablePassword"), false));
			this->Parameters().push_back(Node(_T("ShowPassword"), false));
			this->Parameters().push_back(Node(_T("EncodingMethod"), _T("ZipCrypto")));
			this->Parameters().push_back(Node(_T("Overwrite"), OverwriteKind::Confirm));
			this->Parameters().push_back(Node(_T("ThreadSize"), 1));
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Type
		///
		/// <summary>
		/// 圧縮形式を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const string_type& Type() const { return this->Find(_T("Type")).Value<ValueKind::String>(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// Type
		///
		/// <summary>
		/// 圧縮形式を設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Type(const string_type& str) { this->Find(_T("Type")).Value(str); }
		
		/* ----------------------------------------------------------------- */
		///
		/// Level
		///
		/// <summary>
		/// 圧縮レベルを取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const int& Level() const { return this->Find(_T("Level")).Value<ValueKind::Number>(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// Level
		///
		/// <summary>
		/// 圧縮レベルを設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Level(int n) { this->Find(_T("Level")).Value(n); }
		
		/* ----------------------------------------------------------------- */
		///
		/// Method
		///
		/// <summary>
		/// 圧縮方法を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const string_type& Method() const { return this->Find(_T("Method")).Value<ValueKind::String>(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// Method
		///
		/// <summary>
		/// 圧縮方法を設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Method(const string_type& str) { this->Find(_T("Method")).Value(str); }
		
		/* ----------------------------------------------------------------- */
		///
		/// EnablePassword
		///
		/// <summary>
		/// パスワードを設定するかどうかを取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const bool& EnablePassword() const { return this->Find(_T("EnablePassword")).Value<ValueKind::Bool>(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// EnablePassword
		///
		/// <summary>
		/// パスワードを設定するかどうかを設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void EnablePassword(bool enable) { this->Find(_T("EnablePassword")).Value(enable); }
		
		/* ----------------------------------------------------------------- */
		///
		/// ShowPassword
		///
		/// <summary>
		/// パスワードを入力する際に、入力したパスワードそのままを表示する
		/// かどうかを取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const bool& ShowPassword() const { return this->Find(_T("ShowPassword")).Value<ValueKind::Bool>(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// ShowPassword
		///
		/// <summary>
		/// パスワードを入力する際に、入力したパスワードそのままを表示する
		/// かどうかを設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void ShowPassword(bool enable) { this->Find(_T("ShowPassword")).Value(enable); }
		
		/* ----------------------------------------------------------------- */
		///
		/// EncodingMethod
		///
		/// <summary>
		/// パスワードを設定する際の暗号化方式を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const string_type& EncodingMethod() const { return this->Find(_T("EncodingMethod")).Value<ValueKind::String>(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// EncodingMethod
		///
		/// <summary>
		/// パスワードを設定する際の暗号化方式を設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void EncodingMethod(const string_type& str) { this->Find(_T("EncodingMethod")).Value(str); }
		
		/* ----------------------------------------------------------------- */
		///
		/// Overwrite
		///
		/// <summary>
		/// 同名の圧縮ファイルが存在している場合の扱いを取得します。
		/// 設定可能な値は次の通りです。
		/// 
		/// 0 : 上書き
		/// 1 : 既存の圧縮ファイルに追加
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const int& Overwrite() const { return this->Find(_T("Overwrite")).Value<ValueKind::Number>(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// Overwrite
		///
		/// <summary>
		/// 同名の圧縮ファイルが存在している場合の扱いを設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Overwrite(int n) { this->Find(_T("Overwrite")).Value(n); }
		
		/* ----------------------------------------------------------------- */
		///
		/// ThreadSize
		///
		/// <summary>
		/// CPU スレッド数を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const int& ThreadSize() const { return this->Find(_T("ThreadSize")).Value<ValueKind::Number>(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// ThreadSize
		///
		/// <summary>
		/// CPU スレッド数を設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void ThreadSize(int n) { this->Find(_T("ThreadSize")).Value(n); }
		
		/* ----------------------------------------------------------------- */
		///
		/// OutputPath
		///
		/// <summary>
		/// 出力先パスを取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const string_type& OutputPath() const { return path_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// OutputPath
		///
		/// <summary>
		/// 出力先パスを設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void OutputPath(const string_type& path) { path_ = path; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Password
		///
		/// <summary>
		/// パスワードを取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const string_type& Password() const { return password_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Password
		///
		/// <summary>
		/// パスワードを設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Password(const string_type& str) { password_ = str; }
		
	private:
		string_type path_;
		string_type password_;
	};
} // namespace CubeICE

#endif // CUBEICE_USER_SETTING_RUNTIME_COMPRESSION_SETTING_H
