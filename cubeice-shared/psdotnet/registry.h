// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  psdotnet/registry.h
 *
 *  Copyright (c) 2011, clown.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#ifndef PSDOTNET_REGISTRY_H
#define PSDOTNET_REGISTRY_H

#include <psdotnet/config.h>
#include <psdotnet/registry-key.h>
#include <windows.h>
#include <tchar.h>
#include <boost/utility.hpp>

namespace PsdotNet {
	/* --------------------------------------------------------------------- */
	//  Registry
	/* --------------------------------------------------------------------- */
	class Registry : boost::noncopyable {
	public:
		typedef RegistryKey::size_type size_type;
		typedef RegistryKey::char_type char_type;
		typedef RegistryKey::string_type string_type;
		
	public:
		/* ----------------------------------------------------------------- */
		/*
		 *  access methods
		 *
		 *  予約済みハンドルの取得．
		 */
		/* ----------------------------------------------------------------- */
		static RegistryKey ClassesRoot() { return RegistryKey(HKEY_CLASSES_ROOT); }
		static RegistryKey CurrentConfig() { return RegistryKey(HKEY_CURRENT_CONFIG); }
		static RegistryKey CurrentUser() { return RegistryKey(HKEY_CURRENT_USER); }
		static RegistryKey LocalMachine() { return RegistryKey(HKEY_LOCAL_MACHINE); }
		static RegistryKey Users() { return RegistryKey(HKEY_USERS); }
		static RegistryKey PerformanceData() { return RegistryKey(HKEY_PERFORMANCE_DATA); }
		
	private:
		Registry();
		~Registry();
	};
} // namespace PsdotNet

#endif // PSDOTNET_REGISTRY_H
