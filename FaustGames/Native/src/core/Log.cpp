#include "Log.h"

namespace core
{
	Log Log::Default;

	LogArgs::LogArgs(LogLevel::e l, const char* m)
	{
		level = l;
		message = m;
	}

	void Log::out(LogLevel::e level, const char* message)
	{
		LogArgs e(level, message);
		onLog.invoke(e);
	}
}