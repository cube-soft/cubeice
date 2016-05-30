// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/user-setting/user-shortcut-setting.h
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
#ifndef CUBEICE_USER_SETTING_USER_SHORTCUT_SETTING_H
#define CUBEICE_USER_SETTING_USER_SHORTCUT_SETTING_H

#include <cubeice/config.h>
#include <cubeice/shortcut.h>
#include <cubeice/user-setting/user-setting-base.h>
#include <psdotnet/environment.h>
#include <psdotnet/wsh.h>
#include <psdotnet/io.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// UserShortcutSetting
	///
	/// CubeICE のデスクトップに作成するショートカットに関連する各種ユーザ
	/// 設定を取得・保存するクラスです。
	///
	/* --------------------------------------------------------------------- */
	class UserShortcutSetting : public UserSettingBase {
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		UserShortcutSetting() :
			UserSettingBase(), v_(), deleted_() {}
		
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		UserShortcutSetting(const UserShortcutSetting& cp) :
			UserSettingBase(cp), v_(cp.v_), deleted_(cp.deleted_) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~UserShortcutSetting() {}
		
		/* ----------------------------------------------------------------- */
		///
		/// Reset
		///
		/// <summary>
		/// CubeICE のショートカットに関連する設定可能な各種項目を
		/// 定義し、初期化します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void Reset() { v_.clear(); }
		
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
			v_.clear();
			
			string_type desktop = PsdotNet::Environment::GetFolderPath(PsdotNet::Environment::SpecialFolder::DesktopDirectory);
			string_type install = old.install_path();
			
			if ((old.shortcut_flags() & COMPRESS_FLAG) != 0) {
				Shortcut compression;
				compression.Name(_T("CubeICE 圧縮"));
				compression.Directory(desktop);
				compression.TargetPath(install + _T("\\") + CUBEICE_NAME);
				compression.Arguments(_T("/c"));
				compression.IconLocation(install + _T("\\") + CUBEICE_NAME + _T(",1"));
				if (this->Exists(compression)) v_.push_back(compression);
			}
			
			if ((old.shortcut_flags() & DECOMPRESS_FLAG) != 0) {
				Shortcut decompression;
				decompression.Name(_T("CubeICE 解凍"));
				decompression.Directory(desktop);
				decompression.TargetPath(install + _T("\\") + CUBEICE_NAME);
				decompression.Arguments(_T("/x"));
				decompression.IconLocation(install + _T("\\") + CUBEICE_NAME + _T(",2"));
				if (this->Exists(decompression)) v_.push_back(decompression);
			}
			
			if ((old.shortcut_flags() & SETTING_FLAG) != 0) {
				Shortcut setting;
				setting.Name(_T("CubeICE 設定"));
				setting.Directory(desktop);
				setting.TargetPath(install + _T("\\") + CUBEICE_SETTING_NAME);
				setting.IconLocation(install + _T("\\") + CUBEICE_SETTING_NAME + _T(",0"));
				if (this->Exists(setting)) v_.push_back(setting);
			}
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Items
		///
		/// <summary>
		/// 作成する（もしくは作例される予定の）ショートカットの一覧を
		/// 取得します。
		///
		/// NOTE: Items() メンバ関数は、既に登録されているショートカット
		/// の内容を参照する方法のみを提供します。新たなショートカットを
		/// 追加する、または既に存在するショートカットを削除する場合は
		/// Add/Delete メンバ関数を使用して下さい。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const std::vector<Shortcut>& Items() const { return v_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// GetOptional
		///
		/// <summary>
		/// 指定された名前に対応するショートカット情報を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		optional<Shortcut&> GetOptional(const string_type& name) {
			BOOST_FOREACH(Shortcut& item, v_) {
				if (item.Name() == name) return optional<Shortcut&>(item);
			}
			return optional<Shortcut&>();
		}
		
		optional<const Shortcut&> GetOptional(const string_type& name) const {
			BOOST_FOREACH(const Shortcut& item, v_) {
				if (item.Name() == name) return optional<const Shortcut&>(item);
			}
			return optional<const Shortcut&>();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Get
		///
		/// <summary>
		/// 指定された名前に対応するショートカット情報を取得します。
		/// 項目が存在しない場合は、例外が送出されます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		Shortcut& Get(const string_type& name) {
			optional<Shortcut&> dest = this->GetOptional(name);
			if (!dest) throw PsdotNet::Exception(_T("item not found"));
			return *dest;
		}
		
		const Shortcut& Get(const string_type& name) const {
			optional<const Shortcut&> dest = this->GetOptional(name);
			if (!dest) throw PsdotNet::Exception(_T("item not found"));
			return *dest;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Add
		///
		/// <summary>
		/// 新たなショートカットを追加します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Add(const Shortcut& item) { v_.push_back(item); }
		
		/* ----------------------------------------------------------------- */
		///
		/// Delete
		///
		/// <summary>
		/// 既に登録されているショートカットを削除します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Delete(const string_type& name) {
			for (std::vector<Shortcut>::iterator pos = v_.begin(); pos != v_.end(); ++pos) {
				if (pos->Name() == name) {
					deleted_.push_back(*pos);
					v_.erase(pos);
					break;
				}
			}
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Clear
		///
		/// <summary>
		/// 既に登録されているショートカットを全て削除します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Clear() {
			deleted_.insert(deleted_.end(), v_.begin(), v_.end());
			v_.clear();
		}
		
	protected:
		/* ----------------------------------------------------------------- */
		/// LoadFromParameterExec
		/* ----------------------------------------------------------------- */
		virtual void LoadFromParameterExec(const NodeSet& src) {
			v_.clear();
			BOOST_FOREACH(const Node& node, src) {
				if (node.Kind() != ValueKind::NodeSet) continue;
				Shortcut item;
				bool status = this->LoadShortcut(node, item);
				if (status) v_.push_back(item);
			}
		}
		
		/* ----------------------------------------------------------------- */
		/// SaveToParameterExec
		/* ----------------------------------------------------------------- */
		virtual void SaveToParameterExec(NodeSet& dest) {
			BOOST_FOREACH(const Shortcut& item, v_) {
				this->SaveShortcut(item, dest);
				this->CreateShortcut(item);
			}
			
			BOOST_FOREACH(const Shortcut& item, deleted_) {
				this->DeleteShortcut(item);
			}
			deleted_.clear();
		}
		
	private:
		/* ----------------------------------------------------------------- */
		/// LoadShortcut
		/* ----------------------------------------------------------------- */
		bool LoadShortcut(const Node& src, Shortcut& dest) {
			if (src.Kind() != ValueKind::NodeSet) return false;
			const NodeSet& v = src.Value<ValueKind::NodeSet>();
			
			optional<const Node&> opt = PsdotNet::Parameter::FindOptional(v, _T("Name"));
			if (!opt || opt->Kind() != ValueKind::String) return false;
			else dest.Name(opt->Value<ValueKind::String>());
			
			opt = PsdotNet::Parameter::FindOptional(v, _T("TargetPath"));
			if (!opt || opt->Kind() != ValueKind::String) return false;
			else dest.TargetPath(opt->Value<ValueKind::String>());
			
			opt = PsdotNet::Parameter::FindOptional(v, _T("Directory"));
			if (opt) dest.Directory(opt->Value<ValueKind::String>());
			else dest.Directory(PsdotNet::Environment::GetFolderPath(PsdotNet::Environment::SpecialFolder::DesktopDirectory));
			
			opt = PsdotNet::Parameter::FindOptional(v, _T("Arguments"));
			if (opt) dest.Arguments(opt->Value<ValueKind::String>());
			
			opt = PsdotNet::Parameter::FindOptional(v, _T("IconLocation"));
			if (opt) dest.IconLocation(opt->Value<ValueKind::String>());
			
			return this->Exists(dest);
		}
		
		/* ----------------------------------------------------------------- */
		/// SaveShortcut
		/* ----------------------------------------------------------------- */
		bool SaveShortcut(const Shortcut& src, NodeSet& dest) {
			NodeSet child;
			if (src.Name().empty()) return false;
			else child.push_back(Node(_T("Name"), src.Name()));
			
			if (src.TargetPath().empty()) return false;
			else child.push_back(Node(_T("TargetPath"), src.TargetPath()));
			
			if (!src.Directory().empty()) child.push_back(Node(_T("Directory"), src.Directory()));
			if (!src.Arguments().empty()) child.push_back(Node(_T("Arguments"), src.Arguments()));
			if (!src.IconLocation().empty()) child.push_back(Node(_T("IconLocation"), src.IconLocation()));
			
			dest.push_back(Node(this->KeyName(), child));
			return true;
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// CreateShortcut
		///
		/// <summary>
		/// 指定されたショートカット情報を基にして、実際にショートカットを
		/// 作成します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		bool CreateShortcut(const Shortcut& src) {
			string_type desktop = PsdotNet::Environment::GetFolderPath(PsdotNet::Environment::SpecialFolder::DesktopDirectory);
			string_type dir = !src.Directory().empty() ? src.Directory() : desktop;
			string_type path = dir + _T("\\") + src.Name() + _T(".lnk");
			
			PsdotNet::WshShortcut sc(path);
			sc.TargetPath(src.TargetPath());
			sc.IconLocation(src.IconLocation());
			if (!src.Arguments().empty()) {
				boost::algorithm::split(sc.Arguments(), src.Arguments(), boost::is_any_of(_T(" \t")));
			}
			sc.Save();
			
			return PsdotNet::IO::File::Exists(path);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// DeleteShortcut
		///
		/// <summary>
		/// 指定されたショートカット情報に該当するショートカットを削除します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		bool DeleteShortcut(const Shortcut& src) {
			string_type desktop = PsdotNet::Environment::GetFolderPath(PsdotNet::Environment::SpecialFolder::DesktopDirectory);
			string_type dir = !src.Directory().empty() ? src.Directory() : desktop;
			string_type path = dir + _T("\\") + src.Name() + _T(".lnk");
			
			PsdotNet::IO::File::Delete(path);
			return !PsdotNet::IO::File::Exists(path);
		}
		
		/* ----------------------------------------------------------------- */
		/// KeyName
		/* ----------------------------------------------------------------- */
		string_type KeyName() {
			static int index = 0;
			std::basic_stringstream<char_type> ss;
			ss << _T("sc") << index;
			++index;
			return ss.str();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Exists
		///
		/// <summary>
		/// 指定されたショートカットが存在するかどうかを判定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		bool Exists(const Shortcut& src) {
			string_type desktop = PsdotNet::Environment::GetFolderPath(PsdotNet::Environment::SpecialFolder::DesktopDirectory);
			string_type dir = !src.Directory().empty() ? src.Directory() : desktop;
			string_type path = dir + _T("\\") + src.Name() + _T(".lnk");
			return PsdotNet::IO::File::Exists(path);
		}
		
	private:
		std::vector<Shortcut> v_;
		std::vector<Shortcut> deleted_;
	}; // class UserShortcutSetting
} // namespace CubeICE

#endif // CUBEICE_USER_SETTING_USER_SHORTCUT_SETTING_H
