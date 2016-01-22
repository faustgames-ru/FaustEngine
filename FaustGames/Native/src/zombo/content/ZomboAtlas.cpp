#include "ZomboAtlas.h"

namespace zombo
{
	void ZomboContentImage::initWithQuad(float w, float h)
	{
		w *= 0.5f;
		h *= 0.5f;
		uint detail = 8;

		ZomboImageVertex vertex;
		for (uint y = 0; y <= detail; y++)
		{
			float v = static_cast<float>(y) / static_cast<float>(detail);
			vertex.xy.setY(core::Math::lerp(-h, h, v));
			vertex.v = static_cast<ushort>(v * static_cast<float>(ZomboConstants::t1));
			vertex.normalized.setY(v);
			for (uint x = 0; x <= detail; x++)
			{
				float u = static_cast<float>(x) / static_cast<float>(detail);
				vertex.xy.setX(core::Math::lerp(-w, w, u));
				vertex.u = static_cast<ushort>(u * static_cast<float>(ZomboConstants::t1));;
				vertex.normalized.setX(u);
				vertices.push_back(vertex);
			}
		}
		for (uint y = 0; y < detail; y++)
		{			
			for (uint x = 0; x < detail; x++)
			{
				ushort lt = y * (detail + 1) + x;
				ushort rt = y * (detail + 1) + x + 1;
				ushort lb = (y + 1) * (detail + 1) + x;
				ushort rb = (y + 1) * (detail + 1) + x + 1;
				indices.push_back(lt);
				indices.push_back(rt);
				indices.push_back(rb);
				indices.push_back(lt);
				indices.push_back(rb);
				indices.push_back(lb);
			}
		}

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
