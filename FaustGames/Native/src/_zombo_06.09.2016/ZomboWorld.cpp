#include "ZomboWorld.h"

namespace zombo
{
	void ZomboWorld::update(float ellapsedTime)
	{
		contentSystem.update();
		animationSystem.update(ellapsedTime);
		renderSystem.update(ellapsedTime);
	}
}
