#include "ZomboRenderSprite.h"

namespace zombo
{
	ZomboComponentTypes::e ZomboRenderSprite::ComponentType(ZomboComponentTypes::RenderSprite);
	
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
