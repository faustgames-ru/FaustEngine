#ifndef ZOMBO_RENDER_SPRITE_H
#define ZOMBO_RENDER_SPRITE_H

#include "../zombo_classes.h"
#include "../common/ZomboTransfom.h"
#include "../content/ZomboAtlas.h"

namespace zombo
{
	class ZomboRenderSprite
	{
	public:
		ZomboContentImage *image;
		ZomboTransfom transform;
	};
}

#endif /* ZOMBO_RENDER_SPRITE_H */