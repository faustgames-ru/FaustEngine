#ifndef ZOMBO_RENDER_SYSTEM_H
#define ZOMBO_RENDER_SYSTEM_H

#include "../zombo_classes.h"
#include "ZomboRenderSprite.h"

namespace zombo
{
	class ZomboRenderSystem
	{
	public:
		void renderSprite(ZomboTransfom* tramsform,  ZomboRenderSprite* sprite);
		void update(float ellapsedTime);
	};
}

#endif /* ZOMBO_RENDER_SYSTEM_H */