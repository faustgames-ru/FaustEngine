#ifndef ZOMBO_WORLD_H
#define ZOMBO_WORLD_H

#include "zombo_classes.h"
#include "ZomboEntity.h"
#include "render/ZomboRenderSystem.h"
#include "content/ZomboContentSystem.h"

namespace zombo
{
	class ZomboWorld
	{
	public:
		ZomboRenderSystem renderSystem;
		ZomboRenderSystem animationSystem;
		ZomboContentSystem contentSystem;
		void update(float ellapsedTime);
	private:
		std::vector<ZomboEntity *> _entities;
		std::vector<ZomboEntity *> _workingSet;
	};
}

#endif /* ZOMBO_WORLD_H */