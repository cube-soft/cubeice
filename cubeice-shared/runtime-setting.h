/* ------------------------------------------------------------------------- */
/*
 *  runtime-setting.h
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
#ifndef CUBEICE_RUNTIME_SETTING_H
#define CUBEICE_RUNTIME_SETTING_H

#include <tchar.h>
#include <windows.h>
#include <winreg.h>
#include "user-setting.h"

/* ------------------------------------------------------------------------- */
//  レジストリ
/* ------------------------------------------------------------------------- */
#define CUBEICE_REG_RUNTIME             _T("Software\\CubeSoft\\CubeICE\\Runtime")
#define CUBEICE_REG_COMPTYPE            _T("CompressType")
#define CUBEICE_REG_COMPLEVEL           _T("CompressLevel")
#define CUBEICE_REG_COMPMETHOD          _T("CompressMethod")
#define CUBEICE_REG_THREAD_SIZE         _T("ThreadSize")
#define CUBEICE_REG_ENABLE_PASSWORD     _T("EnablePassword")
#define CUBEICE_REG_SHOW_PASSWORD       _T("ShowPassword")
#define CUBEICE_REG_ENMETHOD            _T("EncodingMethod")

namespace cubeice {
	class runtime_setting {
	public:
		typedef TCHAR char_type;
		typedef std::basic_string<TCHAR> string_type;
		typedef std::size_t size_type;
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		runtime_setting() :
			path_(), type_(), level_(5), method_(), thread_size_(1),
			enable_password_(false), show_password_(false), password_(), encoding_() {
			this->load();
		}
		
