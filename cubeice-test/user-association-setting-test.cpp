// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  user-compression-setting-test.cpp
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
#include <boost/test/unit_test.hpp>
#include <cubeice/user-setting.h>
#include "cubeice-test.h"

BOOST_AUTO_TEST_SUITE(UserAssociationSettingTest)

typedef CubeICE::UserSettingBase::ValueKind ValueKind;
typedef CubeICE::UserSettingBase::Node Node;
typedef CubeICE::UserSettingBase::NodeSet NodeSet;

/* ------------------------------------------------------------------------- */
///
/// TestDefaultValue
///
/// <summary>
/// 各種設定の初期値のテスト。
/// </summary>
///
/* ------------------------------------------------------------------------- */
BOOST_AUTO_TEST_CASE(TestDefaultValue) {
	try {
		CubeICE::UserSetting root;
		CubeICE::UserAssociationSetting& setting = root.Association();
		BOOST_CHECK(setting.Parameters().size() == 21);
		boost::optional<Node&> pt;
		
		pt = setting.FindOptional(_T("zip"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.IsAssociated(_T("zip")) == true);
		
		pt = setting.FindOptional(_T("lzh"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.IsAssociated(_T("lzh")) == true);
		
		pt = setting.FindOptional(_T("rar"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.IsAssociated(_T("rar")) == true);
		
		pt = setting.FindOptional(_T("7z"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.IsAssociated(_T("7z")) == true);
		
		pt = setting.FindOptional(_T("arj"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.IsAssociated(_T("arj")) == false);
		
		pt = setting.FindOptional(_T("bz2"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.IsAssociated(_T("bz2")) == true);
		
		pt = setting.FindOptional(_T("cab"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.IsAssociated(_T("cab")) == true);
		
		pt = setting.FindOptional(_T("chm"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.IsAssociated(_T("chm")) == false);
		
		pt = setting.FindOptional(_T("cpio"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.IsAssociated(_T("cpio")) == false);
		
		pt = setting.FindOptional(_T("deb"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.IsAssociated(_T("deb")) == false);
		
		pt = setting.FindOptional(_T("dmg"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.IsAssociated(_T("dmg")) == false);
		
		pt = setting.FindOptional(_T("gz"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.IsAssociated(_T("gz")) == true);
		
		pt = setting.FindOptional(_T("iso"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.IsAssociated(_T("iso")) == false);
		
		pt = setting.FindOptional(_T("jar"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.IsAssociated(_T("jar")) == false);
		
		pt = setting.FindOptional(_T("rpm"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.IsAssociated(_T("rpm")) == false);
		
		pt = setting.FindOptional(_T("tar"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.IsAssociated(_T("tar")) == true);
		
		pt = setting.FindOptional(_T("tbz"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.IsAssociated(_T("tbz")) == true);
		
		pt = setting.FindOptional(_T("tgz"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.IsAssociated(_T("tgz")) == true);
		
		pt = setting.FindOptional(_T("wim"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.IsAssociated(_T("wim")) == false);
		
		pt = setting.FindOptional(_T("xar"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.IsAssociated(_T("xar")) == false);
		
		pt = setting.FindOptional(_T("xz"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.IsAssociated(_T("xz")) == true);
	}
	catch (std::exception& err) {
		BOOST_FAIL(err.what());
	}
}

BOOST_AUTO_TEST_SUITE_END()
