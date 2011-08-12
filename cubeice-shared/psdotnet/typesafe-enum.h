// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  psdotnet/typesafe-enum.h
 *
 *  Copyright (c) 2011, clown.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#ifndef PSDOTNET_TYPESAFE_ENUM_H
#define PSDOTNET_TYPESAFE_ENUM_H

namespace PsdotNet {
	/* --------------------------------------------------------------------- */
	/*
	 *  TypesafeEnum
	 *
	 *  The class is an implementation of Typesafe Enum idiom.
	 *  http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Safe_Enum
	 */
	/* --------------------------------------------------------------------- */
	template<typename DefT, typename InnerT = typename DefT::type>
	class TypesafeEnum : public DefT {
	private:
		typedef typename DefT::type type;
		InnerT value_;
		
	public:
		TypesafeEnum(type x) : value_(x) {}
		InnerT ToEnum() const { return value_; }
		
		bool operator==(const TypesafeEnum& s) const { return this->value_ == s.value_; }
		bool operator!=(const TypesafeEnum& s) const { return this->value_ != s.value_; }
		bool operator< (const TypesafeEnum& s) const { return this->value_ <  s.value_; }
		bool operator<=(const TypesafeEnum& s) const { return this->value_ <= s.value_; }
		bool operator> (const TypesafeEnum& s) const { return this->value_ >  s.value_; }
		bool operator>=(const TypesafeEnum& s) const { return this->value_ >= s.value_; }
	};
}

#endif // PSDOTNET_TYPESAFE_ENUM_H
