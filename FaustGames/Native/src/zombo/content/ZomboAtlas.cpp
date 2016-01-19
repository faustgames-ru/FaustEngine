#include "ZomboAtlas.h"

namespace zombo
{
	void ZomboContentImage::initWithQuad(float w, float h)
	{
		w *= 0.5f;
		h *= 0.5f;
		ushort t0 = 1;
		ushort t1 = 65535;
		vertices.resize(4);
		vertices[0].xy = core::Vector2(-w, -h);
		vertices[0].u = t0;
		vertices[0].v = t0;

		vertices[1].xy = core::Vector2(-w,  h);
		vertices[1].u = t0;
		vertices[1].v = t1;

		vertices[2].xy = core::Vector2( w,  h);
		vertices[2].u = t1;
		vertices[2].v = t1;

		vertices[3].xy = core::Vector2( w, -h);
		vertices[3].u = t1;
		vertices[3].v = t0;

		indices.resize(6);
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 3;
		bounds.Min = core::Vector2(-w, -h);
		bounds.Max = core::Vector2(w, h);
	}

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
