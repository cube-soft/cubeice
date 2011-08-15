// -*- coding: shift-jis -*-
/* ------------------------------------------------------------------------- */
/*
 *  psdotnet/logger.h
 *
 *  Copyright (c) 2011, clown.
 *
 *  Distributed under the Boost Software License,
 *  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */
/* ------------------------------------------------------------------------- */
#ifndef PSDOTNET_LOGGER_H
#define PSDOTNET_LOGGER_H

#include <psdotnet/config.h>
#include <windows.h>
#include <tchar.h>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <psdotnet/registry-key.h>
#include <psdotnet/typesafe-enum.h>

namespace PsdotNet {
	/* --------------------------------------------------------------------- */
	//  LogLevel
	/* --------------------------------------------------------------------- */
	namespace Detail {
		struct LogLevel_Def {
			enum type {
				Quiet                   = -1,
				Trace                   = 10,
				Debug                   = 20,
				Info                    = 30,
				Warn                    = 40,
				Error                   = 50,
				Fatal                   = 60
			};
		};
	}
	typedef TypesafeEnum<Detail::LogLevel_Def> LogLevel;
	
	namespace Detail {
		inline PsdotNet::LogLevel ToLogLevel(int level) {
			switch (level) {
				case PsdotNet::LogLevel::Trace: return PsdotNet::LogLevel::Trace;
				case PsdotNet::LogLevel::Debug: return PsdotNet::LogLevel::Debug;
				case PsdotNet::LogLevel::Info:  return PsdotNet::LogLevel::Info;
				case PsdotNet::LogLevel::Warn:  return PsdotNet::LogLevel::Warn;
				case PsdotNet::LogLevel::Error: return PsdotNet::LogLevel::Error;
				case PsdotNet::LogLevel::Fatal: return PsdotNet::LogLevel::Fatal;
				default: break;
			}
			return PsdotNet::LogLevel::Quiet;
		}
	}
	
	/* --------------------------------------------------------------------- */
	//  IAppender
	/* --------------------------------------------------------------------- */
	class LogInfo;
	class IAppender {
	public:
		typedef std::size_t size_type;
		typedef TCHAR char_type;
		typedef std::basic_string<TCHAR> string_type;
		
		IAppender() {}
		virtual ~IAppender() {}
		
		virtual void Set(const LogInfo& info) = 0;
		virtual void operator()(const char_type* format, ...) = 0;
		virtual bool Reset() { return true; }
		virtual bool Finish() { return true; }
	};
	
	/* --------------------------------------------------------------------- */
	//  NullAppender
	/* --------------------------------------------------------------------- */
	class NullAppender : public virtual IAppender {
	public:
		NullAppender() : IAppender() {}
		virtual ~NullAppender() {}
		virtual void Set(const LogInfo& info) {}
		virtual void operator()(const char_type* format, ...) {}
	};
	
	/* --------------------------------------------------------------------- */
	//  Logger
	/* --------------------------------------------------------------------- */
	class Logger : boost::noncopyable {
	private:
		typedef boost::shared_ptr<IAppender> appender_ptr;
	public:
		/* ----------------------------------------------------------------- */
		//  Configure
		/* ----------------------------------------------------------------- */
		template <class AppenderT>
		static void Configure(const AppenderT& cp, const LogLevel& lv = LogLevel::Error) {
			Logger::GetUserAppender() = appender_ptr(new AppenderT(cp));
			Logger::GetLevel() = lv;
		}
		
		/* ----------------------------------------------------------------- */
		//  GetAppender
		/* ----------------------------------------------------------------- */
		static IAppender& GetAppender(const LogLevel& lv) {
			appender_ptr dest = Logger::IsValid(lv) ? Logger::GetUserAppender() : Logger::GetNullAppender();
			return *dest;
		}
		
	private:
		/* ----------------------------------------------------------------- */
		//  GetUserAppender
		/* ----------------------------------------------------------------- */
		static appender_ptr& GetUserAppender() {
			static appender_ptr out_;
			return out_;
		}
		
		/* ----------------------------------------------------------------- */
		//  GetNullAppender
		/* ----------------------------------------------------------------- */
		static appender_ptr& GetNullAppender() {
			static appender_ptr null_(new NullAppender());
			return null_;
		}
		
		/* ----------------------------------------------------------------- */
		//  GetLevel
		/* ----------------------------------------------------------------- */
		static LogLevel& GetLevel() {
			static LogLevel level_ = LogLevel::Error;
			return level_;
		}
		
		/* ----------------------------------------------------------------- */
		//  IsValid
		/* ----------------------------------------------------------------- */
		static bool IsValid(const LogLevel& lv) {
			return Logger::GetUserAppender() && lv >= Logger::GetLevel();
		}
	};
	
