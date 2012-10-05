// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  user-context-setting-test.cpp
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

BOOST_AUTO_TEST_SUITE(UserContextSettingTest)

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
		CubeICE::UserContextSetting& setting = root.Context();
		BOOST_CHECK(setting.Parameters().size() == 2);
		boost::optional<Node&> pt;
		
		pt = setting.FindOptional(_T("IsExtended"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.IsExtended() == false);
		
		pt = setting.FindOptional(_T("Builtin"));
		int flags = CubeICE::ContextKind::Compression       |
			CubeICE::ContextKind::CompressionZip            |
			CubeICE::ContextKind::CompressionZipPassword    |
			CubeICE::ContextKind::Compression7z             |
			CubeICE::ContextKind::CompressionBzip2          |
			CubeICE::ContextKind::CompressionGzip           |
			CubeICE::ContextKind::CompressionDetail         |
			CubeICE::ContextKind::CompressionSelfExecutable |
			CubeICE::ContextKind::Decompression             |
			CubeICE::ContextKind::DecompressionHere         |
			CubeICE::ContextKind::DecompressionDesktop      |
			CubeICE::ContextKind::DecompressionRuntime      |
			CubeICE::ContextKind::DecompressionMyDocuments;
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == flags);
		BOOST_CHECK(setting.Builtin() == flags);
		
		BOOST_CHECK(setting.ExtendedContext().empty());
	}
	catch (std::exception& err) {
		BOOST_FAIL(err.what());
	}
}

/* ------------------------------------------------------------------------- */
///
/// TestAddExtendedContext
///
/// <summary>
/// ユーザが独自に定義したコンテキストメニューを追加するテスト。
/// </summary>
///
/* ------------------------------------------------------------------------- */
BOOST_AUTO_TEST_CASE(TestAddExtendedContext) {
	try {
		UserSettingSaver saver;
		
		// テストデータを作成してレジストリにいったん保存する。
		{
			CubeICE::UserSetting src;
			CubeICE::ContextContainer& root = src.Context().ExtendedContext();
			CubeICE::string_type target = src.InstallDirectory() + _T("\\") + CUBEICE_NAME;
			
			CubeICE::Context compression(CubeICE::Context(_T("圧縮")));
			compression.Children().push_back(CubeICE::Context(_T("zip"), target, _T("/c:zip"), _T("")));
			compression.Children().push_back(CubeICE::Context(_T("7z"), target, _T("/c:zip"), _T("")));
			BOOST_CHECK(compression.Children().size() == 2);
			root.push_back(compression);
			
			CubeICE::Context decompression(CubeICE::Context(_T("解凍")));
			decompression.Children().push_back(CubeICE::Context(_T("ここに解凍"), target, _T("/x:here"),_T("")));
			decompression.Children().push_back(CubeICE::Context(_T("場所を指定して解凍"), target, _T("/x:here"), _T("")));
			decompression.Children().push_back(CubeICE::Context(_T("デスクトップに解凍"), target, _T("/x:here"), _T("")));
			BOOST_CHECK(decompression.Children().size() == 3);
			root.push_back(decompression);
			
			BOOST_CHECK(root.size() == 2);
			BOOST_CHECK(root.at(0).Name() == _T("圧縮"));
			BOOST_CHECK(root.at(1).Name() == _T("解凍"));
			BOOST_CHECK(root.at(0).Children().size() == 2);
			BOOST_CHECK(root.at(0).Children().at(0).Name() == _T("zip"));
			BOOST_CHECK(root.at(0).Children().at(0).TargetPath() == target);
			BOOST_CHECK(root.at(0).Children().at(0).Arguments().size() > 0);
			BOOST_CHECK(root.at(0).Children().at(1).Name() == _T("7z"));
			BOOST_CHECK(root.at(0).Children().at(1).TargetPath() == target);
			BOOST_CHECK(root.at(0).Children().at(1).Arguments().size() > 0);
			BOOST_CHECK(root.at(1).Children().size() == 3);
			BOOST_CHECK(root.at(1).Children().at(0).Name() == _T("ここに解凍"));
			BOOST_CHECK(root.at(1).Children().at(0).TargetPath() == target);
			BOOST_CHECK(root.at(1).Children().at(0).Arguments().size() > 0);
			BOOST_CHECK(root.at(1).Children().at(1).Name() == _T("場所を指定して解凍"));
			BOOST_CHECK(root.at(1).Children().at(1).TargetPath() == target);
			BOOST_CHECK(root.at(1).Children().at(1).Arguments().size() > 0);
			BOOST_CHECK(root.at(1).Children().at(2).Name() == _T("デスクトップに解凍"));
			BOOST_CHECK(root.at(1).Children().at(2).TargetPath() == target);
			BOOST_CHECK(root.at(1).Children().at(2).Arguments().size() > 0);
			
			src.Context().IsExtended(true);
			src.Save();
		}
		
		// 保存したテストデータを読み込んでテスト。
		{
			CubeICE::UserSetting setting;
			setting.Load();
			BOOST_CHECK(setting.Context().IsExtended());
			
			CubeICE::string_type target = setting.InstallDirectory() + _T("\\") + CUBEICE_NAME;
			CubeICE::ContextContainer& root = setting.Context().ExtendedContext();
			BOOST_CHECK(root.size() == 2);
			BOOST_CHECK(root.at(0).Name() == _T("圧縮"));
			BOOST_CHECK(root.at(1).Name() == _T("解凍"));
			BOOST_CHECK(root.at(0).Children().size() == 2);
			BOOST_CHECK(root.at(0).Children().at(0).Name() == _T("zip"));
			BOOST_CHECK(root.at(0).Children().at(0).TargetPath() == target);
			BOOST_CHECK(root.at(0).Children().at(0).Arguments().size() > 0);
			BOOST_CHECK(root.at(0).Children().at(1).Name() == _T("7z"));
			BOOST_CHECK(root.at(0).Children().at(1).TargetPath() == target);
			BOOST_CHECK(root.at(0).Children().at(1).Arguments().size() > 0);
			BOOST_CHECK(root.at(1).Children().size() == 3);
			BOOST_CHECK(root.at(1).Children().at(0).Name() == _T("ここに解凍"));
			BOOST_CHECK(root.at(1).Children().at(0).TargetPath() == target);
			BOOST_CHECK(root.at(1).Children().at(0).Arguments().size() > 0);
			BOOST_CHECK(root.at(1).Children().at(1).Name() == _T("場所を指定して解凍"));
			BOOST_CHECK(root.at(1).Children().at(1).TargetPath() == target);
			BOOST_CHECK(root.at(1).Children().at(1).Arguments().size() > 0);
			BOOST_CHECK(root.at(1).Children().at(2).Name() == _T("デスクトップに解凍"));
			BOOST_CHECK(root.at(1).Children().at(2).TargetPath() == target);
			BOOST_CHECK(root.at(1).Children().at(2).Arguments().size() > 0);
		}
	}
	catch (const std::exception& err) {
		BOOST_FAIL(err.what());
	}
}

BOOST_AUTO_TEST_SUITE_END()
