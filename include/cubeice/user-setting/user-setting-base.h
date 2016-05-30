// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/user-setting/user-setting-base.h
 *
 *  Copyright (c) 2010 CubeSoft, Inc.
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
#ifndef CUBEICE_USER_SETTING_USER_SETTING_BASE_H
#define CUBEICE_USER_SETTING_USER_SETTING_BASE_H

#include <cubeice/config.h>
#include <cubeice/user-setting/v1.h>
#include <psdotnet/parameter.h>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// UserSettingBase
	///
	/// <summary>
	/// 各種ユーザ設定を操作するクラスの基底クラスです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class UserSettingBase {
	public:
		typedef CubeICE::char_type char_type;
		typedef CubeICE::string_type string_type;
		typedef CubeICE::v1::user_setting Version1;
		typedef PsdotNet::Parameter::ValueKind ValueKind;
		typedef PsdotNet::Parameter::Node Node;
		typedef PsdotNet::Parameter::NodeSet NodeSet;
		typedef PsdotNet::Parameter::Document Document;
		
	public:
		/* ----------------------------------------------------------------- */
		///
		/// constructor
		///
		/// <summary>
		/// CubeICE の各種設定項目を規定の値で初期化します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		UserSettingBase() :
			parameters_() {
			this->Reset();
		}
		
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		UserSettingBase(const UserSettingBase& cp) :
			parameters_(cp.parameters_) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~UserSettingBase() {}
		
		/* ----------------------------------------------------------------- */
		/// Reset
		/* ----------------------------------------------------------------- */
		virtual void Reset() { parameters_.clear(); }
		
		/* ----------------------------------------------------------------- */
		///
		/// Upgrade
		///
		/// <summary>
		/// 旧バージョンのデータ構造から現在のものに変換します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void Upgrade(const Version1& old) {}
		
		/* ----------------------------------------------------------------- */
		///
		/// LoadFromParameter
		///
		/// <summary>
		/// 引数に指定されたパラメータ保存用クラスから各種設定情報を
		/// 読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void LoadFromParameter(const NodeSet& src) {
			this->LoadFromParameterExec(src);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveToParameter
		///
		/// <summary>
		/// 引数に指定されたパラメータ保存用クラスに現在の情報を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void SaveToParameter(NodeSet& dest) {
			this->SaveToParameterExec(dest);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Parameters
		///
		/// <summary>
		/// CubeICE の各種設定項目を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		NodeSet& Parameters() { return parameters_; }
		const NodeSet& Parameters() const { return parameters_; }
		
		/* ----------------------------------------------------------------- */
		///
		/// FindOptional
		///
		/// <summary>
		/// name で指定された CubeICE の設定項目を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		optional<Node&> FindOptional(const string_type& name) {
			return PsdotNet::Parameter::FindOptional(parameters_, name);
		}
		
		optional<const Node&> FindOptional(const string_type& name) const {
			return PsdotNet::Parameter::FindOptional(parameters_, name);
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Find
		///
		/// <summary>
		/// name で指定された CubeICE の設定項目を取得します。該当する項目が
		/// 存在しない場合は例外が送出されます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		Node& Find(const string_type& name) { return PsdotNet::Parameter::Find(parameters_, name); }
		const Node& Find(const string_type& name) const { return PsdotNet::Parameter::Find(parameters_, name); }
		
	protected:
		/* ----------------------------------------------------------------- */
		///
		/// LoadFromParameterExec
		///
		/// <summary>
		/// 汎用データ構造である PsdotNet::Parameter::NodeSet から必要な
		/// 部分を抽出して、現在のユーザ設定を読み込みます。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void LoadFromParameterExec(const NodeSet& src) {
			if (src.empty()) return;
			
			BOOST_FOREACH(Node& element, parameters_) {
				optional<const Node&> node = PsdotNet::Parameter::FindOptional(src, element.Name());
				if (node) element = *node;
			}
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// SaveToParameterExec
		///
		/// <summary>
		/// 汎用的なデータ構造である PsdotNet::Parameter::NodeSet へ現在の
		/// ユーザ設定を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void SaveToParameterExec(NodeSet& dest) {
			dest.insert(dest.end(), parameters_.begin(), parameters_.end());
		}
		
	private:
		NodeSet parameters_;
	}; // class UserSettingBase
} // namespace CubeICE

#endif // CUBEICE_USER_SETTING_USER_SETTING_BASE_H
