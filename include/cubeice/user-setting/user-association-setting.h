// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/user-setting/user-association-setting.h
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
#ifndef CUBEICE_USER_SETTING_USER_ASSOCIATION_SETTING_H
#define CUBEICE_USER_SETTING_USER_ASSOCIATION_SETTING_H

#include <cubeice/config.h>
#include <cubeice/user-setting/user-setting-base.h>
#include <psdotnet/registry.h>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	///
	/// UserAssociationSetting
	///
	/// <summary>
	/// CubeICE のファイルの関連付けに関連する各種ユーザ設定を取得・保存
	/// するクラスです。
	/// </summary>
	///
	/* --------------------------------------------------------------------- */
	class UserAssociationSetting : public UserSettingBase {
	public:
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		UserAssociationSetting() :
			UserSettingBase() {
			this->Reset();
		}
		
		/* ----------------------------------------------------------------- */
		/// constructor
		/* ----------------------------------------------------------------- */
		UserAssociationSetting(const UserAssociationSetting& cp) :
			UserSettingBase(cp) {}
		
		/* ----------------------------------------------------------------- */
		/// destructor
		/* ----------------------------------------------------------------- */
		virtual ~UserAssociationSetting() {}
		
		/* ----------------------------------------------------------------- */
		///
		/// Reset
		///
		/// <summary>
		/// CubeICE の関連付けに関連する設定可能な項目をここで定義します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		virtual void Reset() {
			this->Parameters().clear();
			this->Parameters().push_back(Node(_T("zip"), true));
			this->Parameters().push_back(Node(_T("lzh"), true));
			this->Parameters().push_back(Node(_T("rar"), true));
			this->Parameters().push_back(Node(_T("7z"), true));
			this->Parameters().push_back(Node(_T("arj"), false));
			this->Parameters().push_back(Node(_T("bz2"), true));
			this->Parameters().push_back(Node(_T("cab"), true));
			this->Parameters().push_back(Node(_T("chm"), false));
			this->Parameters().push_back(Node(_T("cpio"), false));
			this->Parameters().push_back(Node(_T("deb"), false));
			this->Parameters().push_back(Node(_T("dmg"), false));
			this->Parameters().push_back(Node(_T("gz"), true));
			this->Parameters().push_back(Node(_T("iso"), false));
			this->Parameters().push_back(Node(_T("jar"), false));
			this->Parameters().push_back(Node(_T("rpm"), false));
			this->Parameters().push_back(Node(_T("tar"), true));
			this->Parameters().push_back(Node(_T("tbz"), true));
			this->Parameters().push_back(Node(_T("tgz"), true));
			this->Parameters().push_back(Node(_T("wim"), false));
			this->Parameters().push_back(Node(_T("xar"), false));
			this->Parameters().push_back(Node(_T("xz"), true));
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
			int associations = old.decompression().flags();
			this->Find(_T("zip")).Value((associations & ZIP_FLAG) != 0);
			this->Find(_T("lzh")).Value((associations & LZH_FLAG) != 0);
			this->Find(_T("rar")).Value((associations & RAR_FLAG) != 0);
			this->Find(_T("7z")).Value((associations & SEVENZIP_FLAG) != 0);
			this->Find(_T("arj")).Value((associations & ARJ_FLAG) != 0);
			this->Find(_T("bz2")).Value((associations & BZ2_FLAG) != 0);
			this->Find(_T("cab")).Value((associations & CAB_FLAG) != 0);
			this->Find(_T("chm")).Value((associations & CHM_FLAG) != 0);
			this->Find(_T("cpio")).Value((associations & CPIO_FLAG) != 0);
			this->Find(_T("deb")).Value((associations & DEB_FLAG) != 0);
			this->Find(_T("dmg")).Value((associations & DMG_FLAG) != 0);
			this->Find(_T("gz")).Value((associations & GZ_FLAG) != 0);
			this->Find(_T("iso")).Value((associations & ISO_FLAG) != 0);
			this->Find(_T("jar")).Value((associations & JAR_FLAG) != 0);
			this->Find(_T("rpm")).Value((associations & RPM_FLAG) != 0);
			this->Find(_T("tar")).Value((associations & TAR_FLAG) != 0);
			this->Find(_T("tbz")).Value((associations & TBZ_FLAG) != 0);
			this->Find(_T("tgz")).Value((associations & TGZ_FLAG) != 0);
			this->Find(_T("wim")).Value((associations & WIM_FLAG) != 0);
			this->Find(_T("xar")).Value((associations & XAR_FLAG) != 0);
			this->Find(_T("xz")).Value((associations & XZ_FLAG) != 0);
		}
		
		/* ----------------------------------------------------------------- */
		/// IsAssociated
		/* ----------------------------------------------------------------- */
		bool IsAssociated(const string_type& name) {
			return this->Find(name).Value<ValueKind::Bool>();
		}
		
		/* ----------------------------------------------------------------- */
		///
		/// Synchronize
		///
		/// <summary>
		/// src 下にある値を基に HKEY_CLASSES_ROOT の関連付けを更新します。
		/// CubeICE 以外のアプリケーションが既に関連付けされている場合、
		/// PreArchiver と言うレジストリ値を作成し、そのアプリケーション名
		/// を保存します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		static void Synchronize(const PsdotNet::RegistryKey& src) {
			BOOST_FOREACH(const string_type& name, src.GetValueNames()) {
				string_type archiver = _T("cubeice_") + name;
				string_type tooltip = _T("shellex\\") CUBEICE_CLSID_TOOLTIP;
				
				try {
					if (src.GetValue<bool>(name)) { // CubeICE を関連付ける
						PsdotNet::RegistryKey subkey = PsdotNet::Registry::ClassesRoot().CreateSubKey(_T(".") + name);
						if (!subkey) continue;
						
						string_type pre_archiver = subkey.GetValue<string_type>(_T(""));
						if (!pre_archiver.empty() && pre_archiver != archiver) subkey.SetValue(_T("PreArchiver"), pre_archiver);
						subkey.SetValue(_T(""), archiver);
						
						PsdotNet::RegistryKey child = subkey.CreateSubKey(tooltip);
						child.SetValue(_T(""), CUBEICE_CLSID_CONTEXT);
					}
					else { // 関連付けを元に戻す、または削除する
						PsdotNet::RegistryKey subkey = PsdotNet::Registry::ClassesRoot().OpenSubKey(_T(".") + name, true);
						if (!subkey || subkey.GetValue<string_type>(_T("")) != archiver) continue;
						
						string_type pre_archiver = subkey.GetValue<string_type>(_T("PreArchiver"));
						if (!pre_archiver.empty()) {
							subkey.SetValue(_T(""), pre_archiver);
							subkey.DeleteValue(_T("PreArchiver"));
							subkey.DeleteSubKeyTree(tooltip);
						}
						else {
							subkey.Close();
							PsdotNet::Registry::ClassesRoot().DeleteSubKeyTree(_T(".") + name);
						}
					}
				}
				catch (const std::exception& /* err */) {
					continue;
				}
			}
			::SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_FLUSH, 0, 0);
		}
		
	protected:
		/* ----------------------------------------------------------------- */
		/// LoadFromParameterExec
		/* ----------------------------------------------------------------- */
		virtual void LoadFromParameterExec(const NodeSet& src) {
			UserSettingBase::LoadFromParameterExec(src);
			this->Synchronize();
		}
		
	private:
		/* ----------------------------------------------------------------- */
		///
		/// Synchronize
		///
		/// <summary>
		/// 保持されているパラメータ値を実際の関連付け (HKEY_CLASSES_ROOT)
		/// と同期します。
		/// </summary>
		///
		/* ----------------------------------------------------------------- */
		void Synchronize() {
			BOOST_FOREACH(Node& element, this->Parameters()) {
				bool is_associated = false;
				try {
					string_type name = _T(".") + element.Name();
					PsdotNet::RegistryKey subkey = PsdotNet::Registry::ClassesRoot().OpenSubKey(name, false);
					string_type value = _T("cubeice_") + element.Name();
					is_associated = (subkey && subkey.GetValue<string_type>(_T("")) == value);
				}
				catch (const std::exception& /* err */) {
					is_associated = false;
				}
				element.Value(is_associated);
			}
		}
	}; // class UserAssociationSetting
} // namespace CubeICE

#endif // CUBEICE_USER_SETTING_USER_ASSOCIATION_SETTING_H
