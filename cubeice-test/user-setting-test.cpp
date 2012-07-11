// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  user-setting-test.cpp
 *
 *  Copyright (c) 2011, clown.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#include <boost/test/unit_test.hpp>
#include <cubeice/user-setting.h>
#include "cubeice-test.h"
#include <psdotnet/parameter.h>
#include <psdotnet/io.h>
#include <psdotnet/environment.h>

BOOST_AUTO_TEST_SUITE(UserSettingTest)

typedef CubeICE::UserSettingBase::ValueKind ValueKind;
typedef CubeICE::UserSettingBase::Node Node;
typedef CubeICE::UserSettingBase::NodeSet NodeSet;

/* ------------------------------------------------------------------------- */
/*!
 *  TestValue
 *
 *  各種ユーザ設定の初期設定値、および値の変更をテストします。
 */
/* ------------------------------------------------------------------------- */
BOOST_AUTO_TEST_CASE(TestValue) {
	try {
		CubeICE::UserSetting setting;
		LOG_INFO(_T("InstallDirectory: %s"), setting.InstallDirectory().c_str());
		LOG_INFO(_T("Version: %s"), setting.Version().c_str());

		// CubeICE の一般（その他）の設定
		const NodeSet& general = setting.Parameters();

		boost::optional<const Node&> pt = PsdotNet::Parameter::FindOptional(general, _T("Tooltip"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.Tooltip() == true);
		setting.Tooltip(false);
		BOOST_CHECK(setting.Tooltip() == false);

		pt = PsdotNet::Parameter::FindOptional(general, _T("TooltipCount"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == 5);
		BOOST_CHECK(setting.TooltipCount() == 5);
		setting.TooltipCount(10);
		BOOST_CHECK(setting.TooltipCount() == 10);
		
		pt = PsdotNet::Parameter::FindOptional(general, _T("Explorer"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::String);
		BOOST_CHECK(pt->Value<ValueKind::String>().empty());
		BOOST_CHECK(setting.Explorer().empty());
		setting.Explorer(_T("C:\\Program Files\\MyExplorer\\explorer.exe"));
		BOOST_CHECK(setting.Explorer() == _T("C:\\Program Files\\MyExplorer\\explorer.exe"));

		pt = PsdotNet::Parameter::FindOptional(general, _T("ErrorReport"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.ErrorReport() == true);
		setting.ErrorReport(false);
		BOOST_CHECK(setting.ErrorReport() == false);

		pt = PsdotNet::Parameter::FindOptional(general, _T("LogLevel"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == PsdotNet::LogLevel::Error);
		BOOST_CHECK(setting.LogLevel() == PsdotNet::LogLevel::Error);
		setting.LogLevel(PsdotNet::LogLevel::Info);
		BOOST_CHECK(setting.LogLevel() == PsdotNet::LogLevel::Info);
		
		// CubeICE の圧縮に関連する設定
		const NodeSet& compression = setting.Compression().Parameters();

		pt = PsdotNet::Parameter::FindOptional(compression, _T("OutputCondition"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == CubeICE::OutputKind::Specific);
		BOOST_CHECK(setting.Compression().OutputCondition() == CubeICE::OutputKind::Specific);
		setting.Compression().OutputCondition(CubeICE::OutputKind::Runtime);
		BOOST_CHECK(setting.Compression().OutputCondition() == CubeICE::OutputKind::Runtime);

		pt = PsdotNet::Parameter::FindOptional(compression, _T("OutputPath"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::String);
		BOOST_CHECK(pt->Value<ValueKind::String>().empty());
		BOOST_CHECK(setting.Compression().OutputPath().empty());
		setting.Compression().OutputPath(_T("c:\\foo\\bar"));
		BOOST_CHECK(setting.Compression().OutputPath() == _T("c:\\foo\\bar"));

		pt = PsdotNet::Parameter::FindOptional(compression, _T("DefaultType"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::String);
		BOOST_CHECK(pt->Value<ValueKind::String>() == _T("zip"));
		BOOST_CHECK(setting.Compression().DefaultType() == _T("zip"));
		setting.Compression().DefaultType(_T("7z"));
		BOOST_CHECK(setting.Compression().DefaultType() == _T("7z"));

		pt = PsdotNet::Parameter::FindOptional(compression, _T("DefaultEnablePassword"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.Compression().DefaultEnablePassword() == false);
		setting.Compression().DefaultEnablePassword(true);
		BOOST_CHECK(setting.Compression().DefaultEnablePassword() == true);

		pt = PsdotNet::Parameter::FindOptional(compression, _T("Overwrite"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == CubeICE::OverwriteKind::Confirm);
		BOOST_CHECK(setting.Compression().Overwrite() == CubeICE::OverwriteKind::Confirm);
		setting.Compression().Overwrite(CubeICE::OverwriteKind::ConfirmOlder);
		BOOST_CHECK(setting.Compression().Overwrite() == CubeICE::OverwriteKind::ConfirmOlder);

		pt = PsdotNet::Parameter::FindOptional(compression, _T("Filtering"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.Compression().Filtering() == false);
		setting.Compression().Filtering(true);
		BOOST_CHECK(setting.Compression().Filtering() == true);

		pt = PsdotNet::Parameter::FindOptional(compression, _T("DeleteOnMail"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);

		// CubeICE の解凍に関連する設定
		const NodeSet& decompression = setting.Decompression().Parameters();

		pt = PsdotNet::Parameter::FindOptional(decompression, _T("OutputCondition"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == CubeICE::OutputKind::Specific);
		BOOST_CHECK(setting.Decompression().OutputCondition() == CubeICE::OutputKind::Specific);
		setting.Decompression().OutputCondition(CubeICE::OutputKind::SameAsSource);
		BOOST_CHECK(setting.Decompression().OutputCondition() == CubeICE::OutputKind::SameAsSource);

		pt = PsdotNet::Parameter::FindOptional(decompression, _T("OutputPath"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::String);
		BOOST_CHECK(pt->Value<ValueKind::String>().empty());
		BOOST_CHECK(setting.Decompression().OutputPath().empty());
		setting.Decompression().OutputPath(_T("C:\\foo\\bar\\サンプル フォルダ"));
		BOOST_CHECK(setting.Decompression().OutputPath() == _T("C:\\foo\\bar\\サンプル フォルダ"));

		pt = PsdotNet::Parameter::FindOptional(decompression, _T("SourceEncoding"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == CubeICE::EncodingKind::Auto);
		BOOST_CHECK(setting.Decompression().SourceEncoding() == CubeICE::EncodingKind::Auto);
		setting.Decompression().SourceEncoding(CubeICE::EncodingKind::UTF8);
		BOOST_CHECK(setting.Decompression().SourceEncoding() == CubeICE::EncodingKind::UTF8);

		pt = PsdotNet::Parameter::FindOptional(decompression, _T("Overwrite"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == CubeICE::OverwriteKind::Confirm);
		BOOST_CHECK(setting.Decompression().Overwrite() == CubeICE::OverwriteKind::Confirm);
		setting.Decompression().Overwrite(CubeICE::OverwriteKind::Force);
		BOOST_CHECK(setting.Decompression().Overwrite() == CubeICE::OverwriteKind::Force);

		pt = PsdotNet::Parameter::FindOptional(decompression, _T("CreateFolder"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Number);
		BOOST_CHECK(pt->Value<ValueKind::Number>() == CubeICE::CreateFolderKind::SkipSingleFolder);
		BOOST_CHECK(setting.Decompression().CreateFolder() == CubeICE::CreateFolderKind::SkipSingleFolder);
		setting.Decompression().CreateFolder(CubeICE::CreateFolderKind::SkipSingleFile);
		BOOST_CHECK(setting.Decompression().CreateFolder() == CubeICE::CreateFolderKind::SkipSingleFile);

		pt = PsdotNet::Parameter::FindOptional(decompression, _T("Filtering"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == true);
		BOOST_CHECK(setting.Decompression().Filtering() == true);
		setting.Decompression().Filtering(false);
		BOOST_CHECK(setting.Decompression().Filtering() == false);

		pt = PsdotNet::Parameter::FindOptional(decompression, _T("DeleteOnExtract"));
		BOOST_CHECK((bool)pt);
		BOOST_CHECK(pt->Kind() == ValueKind::Bool);
		BOOST_CHECK(pt->Value<ValueKind::Bool>() == false);
		BOOST_CHECK(setting.Decompression().DeleteOnExtract() == false);
		setting.Decompression().DeleteOnExtract(true);
		BOOST_CHECK(setting.Decompression().DeleteOnExtract() == true);

		// CubeICE のフィルタリング文字列に関する設定
		CubeICE::UserFilteringSetting& filtering = setting.Filtering();
		BOOST_CHECK(!filtering.Empty());
		BOOST_CHECK(filtering.Count() == 4);
		BOOST_CHECK(filtering.Contains(_T(".DS_Store")));
		BOOST_CHECK(filtering.Contains(_T("Thumbs.db")));
		BOOST_CHECK(filtering.Contains(_T("__MACOSX")));
		BOOST_CHECK(filtering.Contains(_T("desktop.ini")));
		filtering.Add(_T("テストフィルタリング"));
		BOOST_CHECK(filtering.Count() == 5);
		BOOST_CHECK(filtering.Contains(_T("テストフィルタリング")));
		filtering.Delete(_T("Thumbs.db"));
		BOOST_CHECK(filtering.Count() == 4);
		BOOST_CHECK(!filtering.Contains(_T("Thumbs.db")));
		filtering.Clear();
		BOOST_CHECK(filtering.Empty());
		BOOST_CHECK(filtering.Count() == 0);

		// CubeICE のコンテキストメニューに関する設定
		CubeICE::UserContextSetting& context = setting.Context();
		BOOST_CHECK(context.IsExtended() == false);
		context.IsExtended(true);
		BOOST_CHECK(context.IsExtended() == true);
		BOOST_CHECK((context.Builtin() & CubeICE::ContextKind::Compression) != 0);
		BOOST_CHECK((context.Builtin() & CubeICE::ContextKind::CompressionZip) != 0);
		BOOST_CHECK((context.Builtin() & CubeICE::ContextKind::Decompression) != 0);
		BOOST_CHECK((context.Builtin() & CubeICE::ContextKind::Mail) == 0);
		context.Builtin() |= CubeICE::ContextKind::Mail;
		context.Builtin() &= ~CubeICE::ContextKind::CompressionZip;
		BOOST_CHECK((context.Builtin() & CubeICE::ContextKind::Mail) != 0);
		BOOST_CHECK((context.Builtin() & CubeICE::ContextKind::CompressionZip) == 0);

		// CubeICE のドラッグ&ドロップに関する設定
		CubeICE::UserDragDropSetting& dragdrop = setting.DragDrop();
		BOOST_CHECK(dragdrop.IsExtended() == false);
		dragdrop.IsExtended(true);
		BOOST_CHECK(dragdrop.IsExtended() == true);

		// CubeICE ランタイム時の圧縮関する設定
		CubeICE::RuntimeCompressionSetting& cruntime = setting.Runtime().Compression();
		BOOST_CHECK(cruntime.Type() == _T("zip"));
		BOOST_CHECK(cruntime.Level() == CubeICE::CompressionLevel::Normal);
		BOOST_CHECK(cruntime.Method() == _T("Deflate"));
		BOOST_CHECK(!cruntime.EnablePassword());
		BOOST_CHECK(!cruntime.ShowPassword());
		BOOST_CHECK(cruntime.EncodingMethod() == _T("ZipCrypto"));
		BOOST_CHECK(cruntime.Overwrite() == CubeICE::OverwriteKind::Confirm);
		BOOST_CHECK(cruntime.ThreadSize() == 1);
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

/* ------------------------------------------------------------------------- */
//  TestContext
/* ------------------------------------------------------------------------- */
BOOST_AUTO_TEST_CASE(TestContext) {
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

/* ------------------------------------------------------------------------- */
/*!
 * TestAssociation
 */
/* ------------------------------------------------------------------------- */
BOOST_AUTO_TEST_CASE(TestAssociation) {
	try {
		UserSettingSaver saver;

		CubeICE::UserSetting src;
		BOOST_CHECK(src.Association().IsAssociated(_T("zip")) == true);
		BOOST_CHECK(src.Association().IsAssociated(_T("lzh")) == true);
		BOOST_CHECK(src.Association().IsAssociated(_T("rar")) == true);
		BOOST_CHECK(src.Association().IsAssociated(_T("7z")) == true);
		BOOST_CHECK(src.Association().IsAssociated(_T("arj")) == false);
		BOOST_CHECK(src.Association().IsAssociated(_T("bz2")) == true);
		BOOST_CHECK(src.Association().IsAssociated(_T("cab")) == true);
		BOOST_CHECK(src.Association().IsAssociated(_T("chm")) == false);
		BOOST_CHECK(src.Association().IsAssociated(_T("cpio")) == false);
		BOOST_CHECK(src.Association().IsAssociated(_T("deb")) == false);
		BOOST_CHECK(src.Association().IsAssociated(_T("dmg")) == false);
		BOOST_CHECK(src.Association().IsAssociated(_T("gz")) == true);
		BOOST_CHECK(src.Association().IsAssociated(_T("iso")) == false);
		BOOST_CHECK(src.Association().IsAssociated(_T("jar")) == false);
		BOOST_CHECK(src.Association().IsAssociated(_T("rpm")) == false);
		BOOST_CHECK(src.Association().IsAssociated(_T("tar")) == true);
		BOOST_CHECK(src.Association().IsAssociated(_T("tbz")) == true);
		BOOST_CHECK(src.Association().IsAssociated(_T("tgz")) == true);
		BOOST_CHECK(src.Association().IsAssociated(_T("wim")) == false);
		BOOST_CHECK(src.Association().IsAssociated(_T("xar")) == false);
		BOOST_CHECK(src.Association().IsAssociated(_T("xz")) == true);
	}
	catch (const std::exception& err) {
		BOOST_FAIL(err.what());
	}
}

/* ------------------------------------------------------------------------- */
//  TestShortcut
/* ------------------------------------------------------------------------- */
BOOST_AUTO_TEST_CASE(TestShortcut) {
	try {
		UserSettingSaver saver;

		CubeICE::UserSetting setting;
		setting.Load();

		CubeICE::string_type name = _T("CubeICE ショートカットテスト");
		CubeICE::string_type directory = PsdotNet::Environment::GetFolderPath(PsdotNet::Environment::SpecialFolder::DesktopDirectory);
		CubeICE::string_type path = directory + _T("\\") + name + _T(".lnk");
		CubeICE::string_type target = setting.InstallDirectory() + _T("\\") + CUBEICE_NAME;
		CubeICE::string_type icon = setting.InstallDirectory() + _T("\\") + CUBEICE_NAME + _T(",1");
		CubeICE::Shortcut newitem(name, directory, target, _T("/c:zip"), icon);

		setting.Shortcut().Add(newitem);
		setting.Save();
		BOOST_CHECK(PsdotNet::IO::File::Exists(path));

		setting.Shortcut().Delete(name);
		setting.Save();
		BOOST_CHECK(!PsdotNet::IO::File::Exists(path));

		// 削除したショートカット再度登録するテスト
		setting.Shortcut().Add(newitem);
		setting.Save();
		BOOST_CHECK(PsdotNet::IO::File::Exists(path));

		setting.Shortcut().Delete(name);
		setting.Save();
		BOOST_CHECK(!PsdotNet::IO::File::Exists(path));
	}
	catch (const std::exception& err) {
		BOOST_FAIL(err.what());
	}
}

BOOST_AUTO_TEST_SUITE_END()