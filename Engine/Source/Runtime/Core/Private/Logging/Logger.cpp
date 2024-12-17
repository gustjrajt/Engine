#include "Logging/Logger.h"
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/locale.hpp>

static boost::log::sources::wseverity_logger<ELogVerbosity> Logger;
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (std::basic_ostream< CharT, TraitsT >& strm, ELogVerbosity lvl)
{
	static const char* const str[] =
	{
		"Fatal",
		"Error",
		"Warning",
		"Log",
	};
	if (static_cast<std::size_t>(lvl) < (sizeof(str) / sizeof(*str)))
		strm << str[lvl];
	else
		strm << static_cast<int>(lvl);
	return strm;
}

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

FLogger* FLogger::Get(const bool bDestroy)
{
	static unique_ptr<FLogger> Instance = make_unique<FLogger>();
	if (bDestroy)
	{
		Instance.reset();
		return nullptr;
	}
	return Instance.get();
}

BOOST_LOG_ATTRIBUTE_KEYWORD(Severity, "Severity", ELogVerbosity)
BOOST_LOG_ATTRIBUTE_KEYWORD(Timestamp, "TimeStamp", boost::posix_time::ptime)

const TCHAR* GetLogName(ELogVerbosity InLogVerbosity)
{
	switch (InLogVerbosity)
	{
	case ELogVerbosity::Fatal:  return TEXT("Fatal");
	case ELogVerbosity::Error:  return TEXT("Error");
	case ELogVerbosity::Warning:	 return TEXT("Warning");
	case ELogVerbosity::Log: return TEXT("Log");
	}
	_ASSERT(false);
	return nullptr;
}

FLogger::FLogger()
{
	// FileSink
	{
		boost::shared_ptr<sinks::text_file_backend> Backend = boost::make_shared<sinks::text_file_backend>(
			keywords::auto_flush = true,
			keywords::open_mode = (std::ios::out | std::ios::app),
			keywords::max_files = 256,	// 최대 저장 파일 수
			keywords::rotation_size = 10 * 1024 * 1024,
			keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
			keywords::file_name = FString(TEXT("Logs\\Log")) + TEXT("_%Y-%m-%d_%H-%M-%S.%N.log")
		);
		
		// .log파일에 써지는 문자열 포맷 세팅
		boost::shared_ptr<sinks::asynchronous_sink< sinks::text_file_backend>> FileSink(new sinks::asynchronous_sink< sinks::text_file_backend>(Backend));
		FileSink->set_formatter(
			expr::stream
			<< expr::attr<unsigned int>("LineID") << ": "
			<< expr::format_date_time(Timestamp, "%Y-%m-%d %H:%M:%S")
			<< " [" 
			<< Severity			//위의 선언된 << Operator 작동 Severity값이 문자열로 치환되어서 적용
			<< "] " << expr::message
		);

		// The sink will perform character code conversion as needed, according to the locale set with imbue()
		FileSink->imbue(boost::locale::generator()("en_US.UTF-8"));
		logging::core::get()->add_sink(FileSink);
	}

	{
		// Console에 표시되는 부분
		boost::shared_ptr<sinks::text_ostream_backend> ConsoleBackend = boost::make_shared<sinks::text_ostream_backend>();
		ConsoleBackend->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));

		boost::shared_ptr<sinks::asynchronous_sink<sinks::text_ostream_backend>> ConsoleSink(new sinks::asynchronous_sink<sinks::text_ostream_backend>(ConsoleBackend));

		// Console 에 띄우는 문자열 포맷세팅
		ConsoleSink->set_formatter(
			expr::stream
			<< expr::if_(expr::attr<ELogVerbosity>("Severity") == ELogVerbosity::Log)[
				expr::stream << "\033[1;37m" // White
			]
			<< expr::if_(expr::attr<ELogVerbosity>("Severity") == ELogVerbosity::Fatal)[
				expr::stream << "\033[1;31m" // Red
			]
			<< expr::if_(expr::attr<ELogVerbosity>("Severity") == ELogVerbosity::Warning)[
				expr::stream << "\033[1;33m" // Yellow
			]
			<< expr::if_(expr::attr<ELogVerbosity>("Severity") == ELogVerbosity::Error)[
				expr::stream << "\033[1;35m" // Magenta
			]
			<< expr::attr<unsigned int>("LineID") << ": "
			<< expr::format_date_time(Timestamp, "%Y-%m-%d %H:%M:%S")
			<< " [" 
			<< Severity			//위의 선언된 << Operator 작동 Severity값이 문자열로 치환되어서 적용
			<< "] " << expr::message
			<< "\033[37m" // Defalut
		);

		// The sink will perform character code conversion as needed, according to the locale set with imbue()
		ConsoleSink->imbue(boost::locale::generator()("en_US.UTF-8"));
		logging::core::get()->add_sink(ConsoleSink);
	}
	logging::add_common_attributes();

	LogF(ELogVerbosity::Log, TEXT("Sample"));
}

void FLogger::LogF(ELogVerbosity InLogVerbosity, FStringView InMessage)
{
	switch (InLogVerbosity)
	{
	case ELogVerbosity::Fatal:
		BOOST_LOG_SEV(Logger, ELogVerbosity::Fatal) << InMessage;
		break;
	case ELogVerbosity::Error:
		BOOST_LOG_SEV(Logger, ELogVerbosity::Error) << InMessage;
		break;
	case ELogVerbosity::Warning:
		BOOST_LOG_SEV(Logger, ELogVerbosity::Warning) << InMessage;
		break;
	case ELogVerbosity::Log:
		BOOST_LOG_SEV(Logger, ELogVerbosity::Log) << InMessage;
		break;
	default:
		_ASSERT(false);
		break;
	}

	if (IsDebuggerPresent())
	{
		const FString NewLog = std::format(TEXT("[{}] {}\n"), GetLogName(InLogVerbosity), InMessage);
		OutputDebugStringW(NewLog.data());
	}
}
