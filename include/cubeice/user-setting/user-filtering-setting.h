// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/user-setting/user-filtering-setting.h
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
#ifndef CUBEICE_USER_SETTING_USER_FILTERING_SETTING_H
#define CUBEICE_USER_SETTING_USER_FILTERING_SETTING_H

#include <cubeice/config.h>
#include <cubeice/user-setting/user-setting-base.h>
#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// UserFilteringSetting
	///
	/// <summary>
	/// CubeICE で圧縮・解凍を行う際にフィルタリングするファイル名の一覧を
	/// 取得・保存するクラスです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class UserFilteringSetting : public UserSettingBase {
	public:
		typedef std::vector<string_type> container_type;
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		UserFilteringSetting() :
			UserSettingBase(), v_() {
			this->Reset();
		}
		
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		virtual ~UserFilteringSetting() {}
		
		/* ----------------------------------------------------------------- */
		///
		/// Reset
		///
		/// <summary>
		/// CubeICE で圧縮・解凍する際にフィルタリングするファイル名一覧の
		/// 初期値を設定します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void Reset() {
			v_.clear();
			v_.push_back(_T(".DS_Store"));
			v_.push_back(_T("Thumbs.db"));
			v_.push_back(_T("__MACOSX"));
			v_.push_back(_T("desktop.ini"));
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
			v_.clear();
			BOOST_FOREACH(const string_type& str, old.filters()) {
				if (!str.empty()) v_.push_back(str);
			}
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Empty
		///
		/// <summary>
		/// フィルタリングするファイルやフォルダを表した文字列が 1 つも
		/// 存在していないかどうかを取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		bool Empty() const { return v_.empty(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// Count
		///
		/// <summary>
		/// フィルタリングするファイルやフォルダを表した文字列の数を取得
		/// します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		container_type::size_type Count() const { return v_.size(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// Contains
		///
		/// <summary>
		/// 指定した文字列がフィルタリングするファイルやフォルダを表した
		/// 文字列群の中に含まれているかどうかを確認します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		bool Contains(const string_type& str) const {
			return std::find(v_.begin(), v_.end(), str) != v_.end();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Add
		///
		/// <summary>
		/// 指定した文字列をフィルタリングするファイルやフォルダを表した
		/// 文字列群に追加します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Add(const string_type& str) { if (!this->Contains(str)) v_.push_back(str); }
		
		/* ----------------------------------------------------------------- */
		///
		/// Delete
		///
		/// <summary>
		/// 指定した文字列をフィルタリングするファイルやフォルダを表した
		/// 文字列群から削除します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Delete(const string_type& str) {
			container_type::iterator pos = std::find(v_.begin(), v_.end(), str);
			if (pos != v_.end()) v_.erase(pos);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Clear
		///
		/// <summary>
		/// フィルタリングするファイルやフォルダを表した文字列群を全て
		/// 消去します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Clear() { v_.clear(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// Data
		///
		/// <summary>
		/// フィルタリングするファイルやフォルダを表した文字列群を取得
		/// します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		const container_type& Data() const { return v_; }
		
	protected:
		/* ----------------------------------------------------------------- */
		/// LoadFromParameterExec
		/* ----------------------------------------------------------------- */
		virtual void LoadFromParameterExec(const NodeSet& src) {
			optional<const Node&> node = PsdotNet::Parameter::FindOptional(src, _T("Filtering"));
			if (!node) return;
			
			v_.clear();
			container_type tmp;
			boost::algorithm::split(tmp, node->Value<ValueKind::String>(), boost::is_any_of(_T("|")));
			BOOST_FOREACH(const string_type& str, tmp) {
				if (!tmp.empty()) v_.push_back(str);
			}
		}
		
		/* ----------------------------------------------------------------- */
		/// SaveToParameterExec
		/* ----------------------------------------------------------------- */
		virtual void SaveToParameterExec(NodeSet& dest) {
			dest.push_back(Node(_T("Filtering"), boost::algorithm::join(v_, _T("|"))));
		}
		
	private:
		container_type v_;
	}; // class UserFilteringSetting
} // namespace CubeICE

#endif // CUBEICE_USER_SETTING_USER_FILTERING_SETTING_H
