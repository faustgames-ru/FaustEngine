#ifndef ZOMBO_LOG_H
#define ZOMBO_LOG_H

#include "zombo_classes.h"
#include "../../CommonSources/include/zombo.h"

namespace zombo
{
	struct ZomboLogLevel
	{
		enum e
		{
			Message,
			Warning,
			Error
		};
	};

	struct ZomboLogEntry
	{
		ulong gameTime;
		ZomboLogLevel::e level;
		std::string text;
	};

	class ZomboLog
	{
	public:
		static ZomboLog Default;
		std::vector<ZomboLogEntry> entries;
		void out(ZomboLogLevel::e level, const std::string &text);
		void m(const std::string &text);
		void e(const std::string &text);
	};
}

#endif /* ZOMBO_LOG_H */