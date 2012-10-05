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

BOOST_AUTO_TEST_SUITE(UserCompressionSettingTest)

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
		CubeICE::UserCompressionSetting& setting = root.Compression();
		BOOST_CHECK(setting.Parameters().size() == 8);
		boost::optional<Node&> pt;
		
		pt = setting.FindOptional(_T("OutputCondition"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == CubeICE::OutputKind::Specific);
		BOOST_CHECK(setting.OutputCondition() == CubeICE::OutputKind::Specific);
		
		pt = setting.FindOptional(_T("OutputPath"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::String);
		BOOST_CHECK(pt->Value<ValueKind::String>().empty());
		BOOST_CHECK(setting.OutputPath().empty());
		
		pt = setting.FindOptional(_T("DefaultType"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::String);
		BOOST_CHECK(pt->Value<ValueKind::String>() == _T("zip"));
		
		pt = setting.FindOptional(_T("DefaultEnablePassword"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.DefaultEnablePassword() == false);
		
		pt = setting.FindOptional(_T("Overwrite"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == CubeICE::OverwriteKind::Confirm);
		BOOST_CHECK(setting.Overwrite() == CubeICE::OverwriteKind::Confirm);
		
		pt = setting.FindOptional(_T("Filtering"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.Filtering() == false);
		
		pt = setting.FindOptional(_T("Open"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == CubeICE::OpenKind::Skip);
		BOOST_CHECK(setting.Open() == CubeICE::OpenKind::Skip);
		
		pt = setting.FindOptional(_T("DeleteOnMail"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.DeleteOnMail() == false);
	}
	catch (std::exception& err) {
		BOOST_FAIL(err.what());
	}
}

BOOST_AUTO_TEST_SUITE_END()