		/* ----------------------------------------------------------------- */
		//  load
		/* ----------------------------------------------------------------- */
		void load() {
			HKEY subkey;
			if (RegOpenKeyEx(HKEY_CURRENT_USER, CUBEICE_REG_RUNTIME, 0, KEY_ALL_ACCESS, &subkey)) return;
			
			// 圧縮形式
			char_type buffer[1024] = {};
			DWORD type = 0;
			DWORD size = sizeof(buffer);
			if (RegQueryValueEx(subkey, CUBEICE_REG_COMPTYPE, NULL, &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS) type_ = buffer;
			
			// 圧縮レベル
			type = 0;
			size = sizeof(level_);
			RegQueryValueEx(subkey, CUBEICE_REG_COMPLEVEL, NULL, &type, (LPBYTE)&level_, &size);
			
			// 圧縮メソッド
			type = 0;
			size = sizeof(buffer);
			ZeroMemory(buffer, size);
			if (RegQueryValueEx(subkey, CUBEICE_REG_COMPMETHOD, NULL, &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS) method_ = buffer;
			
			// CPU スレッド数
			type = 0;
			size = sizeof(thread_size_);
			RegQueryValueEx(subkey, CUBEICE_REG_THREAD_SIZE, NULL, &type, (LPBYTE)&thread_size_, &size);
			
			// パスワードを設定する
			DWORD value = 0;
			type = 0;
			size = sizeof(value);
			RegQueryValueEx(subkey, CUBEICE_REG_ENABLE_PASSWORD, NULL, &type, (LPBYTE)&value, &size);
			enable_password_ = (value != 0);
			
			// パスワードを表示する
			value = 0;
			type = 0;
			size = sizeof(value);
			RegQueryValueEx(subkey, CUBEICE_REG_SHOW_PASSWORD, NULL, &type, (LPBYTE)&value, &size);
			show_password_ = (value != 0);
			
			// 暗号化メソッド
			type = 0;
			size = sizeof(buffer);
			ZeroMemory(buffer, size);
			if (RegQueryValueEx(subkey, CUBEICE_REG_ENMETHOD, NULL, &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS) encoding_ = buffer;
		}
		
		/* ----------------------------------------------------------------- */
		//  save
		/* ----------------------------------------------------------------- */
		void save() {
			HKEY subkey;
			DWORD disposition;
			if (RegCreateKeyEx(HKEY_CURRENT_USER, CUBEICE_REG_RUNTIME, 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &subkey, &disposition)) return;
			
			// 圧縮形式
			if (!type_.empty()) {
				DWORD byte = (type_.length() + 1) * sizeof(char_type);
				RegSetValueEx(subkey, CUBEICE_REG_COMPTYPE, 0, REG_SZ, (CONST BYTE*)type_.c_str(), byte);
			}
			else RegDeleteValue(subkey, CUBEICE_REG_COMPTYPE);
			
			// 圧縮レベル
			DWORD value = static_cast<DWORD>(level_);
			RegSetValueEx(subkey, CUBEICE_REG_COMPLEVEL, 0, REG_DWORD, (CONST BYTE*)&value, sizeof(value));
			
			// 圧縮メソッド
			if (!method_.empty()) {
				DWORD byte = (method_.length() + 1) * sizeof(char_type);
				RegSetValueEx(subkey, CUBEICE_REG_COMPMETHOD, 0, REG_SZ, (CONST BYTE*)method_.c_str(), byte);
			}
			else RegDeleteValue(subkey, CUBEICE_REG_COMPMETHOD);
			
			// CPU スレッド数
			value = static_cast<DWORD>(thread_size_);
			RegSetValueEx(subkey, CUBEICE_REG_THREAD_SIZE, 0, REG_DWORD, (CONST BYTE*)&value, sizeof(value));
			
			// パスワードを設定する
			value = enable_password_ ? 1 : 0;
			RegSetValueEx(subkey, CUBEICE_REG_ENABLE_PASSWORD, 0, REG_DWORD, (CONST BYTE*)&value, sizeof(value));
			
			// パスワードを表示する
			value = show_password_ ? 1 : 0;
			RegSetValueEx(subkey, CUBEICE_REG_SHOW_PASSWORD, 0, REG_DWORD, (CONST BYTE*)&value, sizeof(value));
			
			// 暗号化メソッド
			if (!encoding_.empty()) {
				DWORD byte = (encoding_.length() + 1) * sizeof(char_type);
				RegSetValueEx(subkey, CUBEICE_REG_ENMETHOD, 0, REG_SZ, (CONST BYTE*)encoding_.c_str(), byte);
			}
			else RegDeleteValue(subkey, CUBEICE_REG_ENMETHOD);
		}
		
		/* ----------------------------------------------------------------- */
		//  path
		/* ----------------------------------------------------------------- */
		string_type& path() { return path_; }
		const string_type path() const { return path_; }
		
		/* ----------------------------------------------------------------- */
		//  type
		/* ----------------------------------------------------------------- */
		string_type& type() { return type_; }
		const string_type& type() const { return type_; }
		
		/* ----------------------------------------------------------------- */
		//  level
		/* ----------------------------------------------------------------- */
		size_type& level() { return level_; }
		const size_type& level() const { return level_; }
		
		/* ----------------------------------------------------------------- */
		//  method
		/* ----------------------------------------------------------------- */
		string_type& method() { return method_; }
		const string_type& method() const { return method_; }
		
		/* ----------------------------------------------------------------- */
		//  thread_size
		/* ----------------------------------------------------------------- */
		size_type& thread_size() { return thread_size_; }
		const size_type& thread_size() const { return thread_size_; }
		
		/* ----------------------------------------------------------------- */
		//  enable_password
		/* ----------------------------------------------------------------- */
		bool& enable_password() { return enable_password_; }
		const bool& enable_password() const { return enable_password_; }
		
		/* ----------------------------------------------------------------- */
		//  show_password
		/* ----------------------------------------------------------------- */
		bool& show_password() { return show_password_; }
		const bool& show_password() const { return show_password_; }
		
		/* ----------------------------------------------------------------- */
		//  password
		/* ----------------------------------------------------------------- */
		string_type& password() { return password_; }
		const string_type& password() const { return password_; }
		
		/* ----------------------------------------------------------------- */
		//  encoding
		/* ----------------------------------------------------------------- */
		string_type& encoding() { return encoding_; }
		const string_type& encoding() const { return encoding_; }
		
	private:
		string_type path_;
		string_type type_;
		size_type level_;
		string_type method_;
		size_type thread_size_;
		bool enable_password_;
		bool show_password_;
		string_type password_;
		string_type encoding_;
	};
}

#endif // CUBEICE_RUNTIME_SETTING_H
