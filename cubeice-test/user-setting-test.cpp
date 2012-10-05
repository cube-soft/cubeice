// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  user-setting-test.cpp
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
#include <psdotnet/parameter.h>
#include <psdotnet/io.h>
#include <psdotnet/environment.h>

/* ------------------------------------------------------------------------- */
///
/// UserSettingTest
///
/// <summary>
/// UserSettingTest では、UserSetting クラスが直接管理している設定のみ
/// テストを行います。UserCompressionTest クラス等が管理している設定に
/// ついては、別のテストを設けます。
/// </summary>
///
/* ------------------------------------------------------------------------- */
BOOST_AUTO_TEST_SUITE(UserSettingTest)

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
		CubeICE::UserSetting setting;
		LOG_INFO(_T("InstallDirectory: %s"), setting.InstallDirectory().c_str());
		LOG_INFO(_T("Version: %s"), setting.Version().c_str());
		BOOST_CHECK(setting.Parameters().size() == 7);
		boost::optional<Node&> pt;
		
		pt = setting.FindOptional(_T("Tooltip"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.Tooltip() == true);
		
		pt = setting.FindOptional(_T("TooltipCount"));
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == 5);
		BOOST_CHECK(setting.TooltipCount() == 5);
		
		pt = setting.FindOptional(_T("Explorer"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::String);
		BOOST_CHECK(pt->Value<ValueKind::String>().empty());
		BOOST_CHECK(setting.Explorer().empty());
		
		pt = setting.FindOptional(_T("ErrorReport"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.ErrorReport() == true);
		
		pt = setting.FindOptional(_T("LogLevel"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == PsdotNet::LogLevel::Error);
		BOOST_CHECK(setting.LogLevel() == PsdotNet::LogLevel::Error);
		
		pt = setting.FindOptional(_T("CheckUpdate"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.CheckUpdate() == true);
		
		pt = setting.FindOptional(_T("WarnOnDetail"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.WarnOnDetail() == true);
	}
	catch (const std::exception& err) {
		BOOST_FAIL(err.what());
	}
}

/* ------------------------------------------------------------------------- */
//  TestSaveAndLoad
/* ------------------------------------------------------------------------- */
BOOST_AUTO_TEST_CASE(TestSaveAndLoad) {
	try {
		UserSettingSaver saver;

		CubeICE::UserSetting src;
		src.LogLevel(PsdotNet::LogLevel::Trace);
		BOOST_CHECK(src.LogLevel() == PsdotNet::LogLevel::Trace);
		src.Compression().OutputCondition(CubeICE::OutputKind::Runtime);
		BOOST_CHECK(src.Compression().OutputCondition() == CubeICE::OutputKind::Runtime);
		src.Compression().OutputPath(_T("C:\\CubeICE\\サンプルフォルダ 1"));
		BOOST_CHECK(src.Compression().OutputPath() == _T("C:\\CubeICE\\サンプルフォルダ 1"));
		src.Decompression().CreateFolder(CubeICE::CreateFolderKind::SkipSingleFile);
		BOOST_CHECK(src.Decompression().CreateFolder() == CubeICE::CreateFolderKind::SkipSingleFile);
		src.Filtering().Add(_T("テストフィルター"));
		BOOST_CHECK(src.Filtering().Contains(_T("テストフィルター")));
		src.Context().IsExtended(true);
		BOOST_CHECK(src.Context().IsExtended());
		src.DragDrop().IsExtended(true);
		BOOST_CHECK(src.DragDrop().IsExtended());
		src.Runtime().Compression().Type(_T("7z"));
		BOOST_CHECK(src.Runtime().Compression().Type() == _T("7z"));
		src.Save();

		// レジストリからのロードのテストと XML への保存のテスト
		CubeICE::string_type dir  = _T("TestResult");
		CubeICE::string_type path = _T("TestResult\\test-save-and-load.xml");
		{
			CubeICE::UserSetting setting;
			setting.Load();
			BOOST_CHECK(setting.LogLevel() == PsdotNet::LogLevel::Trace);
			BOOST_CHECK(setting.Compression().OutputCondition() == CubeICE::OutputKind::Runtime);
			BOOST_CHECK(setting.Compression().OutputPath() == _T("C:\\CubeICE\\サンプルフォルダ 1"));
			BOOST_CHECK(setting.Decompression().CreateFolder() == CubeICE::CreateFolderKind::SkipSingleFile);
			BOOST_CHECK(setting.Filtering().Contains(_T("テストフィルター")));
			BOOST_CHECK(setting.Context().IsExtended());
			BOOST_CHECK(setting.DragDrop().IsExtended());
			BOOST_CHECK(setting.Runtime().Compression().Type() == _T("7z"));
			if (!PsdotNet::IO::Directory::Exists(dir)) PsdotNet::IO::Directory::Create(dir);
			else PsdotNet::IO::File::Delete(path);
			setting.Save(path);
			BOOST_CHECK(PsdotNet::IO::File::Exists(path));
		}

		// XML からのロードのテスト
		{
			CubeICE::UserSetting setting;
			setting.Load(path);
			BOOST_CHECK(setting.LogLevel() == PsdotNet::LogLevel::Trace);
			BOOST_CHECK(setting.Compression().OutputCondition() == CubeICE::OutputKind::Runtime);
			BOOST_CHECK(setting.Compression().OutputPath() == _T("C:\\CubeICE\\サンプルフォルダ 1"));
			BOOST_CHECK(setting.Decompression().CreateFolder() == CubeICE::CreateFolderKind::SkipSingleFile);
			BOOST_CHECK(setting.Filtering().Contains(_T("テストフィルター")));
			BOOST_CHECK(setting.Context().IsExtended());
			BOOST_CHECK(setting.DragDrop().IsExtended());
			BOOST_CHECK(setting.Runtime().Compression().Type() == _T("7z"));
		}
	}
	catch (const std::exception& err) {
		BOOST_FAIL(err.what());
	}
}

BOOST_AUTO_TEST_SUITE_END()