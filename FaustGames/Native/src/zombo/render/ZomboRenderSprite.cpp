#include "ZomboRenderSprite.h"

namespace zombo
{
	ZomboRenderSprite* ZomboRenderSprite::create()
	{
		return new ZomboRenderSprite();
	}

	ZomboRenderSprite::ZomboRenderSprite(): image(nullptr)
	{
	}

	ZomboRenderSprite::~ZomboRenderSprite()
	{
	}

	void ZomboRenderSprite::dispose() const
	{
		delete this;
	}
}
