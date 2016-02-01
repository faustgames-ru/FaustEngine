#include "ZomboGameEnvironment.h"

namespace zombo
{
	ulong ZomboGameEnvironment::gameTime(0);
	ulong ZomboGameEnvironment::ellapsedTime(0);
	float ZomboGameEnvironment::ellapsedSeconds(0.0f);


	ulong ZomboGameEnvironment::getTime(float seconds)
	{
		return static_cast<ulong>(core::Math::round(seconds * ZomboConstants::LongTimeScale));
	}

	float ZomboGameEnvironment::getSeconds(ulong delta)
	{
		return static_cast<float>(delta) / static_cast<float>(ZomboConstants::LongTimeScale);
	}

	void ZomboGameEnvironment::update(float ellapsedTime)
	{
		ellapsedSeconds = ellapsedTime;
		ZomboGameEnvironment::ellapsedTime = static_cast<ulong>(core::Math::round(static_cast<float>(ellapsedSeconds) * ZomboConstants::LongTimeScale));
		gameTime += ZomboGameEnvironment::ellapsedTime;
	}
}
