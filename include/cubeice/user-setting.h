// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/user-setting.h
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
#ifndef CUBEICE_USER_SETTING_H
#define CUBEICE_USER_SETTING_H

#include <cubeice/config.h>
#include <sstream>
#include <exception>
#include <psdotnet/logger.h>
#include <psdotnet/registry.h>

#include <cubeice/user-setting/utility.h>
#include <cubeice/user-setting/user-setting-base.h>
#include <cubeice/user-setting/user-association-setting.h>
#include <cubeice/user-setting/user-context-setting.h>
#include <cubeice/user-setting/user-shortcut-setting.h>
#include <cubeice/user-setting/user-compression-setting.h>
#include <cubeice/user-setting/user-decompression-setting.h>
#include <cubeice/user-setting/user-filtering-setting.h>
#include <cubeice/user-setting/runtime-setting.h>

/* ------------------------------------------------------------------------- */
//  レジストリに関する定義
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_REG_ROOT
#define CUBEICE_REG_ROOT                        _T("Software\\CubeSoft\\CubeICE")
#endif

#ifndef CUBEICE_REG_STRUCTVERSION
#define CUBEICE_REG_STRUCTVERSION               _T("v2")
#endif

/* ------------------------------------------------------------------------- */
//  基本的な情報
/* ------------------------------------------------------------------------- */
#define CUBEICE_NAME_INSTALL_DIRECTORY          _T("InstallDirectory")
#define CUBEICE_NAME_VERSION                    _T("Version")

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// UserSetting
	///
	/// <summary>
	/// CubeICE に関連する各種ユーザ設定を取得・保存するクラスです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class UserSetting : public UserSettingBase{
	public:
		typedef CubeICE::UserAssociationSetting association_type;
		typedef CubeICE::UserContextSetting context_type;
		typedef CubeICE::UserDragDropSetting dragdrop_type;
		typedef CubeICE::UserShortcutSetting shortcut_type;
		typedef CubeICE::UserCompressionSetting compression_type;
		typedef CubeICE::UserDecompressionSetting decompression_type;
		typedef CubeICE::UserFilteringSetting filtering_type;
		typedef CubeICE::RuntimeSetting runtime_type;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		UserSetting() :
			UserSettingBase(),
			path_(), version_(),
			association_(), context_(), dragdrop_(), shortcut_(),
			compression_(), decompression_(),
			filtering_(), runtime_() {
			this->Reset();
			this->LoadBasicData();
		}
		
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		UserSetting(const UserSetting& cp) :
			UserSettingBase(cp),
			path_(cp.path_), version_(cp.version_),
			association_(cp.association_), context_(cp.context_), dragdrop_(cp.dragdrop_), shortcut_(cp.shortcut_),
			compression_(cp.compression_), decompression_(cp.decompression_),
			filtering_(cp.filtering_), runtime_() {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~UserSetting() {}
		
		/* ----------------------------------------------------------------- */
		///
		/// Reset
		///
		/// <summary>
		/// CubeICE の各種設定項目をここで定義します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void Reset() {
			UserSettingBase::Reset();
			this->Parameters().push_back(Node(_T("Tooltip"), true));
			this->Parameters().push_back(Node(_T("TooltipCount"), 5));
			this->Parameters().push_back(Node(_T("Explorer"), _T("")));
			this->Parameters().push_back(Node(_T("ErrorReport"), true));
			this->Parameters().push_back(Node(_T("LogLevel"), PsdotNet::LogLevel::Error));
			this->Parameters().push_back(Node(_T("CheckUpdate"), true));
			this->Parameters().push_back(Node(_T("WarnOnDetail"), true));
			
			compression_.Reset();
			decompression_.Reset();
			association_.Reset();
			context_.Reset();
			dragdrop_.Reset();
			shortcut_.Reset();
			filtering_.Reset();
			runtime_.Reset();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Upgrade
		///
		/// <summary>
		/// 旧バージョンのデータ構造から現在のものに変換します。
		///
		/// NOTE: いくつかの項目については、旧バージョンには同じものを
		/// 表すものが compression/decompression それぞれに存在します。
		/// 現状では、decomression で設定されている値を採用しています。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void Upgrade(const Version1& old) {
			this->Tooltip((old.decompression().details() & DETAIL_TOOLTIP) != 0);
			this->TooltipCount(old.decompression().max_filelist());
			this->Explorer(old.decompression().explorer());
			this->ErrorReport((old.decompression().details() & DETAIL_REPORT) != 0);
			this->LogLevel(old.loglevel());
			this->CheckUpdate(old.update());
			
			compression_.Upgrade(old);
			decompression_.Upgrade(old);
			association_.Upgrade(old);
			context_.Upgrade(old);
			dragdrop_.Upgrade(old);
			shortcut_.Upgrade(old);
			filtering_.Upgrade(old);
			runtime_.Upgrade(old);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Load
		///
		/// <summary>
		/// レジストリから各種ユーザ設定を読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Load() {
			std::basic_stringstream<char_type> ss;
			ss << CUBEICE_REG_ROOT << _T("\\") << CUBEICE_REG_STRUCTVERSION;
			PsdotNet::RegistryKey root = PsdotNet::Registry::CurrentUser().CreateSubKey(ss.str());
			if (!root) return;
			
			Document doc;
			doc.Read(root);
			this->LoadFromDocument(doc);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Load
		///
		/// <summary>
		/// XML ファイルから各種ユーザ設定を読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Load(const string_type& path) {
			Document doc;
			doc.Read(path);
			this->LoadFromDocument(doc);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Save
		///
		/// <summary>
		/// レジストリへ各種ユーザ設定を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Save() {
			std::basic_stringstream<char_type> ss;
			ss << CUBEICE_REG_ROOT << _T("\\") << CUBEICE_REG_STRUCTVERSION;
			PsdotNet::RegistryKey root = PsdotNet::Registry::CurrentUser().CreateSubKey(ss.str());
			if (!root) return;
			
			Document doc;
			this->SaveToDocument(doc);
			doc.Write(root);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Save
		///
		/// <summary>
		/// XML ファイルへ各種ユーザ設定を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Save(const string_type& path) {
			Document doc;
			this->SaveToDocument(doc);
			doc.Write(path);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// InstallDirectory
		///
		/// <summary>
		/// CubeICE がインストールされたディレクトリのパスを取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const string_type& InstallDirectory() const { return path_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Version
		///
		/// <summary>
		/// インストールされている CubeICE のバージョン情報を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const string_type& Version() const { return version_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Association
		///
		/// <summary>
		/// ファイルの関連付けに関する設定を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		association_type& Association() { return association_; }
		const association_type& Association() const { return association_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Context
		///
		/// <summary>
		/// コンテキストメニューに関する設定を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		context_type& Context() { return context_; }
		const context_type& Context() const { return context_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// DragDrop
		///
		/// <summary>
		/// ドラッグ&ドロップに関する設定を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		dragdrop_type& DragDrop() { return dragdrop_; }
		const dragdrop_type& DragDrop() const { return dragdrop_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Shortcut
		///
		/// <summary>
		/// デスクトップに作成するショートカットに関する設定を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		shortcut_type& Shortcut() { return shortcut_; }
		const shortcut_type& Shortcut() const { return shortcut_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Compression
		///
		/// <summary>
		/// 圧縮に関する設定を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		compression_type& Compression() { return compression_; }
		const compression_type& Compression() const { return compression_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Decompression
		///
		/// <summary>
		/// 解凍に関する設定を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		decompression_type& Decompression() { return decompression_; }
		const decompression_type& Decompression() const { return decompression_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Filtering
		///
		/// <summary>
		/// フィルタリングに関する設定を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		filtering_type& Filtering() { return filtering_; }
		const filtering_type& Filtering() const { return filtering_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Runtime
		///
		/// ランタイム時に関する設定を取得します。
		///
		/* ----------------------------------------------------------------- */
		runtime_type& Runtime() { return runtime_; }
		const runtime_type& Runtime() const { return runtime_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// Tooltip
		///
		/// <summary>
		/// エクスプローラで圧縮ファイルにマウスカーソルを合わせた時に
		/// 表示されるツールチップで、ファイル一覧を表示するかどうかを
		/// 取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const bool& Tooltip() const {
			return this->Find(_T("Tooltip")).Value<ValueKind::Bool>();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Tooltip
		///
		/// <summary>
		/// エクスプローラで圧縮ファイルにマウスカーソルを合わせた時に
		/// 表示されるツールチップで、ファイル一覧を表示するかどうかを
		/// 設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Tooltip(bool enable) {
			this->Find(_T("Tooltip")).Value<ValueKind::Bool>() = enable;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// TooltipCount
		///
		/// <summary>
		/// エクスプローラで圧縮ファイルにマウスカーソルを合わせた時に
		/// 表示されるツールチップで、ファイル一覧を表示する際の最大表示
		/// 件数を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const int& TooltipCount() const {
			return this->Find(_T("TooltipCount")).Value<ValueKind::Number>();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// TooltipCount
		///
		/// <summary>
		/// エクスプローラで圧縮ファイルにマウスカーソルを合わせた時に
		/// 表示されるツールチップで、ファイル一覧を表示する際の最大表示
		/// 件数を設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void TooltipCount(int n) {
			this->Find(_T("TooltipCount")).Value<ValueKind::Number>() = n;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Explorer
		///
		/// <summary>
		/// 圧縮・解凍終了後に出力先フォルダを開く際に使用するプログラムの
		/// パスを取得します。取得される値が空文字の場合、エクスプローラが
		/// 使用されます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const string_type& Explorer() const {
			return this->Find(_T("Explorer")).Value<ValueKind::String>();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Explorer
		///
		/// <summary>
		/// 圧縮・解凍終了後に出力先フォルダを開く際に使用するプログラムの
		/// パスを設定します。取得される値が空文字の場合、エクスプローラが
		/// 使用されます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Explorer(const string_type& path) {
			this->Find(_T("Explorer")).Value<ValueKind::String>() = path;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// ErrorReport
		///
		/// <summary>
		/// CubeICE 実行中に何らかのエラーが発生した時、エラーレポート用
		/// ダイアログを表示するかどうかを取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const bool& ErrorReport() const { return this->Find(_T("ErrorReport")).Value<ValueKind::Bool>(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// ErrorReport
		///
		/// <summary>
		/// CubeICE 実行中に何らかのエラーが発生した時、エラーレポート用
		/// ダイアログを表示するかどうかを設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void ErrorReport(bool enable) { this->Find(_T("ErrorReport")).Value(enable); }
		
		/* ----------------------------------------------------------------- */
		///
		/// LogLevel
		///
		/// <summary>
		/// ログの出力レベルを取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const int& LogLevel() const {
			return this->Find(_T("LogLevel")).Value<ValueKind::Number>();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// LogLevel
		///
		/// <summary>
		/// ログの出力レベルを設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void LogLevel(int level) { this->Find(_T("LogLevel")).Value(level); }
		
		/* ----------------------------------------------------------------- */
		///
		/// LogLevel
		///
		/// <summary>
		/// ログの出力レベルを設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void LogLevel(const PsdotNet::LogLevel& level) { this->LogLevel(level.ToEnum()); }
		
		/* ----------------------------------------------------------------- */
		///
		/// CheckUpdate
		///
		/// <summary>
		/// アップデートチェックプログラムをスタートアップに登録するか
		/// どうかを表す値を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const bool& CheckUpdate() const { return this->Find(_T("CheckUpdate")).Value<ValueKind::Bool>(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// CheckUpdate
		///
		/// <summary>
		/// アップデートチェックプログラムをスタートアップに登録するか
		/// どうかを表す値を設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void CheckUpdate(bool enable) { this->Find(_T("CheckUpdate")).Value(enable); }
		
		/* ----------------------------------------------------------------- */
		///
		/// WarnOnDetail
		///
		/// <summary>
		/// CubeICE 詳細設定を起動した際に警告を表示するかどうかを表す値を
		/// 取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const bool& WarnOnDetail() const { return this->Find(_T("WarnOnDetail")).Value<ValueKind::Bool>(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// WarnOnDetail
		///
		/// <summary>
		/// CubeICE 詳細設定を起動した際に警告を表示するかどうかを表す値を
		/// 設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void WarnOnDetail(bool enable) { this->Find(_T("WarnOnDetail")).Value(enable); }
		
	private:
		/* ----------------------------------------------------------------- */
		///
		/// LoadBasicData
		///
		/// <summary>
		/// インストールディレクトリ、バージョン等、HKEY_LOCAL_MACHINE 以下
		/// に保存されている CubeICE の基本的なデータを読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void LoadBasicData() {
			try {
				using PsdotNet::Registry;
				using PsdotNet::RegistryKey;
				using PsdotNet::RegistryValueKind;
				
				RegistryKey subkey = Registry::LocalMachine().OpenSubKey(CUBEICE_REG_ROOT, false);
				if (!subkey) return;
				
				path_ = subkey.GetValue<string_type>(CUBEICE_NAME_INSTALL_DIRECTORY);
				version_ = subkey.GetValue<string_type>(CUBEICE_NAME_VERSION);
			}
			catch (const std::exception& /* err */) {}
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// LoadFromDocument
		///
		/// <summary>
		/// CubeICE の各種設定を読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void LoadFromDocument(const Document& doc) {
			this->LoadFromParameter(doc.Root());
			filtering_.LoadFromParameter(doc.Root());
			
			// 圧縮関連の設定
			optional<const Node&> node_compression = PsdotNet::Parameter::FindOptional(doc.Root(), _T("Compression"));
			if (node_compression) compression_.LoadFromParameter(node_compression->Value<ValueKind::NodeSet>());
			
			// 解凍関連の設定
			optional<const Node&> node_decompression = PsdotNet::Parameter::FindOptional(doc.Root(), _T("Decompression"));
			if (node_decompression) decompression_.LoadFromParameter(node_decompression->Value<ValueKind::NodeSet>());
			
			// コンテキストメニュー関連の設定
			optional<const Node&> node_context = PsdotNet::Parameter::FindOptional(doc.Root(), _T("Context"));
			if (node_context) context_.LoadFromParameter(node_context->Value<ValueKind::NodeSet>());
			
			// ドラッグ&ドロップ関連の設定
			optional<const Node&> node_dragdrop = PsdotNet::Parameter::FindOptional(doc.Root(), _T("DragDrop"));
			if (node_dragdrop) dragdrop_.LoadFromParameter(node_dragdrop->Value<ValueKind::NodeSet>());
			
			// 関連付け関連の設定
			optional<const Node&> node_association = PsdotNet::Parameter::FindOptional(doc.Root(), _T("Association"));
			if (node_association) association_.LoadFromParameter(node_association->Value<ValueKind::NodeSet>());
			
			// ショートカット関連の設定
			optional<const Node&> node_shortcut = PsdotNet::Parameter::FindOptional(doc.Root(), _T("Shortcut"));
			if (node_shortcut) shortcut_.LoadFromParameter(node_shortcut->Value<ValueKind::NodeSet>());
			
			// ランタイム時の設定
			optional<const Node&> node_runtime = PsdotNet::Parameter::FindOptional(doc.Root(), _T("Runtime"));
			if (node_runtime) runtime_.LoadFromParameter(node_runtime->Value<ValueKind::NodeSet>());
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveToDocument
		///
		/// <summary>
		/// CubeICE の各種設定を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void SaveToDocument(Document& doc) {
			this->SaveToParameter(doc.Root());
			filtering_.SaveToParameter(doc.Root());
			
			// 圧縮関連の設定
			NodeSet ns_compression;
			compression_.SaveToParameter(ns_compression);
			doc.Root().push_back(Node(_T("Compression"), ns_compression));
			
			// 解凍関連の設定
			NodeSet ns_decompression;
			decompression_.SaveToParameter(ns_decompression);
			doc.Root().push_back(Node(_T("Decompression"), ns_decompression));
			
			// コンテキストメニュー関連の設定
			NodeSet ns_context;
			context_.SaveToParameter(ns_context);
			doc.Root().push_back(Node(_T("Context"), ns_context));
			
			// ドラッグ&ドロップ関連の設定
			NodeSet ns_dragdrop;
			dragdrop_.SaveToParameter(ns_dragdrop);
			doc.Root().push_back(Node(_T("DragDrop"), ns_dragdrop));
			
			// 関連付け関連の設定
			NodeSet ns_association;
			association_.SaveToParameter(ns_association);
			doc.Root().push_back(Node(_T("Association"), ns_association));
			
			// ショートカット関連の設定
			NodeSet ns_shortcut;
			shortcut_.SaveToParameter(ns_shortcut);
			doc.Root().push_back(Node(_T("Shortcut"), ns_shortcut));
			
			// ランタイム時の設定
			NodeSet ns_runtime;
			runtime_.SaveToParameter(ns_runtime);
			doc.Root().push_back(Node(_T("Runtime"), ns_runtime));
		}
		
	private:
		string_type             path_;              // インストールディレクトリ
		string_type             version_;           // バージョン
		association_type        association_;       // ファイルの関連付け
		context_type            context_;           // コンテキストメニュー
		dragdrop_type           dragdrop_;          // ドラッグ&ドロップメニュー
		shortcut_type           shortcut_;          // デスクトップに作成するショートカット
		compression_type        compression_;       // 圧縮
		decompression_type      decompression_;     // 解凍
		filtering_type          filtering_;         // フィルタリング
		runtime_type            runtime_;           // ランタイム時の設定
	}; // class UserSetting
} // namespace CubeICE

#endif // CUBEICE_USER_SETTING_H
