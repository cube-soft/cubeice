// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  cubeice/context.h
 *
 *  Copyright (c) 2010 CubeSoft Inc.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#ifndef CUBEICE_CONTEXT_H
#define CUBEICE_CONTEXT_H

#include <cubeice/config.h>
#include <vector>
#include <algorithm>

namespace CubeICE {
	/* ---------------------------------------------------------------------- */
	/*!
	 *  Context
	 *
	 *  コンテキストメニューの各種データを管理するためのクラスです。
	 */
	/* ---------------------------------------------------------------------- */
	class Context {
	public:
		typedef CubeICE::char_type char_type;
		typedef CubeICE::string_type string_type;
		typedef std::vector<Context> container_type;
		
	public:
		/* ------------------------------------------------------------------ */
		//  constructor
		/* ------------------------------------------------------------------ */
		Context() :
			index_(-1), name_(),
			target_(), arguments_(),
			icon_(), children_() {}
		
		/* ------------------------------------------------------------------ */
		//  constructor
		/* ------------------------------------------------------------------ */
		explicit Context(const string_type& name) :
			index_(-1), name_(name), target_(), arguments_(), icon_(), children_() {}
		
		/* ------------------------------------------------------------------ */
		//  constructor
		/* ------------------------------------------------------------------ */
		Context(const string_type& name, const string_type& icon) :
			index_(-1), name_(name), target_(), arguments_(), icon_(icon), children_() {}
			
		/* ------------------------------------------------------------------ */
		//  constructor
		/* ------------------------------------------------------------------ */
		Context(const string_type& name, const string_type& target, const string_type& arguments, const string_type& icon) :
			index_(-1), name_(name), target_(target), arguments_(arguments), icon_(icon), children_() {}
		
		/* ------------------------------------------------------------------ */
		//  constructor
		/* ------------------------------------------------------------------ */
		Context(const Context& cp) :
			index_(cp.index_), name_(cp.name_), target_(cp.target_), arguments_(cp.arguments_),
			icon_(cp.icon_), children_(cp.children_) {}
		
		/* ------------------------------------------------------------------ */
		//  destructor
		/* ------------------------------------------------------------------ */
		virtual ~Context() {}
		
		/* ------------------------------------------------------------------ */
		//  operator=
		/* ------------------------------------------------------------------ */
		Context& operator=(const Context& cp) {
			index_ = cp.index_;
			name_ = cp.name_;
			target_ = cp.target_;
			arguments_ = cp.arguments_;
			icon_ = cp.icon_;
			children_ = cp.children_;
			
			return *this;
		}
		
		/* ------------------------------------------------------------------ */
		///
		/// Empty
		///
		/// <summary>
		/// 空の項目であるかどうかを判定します。「空の項目」とは、コマンド
		/// が指定されておらず、子要素も持ってない項目の事を指します。
		/// </summary>
		///
		/* ------------------------------------------------------------------ */
		bool Empty() const { return target_.empty() && children_.empty(); }
		
		/* ------------------------------------------------------------------ */
		//  Index
		/* ------------------------------------------------------------------ */
		const int& Index() const { return index_; }
		
		/* ------------------------------------------------------------------ */
		//  Index
		/* ------------------------------------------------------------------ */
		void Index(int n) { index_ = n; }
		
		/* ------------------------------------------------------------------ */
		//  Name
		/* ------------------------------------------------------------------ */
		const string_type& Name() const { return name_; }
		
		/* ------------------------------------------------------------------ */
		//  Name
		/* ------------------------------------------------------------------ */
		void Name(const string_type& str) { name_ = str; }
		
		/* ------------------------------------------------------------------ */
		//  TargetPath
		/* ------------------------------------------------------------------ */
		const string_type& TargetPath() const { return target_; }
		
		/* ------------------------------------------------------------------ */
		//  TargetPath
		/* ------------------------------------------------------------------ */
		void TargetPath(const string_type& str) { target_ = str; }
		
		/* ------------------------------------------------------------------ */
		//  Arguments
		/* ------------------------------------------------------------------ */
		const string_type& Arguments() const { return arguments_; }
		
		/* ------------------------------------------------------------------ */
		//  Arguments
		/* ------------------------------------------------------------------ */
		void Arguments(const string_type& str) { arguments_ = str; }
		
		/* ----------------------------------------------------------------- */
		//  IconLocation
		/* ----------------------------------------------------------------- */
		const string_type& IconLocation() const { return icon_; }
		
		/* ----------------------------------------------------------------- */
		//  IconLocation
		/* ----------------------------------------------------------------- */
		void IconLocation(const string_type& location) { icon_ = location; }
		
		/* ------------------------------------------------------------------ */
		//  Children
		/* ------------------------------------------------------------------ */
		container_type& Children() { return children_; }
		const container_type& Children() const { return children_; }
		
		/* ------------------------------------------------------------------ */
		//  比較演算子
		/* ------------------------------------------------------------------ */
		friend bool operator==(const Context& lhs, const Context& rhs) { return lhs.index_ == rhs.index_; }
		friend bool operator!=(const Context& lhs, const Context& rhs) { return lhs.index_ != rhs.index_; }
		friend bool operator< (const Context& lhs, const Context& rhs) { return lhs.index_ <  rhs.index_; }
		friend bool operator<=(const Context& lhs, const Context& rhs) { return lhs.index_ <= rhs.index_; }
		friend bool operator> (const Context& lhs, const Context& rhs) { return lhs.index_ >  rhs.index_; }
		friend bool operator>=(const Context& lhs, const Context& rhs) { return lhs.index_ >= rhs.index_; }
		
	private:
		int index_;
		string_type name_;
		string_type target_;
		string_type arguments_;
		string_type icon_;
		container_type children_;
	}; // class Context
	
	
	/* ---------------------------------------------------------------------- */
	//  ContextContainer
	/* ---------------------------------------------------------------------- */
	typedef std::vector<Context> ContextContainer;
} // namespace CubeICE

#endif // CUBEICE_CONTEXT_H
