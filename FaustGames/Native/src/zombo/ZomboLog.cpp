#include "ZomboLog.h"
#include "common/ZomboGameEnvironment.h"

namespace zombo
{
	ZomboLog ZomboLog::Default;

	void ZomboLog::out(ZomboLogLevel::e level, const std::string& text)
	{
		entries.push_back(ZomboLogEntry());
		entries.back().gameTime = ZomboGameEnvironment::gameTime;
		entries.back().level = level;
		entries.back().text = text;
	}

	void ZomboLog::m(const std::string& text)
	{
		out(ZomboLogLevel::Message, text);
	}

	void ZomboLog::e(const std::string& text)
	{
		out(ZomboLogLevel::Error, text);
	}
}