	/* --------------------------------------------------------------------- */
	//  LogInfo
	/* --------------------------------------------------------------------- */
	class LogInfo {
	public:
		typedef std::size_t size_type;
		typedef TCHAR char_type;
		typedef std::basic_string<TCHAR> string_type;
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		LogInfo() :
			level_(LogLevel::Error), date_(_T(__DATE__)), time_(_T(__TIME__)), file_(_T(__FILE__)), line_(__LINE__) {}
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		LogInfo(const LogLevel& lv, const string_type& d, const string_type& t, const string_type& f, const size_type& l) :
			level_(lv), date_(d), time_(t), file_(f), line_(l) {}
		
		/* ----------------------------------------------------------------- */
		//  constructor
		/* ----------------------------------------------------------------- */
		LogInfo(const LogInfo& cp) :
			level_(cp.level_), date_(cp.date_), time_(cp.time_), file_(cp.file_), line_(cp.line_) {}
		
		/* ----------------------------------------------------------------- */
		//  destructor
		/* ----------------------------------------------------------------- */
		~LogInfo() {}
		
		/* ----------------------------------------------------------------- */
		//  operator=
		/* ----------------------------------------------------------------- */
		LogInfo& operator=(const LogInfo& cp) {
			level_ = cp.level_;
			date_  = cp.date_;
			time_  = cp.time_;
			file_  = cp.file_;
			line_  = cp.line_;
			return *this;
		}
		
		/* ----------------------------------------------------------------- */
		//  SetToAppender
		/* ----------------------------------------------------------------- */
		IAppender& SetToAppender() const {
			IAppender& instance = PsdotNet::Logger::GetAppender(level_);
			instance.Set(*this);
			return instance;
		}
		
		/* ----------------------------------------------------------------- */
		//  access methods
		/* ----------------------------------------------------------------- */
		const LogLevel& Level() const { return level_; }
		const string_type& Date() const { return date_; }
		const string_type& Time() const { return time_; }
		const string_type& File() const { return file_; }
		const size_type& Line() const { return line_; }
		
	private:
		LogLevel level_;
		string_type date_;
		string_type time_;
		string_type file_;
		size_type line_;
	};
	
} // namespace PsdotNet

#ifndef LOG_TRACE
#ifdef PSDOTNET_INVALIDATE_LOG_MACRO
#define LOG_TRACE(x, ...)
#else
#define LOG_TRACE PsdotNet::LogInfo(PsdotNet::LogLevel::Trace, _T(__DATE__), _T(__TIME__), _T(__FILE__), __LINE__).SetToAppender()
#endif // PSDOTNET_INVALIDATE_LOG_MACRO
#else
#error LOG_TRACE is already defined.
#endif // LOG_TRACE

#ifndef LOG_DEBUG
#ifdef PSDOTNET_INVALIDATE_LOG_MACRO
#define LOG_DEBUG(x, ...)
#else
#define LOG_DEBUG PsdotNet::LogInfo(PsdotNet::LogLevel::Debug, _T(__DATE__), _T(__TIME__), _T(__FILE__), __LINE__).SetToAppender()
#endif // PSDOTNET_INVALIDATE_LOG_MACRO
#else
#error LOG_DEBUG is already defined.
#endif // LOG_DEBUG

#ifndef LOG_INFO
#ifdef PSDOTNET_INVALIDATE_LOG_MACRO
#define LOG_INFO(x, ...)
#else
#define LOG_INFO PsdotNet::LogInfo(PsdotNet::LogLevel::Info, _T(__DATE__), _T(__TIME__), _T(__FILE__), __LINE__).SetToAppender()
#endif // PSDOTNET_INVALIDATE_LOG_MACRO
#else
#error LOG_INFO is already defined.
#endif // LOG_INFO

#ifndef LOG_WARN
#ifdef PSDOTNET_INVALIDATE_LOG_MACRO
#define LOG_WARN(x, ...)
#else
#define LOG_WARN PsdotNet::LogInfo(PsdotNet::LogLevel::Warn, _T(__DATE__), _T(__TIME__), _T(__FILE__), __LINE__).SetToAppender()
#endif // PSDOTNET_INVALIDATE_LOG_MACRO
#else
#error LOG_WARN is already defined.
#endif // LOG_WARN

#ifndef LOG_ERROR
#ifdef PSDOTNET_INVALIDATE_LOG_MACRO
#define LOG_ERROR(x, ...)
#else
#define LOG_ERROR PsdotNet::LogInfo(PsdotNet::LogLevel::Error, _T(__DATE__), _T(__TIME__), _T(__FILE__), __LINE__).SetToAppender()
#endif // PSDOTNET_INVALIDATE_LOG_MACRO
#else
#error LOG_ERROR is already defined.
#endif // LOG_ERROR

#ifndef LOG_FATAL
#ifdef PSDOTNET_INVALIDATE_LOG_MACRO
#define LOG_FATAL(x, ...)
#else
#define LOG_FATAL PsdotNet::LogInfo(PsdotNet::LogLevel::Fatal, _T(__DATE__), _T(__TIME__), _T(__FILE__), __LINE__).SetToAppender()
#endif // PSDOTNET_INVALIDATE_LOG_MACRO
#else
#error LOG_FATAL is already defined.
#endif // LOG_FATAL

#endif // PSDOTNET_LOGGER_H
