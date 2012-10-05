// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  runtime-compression-setting-test.cpp
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

BOOST_AUTO_TEST_SUITE(RuntimeCompressionSettingTest)

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
		CubeICE::RuntimeCompressionSetting& setting = root.Runtime().Compression();
		BOOST_CHECK(setting.Parameters().size() == 8);
		boost::optional<Node&> pt;
		
		pt = setting.FindOptional(_T("Type"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::String);
		BOOST_CHECK(pt->Value<ValueKind::String>() == _T("zip"));
		BOOST_CHECK(setting.Type() == _T("zip"));
		
		pt = setting.FindOptional(_T("Level"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == CubeICE::CompressionLevel::Normal);
		BOOST_CHECK(setting.Level() == CubeICE::CompressionLevel::Normal);
		
		pt = setting.FindOptional(_T("Method"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::String);
		BOOST_CHECK(pt->Value<ValueKind::String>() == _T("Deflate"));
		BOOST_CHECK(setting.Method() == _T("Deflate"));
		
		pt = setting.FindOptional(_T("EnablePassword"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.EnablePassword() == false);
		
		pt = setting.FindOptional(_T("ShowPassword"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.ShowPassword() == false);
		
		pt = setting.FindOptional(_T("EncodingMethod"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::String);
		BOOST_CHECK(pt->Value<ValueKind::String>() == _T("ZipCrypto"));
		BOOST_CHECK(setting.EncodingMethod() == _T("ZipCrypto"));
		
		pt = setting.FindOptional(_T("Overwrite"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == CubeICE::OverwriteKind::Confirm);
		BOOST_CHECK(setting.Overwrite() == CubeICE::OverwriteKind::Confirm);
		
		pt = setting.FindOptional(_T("ThreadSize"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == 1);
		BOOST_CHECK(setting.ThreadSize() == 1);
	}
	catch (std::exception& err) {
		BOOST_FAIL(err.what());
	}
}

BOOST_AUTO_TEST_SUITE_END()
