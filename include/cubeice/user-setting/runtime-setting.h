// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/user-setting/runtime-setting.h
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
#ifndef CUBEICE_USER_SETTING_RUNTIME_SETTING_H
#define CUBEICE_USER_SETTING_RUNTIME_SETTING_H

#include <cubeice/config.h>
#include <cubeice/user-setting/user-setting-base.h>
#include <cubeice/user-setting/runtime-compression-setting.h>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	/// RuntimeSetting
	/* --------------------------------------------------------------------- */
	class RuntimeSetting : public UserSettingBase {
	protected:
		/* ----------------------------------------------------------------- */
		//  LoadFromParameterExec
		/* ----------------------------------------------------------------- */
		virtual void LoadFromParameterExec(const NodeSet& src) {
			UserSettingBase::LoadFromParameterExec(src);
			optional<const Node&> node_compression = PsdotNet::Parameter::FindOptional(src, _T("Compression"));
			if (node_compression) compression_.LoadFromParameter(node_compression->Value<ValueKind::NodeSet>());
		}
		
		/* ----------------------------------------------------------------- */
		//  SaveToParameterExec
		/* ----------------------------------------------------------------- */
		virtual void SaveToParameterExec(NodeSet& dest) {
			UserSettingBase::SaveToParameterExec(dest);
			NodeSet ns_compression;
			compression_.SaveToParameter(ns_compression);
			dest.push_back(Node(_T("Compression"), ns_compression));
		}
		
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		RuntimeSetting() :
			UserSettingBase() {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~RuntimeSetting() {}
		
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
			compression_.Reset();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Compression
		///
		/// <summary>
		/// 圧縮関連のランタイム設定を取得します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		RuntimeCompressionSetting& Compression() { return compression_; }
		const RuntimeCompressionSetting& Compression() const { return compression_; }
		
	private:
		RuntimeCompressionSetting compression_;
	};
} // namespace CubeICE

#endif // CUBEICE_USER_SETTING_RUNTIME_SETTING_H
