// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/shortcut.h
 *
 *  Copyright (c) 2010 CubeSoft Inc.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_SHORTCUT_H
#define CUBEICE_SHORTCUT_H

#include <cubeice/config.h>

namespace CubeICE {
	/* --------------------------------------------------------------------- */
	/*!
	 *  Shortcut
	 *
	 *  ショートカットの各種情報を保持するためのクラスです。
	 */
	/* --------------------------------------------------------------------- */
	class Shortcut {
	public:
		typedef CubeICE::char_type char_type;
		typedef CubeICE::string_type string_type;
		
	public:
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		Shortcut() :
			name_(), directory_(), target_(), arguments_(), icon_() {}
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		Shortcut(const string_type& name, const string_type& directory, const string_type& target) :
			name_(name), directory_(directory), target_(target),
			arguments_(), icon_() {}
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		Shortcut(const string_type& name, const string_type& directory, const string_type& target,
				 const string_type& arguments, const string_type& icon) :
			name_(name), directory_(directory), target_(target),
			arguments_(arguments), icon_(icon) {}
		
		/* ----------------------------------------------------------------- */
		//  destructor
		/* ----------------------------------------------------------------- */
		virtual ~Shortcut() {}
		
		/* ----------------------------------------------------------------- */
		//  Name
		/* ----------------------------------------------------------------- */
		const string_type& Name() const { return name_; }
		
		/* ----------------------------------------------------------------- */
		//  Name
		/* ----------------------------------------------------------------- */
		void Name(const string_type& name) { name_ = name; }
		
		/* ----------------------------------------------------------------- */
		//  Directory
		/* ----------------------------------------------------------------- */
		const string_type& Directory() const { return directory_; }
		
		/* ----------------------------------------------------------------- */
		//  Directory
		/* ----------------------------------------------------------------- */
		void Directory(const string_type& path) { directory_ = path; }
		
		/* ----------------------------------------------------------------- */
		//  TargetPath
		/* ----------------------------------------------------------------- */
		const string_type& TargetPath() const { return target_; }
		
		/* ----------------------------------------------------------------- */
		//  TargetPath
		/* ----------------------------------------------------------------- */
		void TargetPath(const string_type& path) { target_ = path; }
		
		/* ----------------------------------------------------------------- */
		//  Arguments
		/* ----------------------------------------------------------------- */
		const string_type& Arguments() const { return arguments_; }
		
		/* ----------------------------------------------------------------- */
		//  Arguments
		/* ----------------------------------------------------------------- */
		void Arguments(const string_type& str) { arguments_ = str; }
		
		/* ----------------------------------------------------------------- */
		//  IconLocation
		/* ----------------------------------------------------------------- */
		const string_type& IconLocation() const { return icon_; }
		
		/* ----------------------------------------------------------------- */
		//  IconLocation
		/* ----------------------------------------------------------------- */
		void IconLocation(const string_type& location) { icon_ = location; }
		
	private:
		string_type name_;
		string_type directory_;
		string_type target_;
		string_type arguments_;
		string_type icon_;
	};
} // namespace CubeICE

#endif // CUBEICE_SHORTCUT_H
