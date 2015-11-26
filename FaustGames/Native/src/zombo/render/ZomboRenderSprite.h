#ifndef ZOMBO_RENDER_SPRITE_H
#define ZOMBO_RENDER_SPRITE_H

#include "../zombo_classes.h"
#include "../common/ZomboTransfom.h"

namespace zombo
{
	class ZomboRenderSprite
	{
	public:
		ZomboSpriteImage texture;
		ZomboTransfom transform;
	};
}

#endif /* ZOMBO_RENDER_SPRITE_H */