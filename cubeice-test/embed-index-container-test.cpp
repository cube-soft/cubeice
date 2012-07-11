// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  embed-index-container-test.cpp
 *
 *  Copyright (c) 2011, clown.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#include <boost/test/unit_test.hpp>
#include <cubeice/embed-index-container.h>
#include <cubeice/context.h>
#include "cubeice-test.h"

BOOST_AUTO_TEST_SUITE(EmbedIndexContainerTest)

BOOST_AUTO_TEST_CASE(TestMemberFunctions) {
	typedef CubeICE::EmbedIndexContainer<CubeICE::Context> container_type;

	container_type v;
	v.push_back(CubeICE::Context(_T("foo"), _T("description"), _T("cubeice.exe /c"), 0));
	v.push_back(CubeICE::Context(_T("bar"), _T("description"), _T("cubeice.exe /c"), 0));
	v.push_back(CubeICE::Context(_T("bas"), _T("description"), _T("cubeice.exe /c"), 0));
	v.push_back(CubeICE::Context(_T("hoge"), _T("description"), _T("cubeice.exe /c"), 0));
	v.push_back(CubeICE::Context(_T("fuga"), _T("description"), _T("cubeice.exe /c"), 0));

	BOOST_CHECK(v.size() == 5);
	
	for (container_type::iterator pos = v.begin(); pos != v.end(); ++pos) {
		BOOST_TEST_CHECKPOINT(_T("iterator test"));
	}

	for (container_type::const_iterator pos = v.begin(); pos != v.end(); ++pos) {
		BOOST_TEST_CHECKPOINT(_T("const_iterator test"));
	}

	for (container_type::size_type i = 0; i < v.size(); ++i) {
		BOOST_CHECK(v.at(i).Index() == i);
	}

	v.insert(v.begin(), CubeICE::Context(_T("boke"), _T("description"), _T("cubeice.exe /c"), 0));

	for (container_type::size_type i = 0; i < v.size(); ++i) {
		BOOST_CHECK(v.at(i).Index() == i);
	}

	container_type::iterator erase_position = v.begin();
	++erase_position;
	v.erase(erase_position);

	for (container_type::size_type i = 0; i < v.size(); ++i) {
		BOOST_CHECK(v.at(i).Index() == i);
	}

	v.clear();
	BOOST_CHECK(v.empty());
}

BOOST_AUTO_TEST_SUITE_END()