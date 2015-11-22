#ifndef ZOMBO_RENDER_SYSTEM_H
#define ZOMBO_RENDER_SYSTEM_H

#include "../zombo_classes.h"
#include "ZomboRenderSprite.h"

namespace zombo
{
	class ZomboRenderSystem
	{
	public:
		inline void renderSprite(ZomboRenderSprite* sprite);
	};
}

#endif /* ZOMBO_RENDER_SYSTEM_H */