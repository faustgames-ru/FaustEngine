#include "ZomboAtlas.h"

namespace zombo
{
	ZomboContentImage* ZomboContentImage::create()
	{
		return new ZomboContentImage();
	}

	void ZomboContentImage::dispose() const
	{
		delete this;
	}

	ZomboContentAtlasPage* ZomboContentAtlasPage::create()
	{
		return new ZomboContentAtlasPage();
	}

	void ZomboContentAtlasPage::dispose() const
	{
		delete this;
	}

	ZomboContentAtlasPage::ZomboContentAtlasPage()
	{
	}

	ZomboContentAtlasPage::~ZomboContentAtlasPage()
	{
		for (uint i = 0; i < images.size(); i++)
		{
			images[i]->dispose();
		}
	}

	ZomboContentAtlas* ZomboContentAtlas::create()
	{
		return new ZomboContentAtlas();
	}

	void ZomboContentAtlas::dispose() const
	{
		delete this;
	}

	ZomboContentFrameAnimation* ZomboContentFrameAnimation::create()
	{
		return new ZomboContentFrameAnimation();
	}

	void ZomboContentFrameAnimation::dispose() const
	{
		delete this;
	}
}
