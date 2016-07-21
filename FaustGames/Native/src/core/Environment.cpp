#include "Environment.h"
#include "Math.h"

namespace core
{
	ulong Environment::gameTime(0);
	ulong Environment::elapsedTime(0);
	float Environment::elapsedSeconds(0.0f);


	ulong Environment::getTime(float seconds)
	{
		return static_cast<ulong>(core::Math::round(seconds * LongTimeScale));
	}

	float Environment::getSeconds(ulong delta)
	{
		return static_cast<float>(delta) / static_cast<float>(LongTimeScale);
	}

	void Environment::update(float ellapsedTime)
	{
		elapsedSeconds = ellapsedTime;
		Environment::elapsedTime = static_cast<ulong>(core::Math::round(static_cast<float>(elapsedSeconds) * LongTimeScale));
		if (gameTime > 2147483648)
		{
			// 68 years of game time elapsed. Need warning user about
		}
		gameTime += Environment::elapsedTime;
	}
}
