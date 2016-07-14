#ifndef CORE_ENVIRONMENT_H
#define CORE_ENVIRONMENT_H

#include "core_classes.h"

namespace core
{
	class Environment
	{
	public:
		const static uint LongTimeScale = 0xffffffff; // does 136 years of game time good enough?

		static ulong gameTime;
		static ulong ellapsedTime;
		static float ellapsedSeconds;
		static ulong getTime(float seconds);
		static float getSeconds(ulong delta);
		static void update(float ellapsedTime);
	};
}

#endif /* CORE_ENVIRONMENT_H */