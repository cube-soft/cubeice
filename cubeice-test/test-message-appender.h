// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  test-message-appender.h
 *
 *  Copyright (c) 2011, clown.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#ifndef PSDOTNET_TEST_TEST_MESSAGE_APPENDER_H
#define PSDOTNET_TEST_TEST_MESSAGE_APPENDER_H

#include <psdotnet/config.h>
#include <psdotnet/logger.h>
#include <psdotnet/logger/file-appender.h>
#include <sstream>

#if defined(UNICODE) || defined(_UNICODE)
#include <psdotnet/encoding.h>
#endif

#pragma warning(push)
#pragma warning(disable:4996)

/* ------------------------------------------------------------------------- */
///
/// TestMessageAppender
///
/// <summary>
/// PsdotNet::Logger ライブラリを利用して、Boost.Test のメッセージを出力する
/// ための Appender クラスです。
/// </summary>
///
/* ------------------------------------------------------------------------- */
class TestMessageAppender : public PsdotNet::IAppender {
private:
	typedef PsdotNet::IAppender super;
	
public:
	/* --------------------------------------------------------------------- */
	/// constructor
	/* --------------------------------------------------------------------- */
	TestMessageAppender() : super() {}
	
	/* --------------------------------------------------------------------- */
	/// destructor
	/* --------------------------------------------------------------------- */
	virtual ~TestMessageAppender() {}
	
	/* --------------------------------------------------------------------- */
	/// destructor
	/* --------------------------------------------------------------------- */
	virtual void Set(const PsdotNet::LogInfo& info) {
		info_ = info;
	}
	
	/* --------------------------------------------------------------------- */
	/// operator()
	/* --------------------------------------------------------------------- */
	virtual void operator()(const char_type* format, ...) {
		std::basic_stringstream<char_type> ss;
		ss << PsdotNet::Utility::DateTimeToString(info_) << _T(" ");
		ss << PsdotNet::Utility::LogLevelToString(info_) << _T(" ");
		ss << PsdotNet::Utility::FilenameToString(info_) << _T(":");
		ss << PsdotNet::Utility::LineNumberToString(info_) << _T(" ");
		
		char_type buffer[PSDOTNET_INITIAL_BUFFER_SIZE] = {};
		va_list ap;
		va_start(ap, format);
		int n = _vsntprintf(buffer, PSDOTNET_INITIAL_BUFFER_SIZE, format, ap);
		va_end(ap);
		
		if (n >= 0 && n < PSDOTNET_INITIAL_BUFFER_SIZE) ss << buffer;
		else if (n >= PSDOTNET_INITIAL_BUFFER_SIZE) {
			char_type* retry = new char_type[n + 1];
			va_start(ap, format);
			n = _vsntprintf(retry, n + 1, format, ap);
			va_end(ap);
			if (n >= 0) ss << retry;
			delete [] retry;
		}
		
		BOOST_TEST_MESSAGE(ToAscii(ss.str()).c_str());
	}
	
private:
	/* --------------------------------------------------------------------- */
	/// ToAscii
	/* --------------------------------------------------------------------- */
	static std::basic_string<char> ToAscii(const string_type& src) {
#if defined(UNICODE) || (_UNICODE)
		return PsdotNet::Encoding::Convert<PsdotNet::Encoding::ASCII>(src);
#else
		return src;
#endif
	}
	
private:
	PsdotNet::LogInfo info_;
};

#pragma warning(pop)

#endif // PSDOTNET_TEST_TEST_MESSAGE_APPENDER_H
