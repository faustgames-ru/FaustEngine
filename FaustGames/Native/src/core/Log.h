#ifndef CORE_LOG_H
#define CORE_LOG_H

#include "core_classes.h"
#include "Delegate.h"

namespace core
{
	struct LogLevel
	{
		enum e
		{
			None,
			Message,
			Warning,
			Error,
			Exception
		};
	};
	
	struct LogArgs
	{
		LogLevel::e level;
		const char* message;
		LogArgs(LogLevel::e level, const char* message);
	};
	
	class Log
	{
	public:
		static Log Default;
		Delegate<LogArgs> onLog;

		void out(LogLevel::e level, const char* message);
		void message(const char* message);
		void warning(const char* message);
		void error(const char* message);
		void exception(const char* message);
	};
}

#endif /*CORE_LOG_H*/