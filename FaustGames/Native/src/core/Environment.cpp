#include "Environment.h"
#include "Math.h"

namespace core
{
	ulong Environment::gameTime(0);
	ulong Environment::ellapsedTime(0);
	float Environment::ellapsedSeconds(0.0f);


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
		ellapsedSeconds = ellapsedTime;
		Environment::ellapsedTime = static_cast<ulong>(core::Math::round(static_cast<float>(ellapsedSeconds) * LongTimeScale));
		gameTime += Environment::ellapsedTime;
	}
}
