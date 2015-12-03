#include "ZomboGameEnvironment.h"

namespace zombo
{
	ulong ZomboGameEnvironment::gameTime(0);
	float ZomboGameEnvironment::ellapsedTime(0.0f);


	void ZomboGameEnvironment::update(float ellapsedTime)
	{
		ellapsedTime = ellapsedTime;
		gameTime += static_cast<ulong>(core::Math::round(ellapsedTime * 0xffffffff));
	}
}
