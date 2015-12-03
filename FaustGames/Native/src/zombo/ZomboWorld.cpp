#include "ZomboWorld.h"

namespace zombo
{
	void ZomboWorld::update(float ellapsedTime)
	{
		renderSystem.update(ellapsedTime);
	}
}
