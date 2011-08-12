// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  psdotnet/registry-key.h
 *
 *  Copyright (c) 2011, clown.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#ifndef PSDOTNET_REGISTRY_KEY_H
#define PSDOTNET_REGISTRY_KEY_H

#include <psdotnet/config.h>
#include <string>
#include <windows.h>
#include <tchar.h>
#include <psdotnet/typesafe-enum.h>

namespace PsdotNet {
	/* --------------------------------------------------------------------- */
	//  RegistryRights
	/* --------------------------------------------------------------------- */
	namespace Detail {
		struct RegistryRights_ {
			enum type {
				QueryValues             = KEY_QUERY_VALUE,
				SetValue                = KEY_SET_VALUE,
				CreateSubKey            = KEY_CREATE_SUB_KEY,
				EnumerateSubKeys        = KEY_ENUMERATE_SUB_KEYS,
				Notify                  = KEY_NOTIFY,
				CreateLink              = KEY_CREATE_LINK,
				ExecuteKey              = KEY_EXECUTE,
				ReadKey                 = KEY_READ,
				WriteKey                = KEY_WRITE,
				//Delete                  = DELETE,
				//ReadPermission          = 
				//ChangePermission
				//TakeOwnership
				FullControl             = KEY_ALL_ACCESS
			};
		};
	}
	typedef TypesafeEnum<Detail::RegistryRights_> RegistryRights;
	
	/* --------------------------------------------------------------------- */
	//  RegistryKey
	/* --------------------------------------------------------------------- */
	class RegistryKey {
	public:
		typedef TCHAR char_type;
		typedef std::basic_string<TCHAR> string_type;
		typedef std::size_t size_type;
		typedef HKEY handle_type;
		
	public:
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		RegistryKey() : handle_(NULL) {}
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		RegistryKey(const RegistryKey& cp) : handle_(cp.handle_) {}
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		explicit RegistryKey(const handle_type& cp) : handle_(cp) {}
		
		/* ----------------------------------------------------------------- */
		//  destructor
		/* ----------------------------------------------------------------- */
		~RegistryKey() { this->Close(); }
		
		/* ----------------------------------------------------------------- */
		//  operator bool()
		/* ----------------------------------------------------------------- */
		operator bool() { return handle_ != NULL; }
		
		/* ----------------------------------------------------------------- */
		//  operator=
		/* ----------------------------------------------------------------- */
		RegistryKey& operator=(const RegistryKey& cp) {
			handle_ = cp.handle_;
			return *this;
		}
		
		/* ----------------------------------------------------------------- */
		//  operator=
		/* ----------------------------------------------------------------- */
		RegistryKey& operator=(const handle_type& cp) {
			handle_ = cp;
			return *this;
		}
		
		/* ----------------------------------------------------------------- */
		/*
		 *  Close
		 *
		 *  システムキー (HKEY_CLASSES_ROOT, HKEY_CURRENT_CONFIG,
		 *  HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, HKEY_PERFORMANCE_DATA,
		 *  HKEY_USERS) の場合は何もしない．
		 */
		/* ----------------------------------------------------------------- */
		void Close() {
			if (handle_ &&
			    handle_ != HKEY_CLASSES_ROOT &&
			    handle_ != HKEY_CURRENT_CONFIG &&
			    handle_ != HKEY_CURRENT_USER &&
			    handle_ != HKEY_LOCAL_MACHINE &&
			    handle_ != HKEY_PERFORMANCE_DATA &&
			    handle_ != HKEY_USERS) {
				RegCloseKey(handle_);
				handle_ = NULL;
			}
		}
		
		/* ----------------------------------------------------------------- */
		//  OpenSubKey
		/* ----------------------------------------------------------------- */
		RegistryKey OpenSubKey(const string_type& subkey) const {
			handle_type dest = NULL;
			return this->OpenSubKeyExec(subkey, RegistryRights::FullControl);
		}
		
		/* ----------------------------------------------------------------- */
		//  OpenSubKey
		/* ----------------------------------------------------------------- */
		RegistryKey OpenSubKey(const string_type& subkey, bool writable) const {
			RegistryRights rights = writable ? RegistryRights::FullControl : RegistryRights::ReadKey;
			return this->OpenSubKeyExec(subkey, rights);
		}
		
		/* ----------------------------------------------------------------- */
		//  Handle
		/* ----------------------------------------------------------------- */
		const handle_type& Handle() const { return handle_; }
		
		/* ----------------------------------------------------------------- */
		//  Handle
		/* ----------------------------------------------------------------- */
		handle_type& Handle() { return handle_; }
		
	private:
		/* ----------------------------------------------------------------- */
		//  OpenSubKeyExec
		/* ----------------------------------------------------------------- */
		RegistryKey OpenSubKeyExec(const string_type& subkey, const RegistryRights& rights) const {
			handle_type dest = NULL;
			if (handle_ && RegOpenKeyEx(handle_, subkey.c_str(), 0, rights.ToEnum(), &dest) == ERROR_SUCCESS) {
				return RegistryKey(dest);
			}
			return RegistryKey();
		}
		
	private:
		handle_type handle_;
	};
} // namespace PsdotNet

#endif // PSDOTNET_REGISTRY_KEY_H
