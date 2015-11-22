#ifndef ZOMBO_RENDER_SPRITE_H
#define ZOMBO_RENDER_SPRITE_H

#include "../zombo_classes.h"

namespace zombo
{
	class ZomboRenderSprite
	{
	public:
		core::Vector3 position;
		ZomboSpriteImage texture;
		float rotation;
		float scale;
	};
}

#endif /* ZOMBO_RENDER_SPRITE_H */