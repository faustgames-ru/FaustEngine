#ifndef CORE_ENVIRONMENT_H
#define CORE_ENVIRONMENT_H

#include "core_classes.h"

namespace core
{
	class Environment
	{
	public:
		const static uint LongTimeScale = 0x100000000; // does 136 years of game time good enough?
		const static uint LongFramesScale = 0x8000000;
		static ulong gameTime;
		static ulong elapsedTime;
		static float elapsedSeconds;
		static ulong getTime(float seconds);
		static float getSeconds(ulong delta);
		static void update(float ellapsedTime);
	};
}

#endif /* CORE_ENVIRONMENT_H */