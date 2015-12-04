#include "ZomboGameEnvironment.h"

namespace zombo
{
	ulong ZomboGameEnvironment::gameTime(0);
	ulong ZomboGameEnvironment::ellapsedTime(0);
	float ZomboGameEnvironment::ellapsedSeconds(0.0f);
	

	void ZomboGameEnvironment::update(float ellapsedTime)
	{
		ellapsedSeconds = ellapsedTime;
		ellapsedTime = static_cast<ulong>(core::Math::round(ellapsedTime * ZomboConstants::LongTimeScale));
		gameTime += ellapsedTime;
	}
}
