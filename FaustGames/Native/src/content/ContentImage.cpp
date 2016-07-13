#include "ContentImage.h"
#include "ContentStatus.h"
#include "ContentBlock.h"

namespace content
{
	void ContentImage::initWithQuad(float w, float h, uint detail)
	{
		w *= 0.5f;
		h *= 0.5f;

		ImageVertex vertex;
		for (uint y = 0; y <= detail; y++)
		{
			float v = static_cast<float>(y) / static_cast<float>(detail);
			vertex.xy.setY(core::Math::lerp(-h, h, v));
			vertex.v = static_cast<ushort>((1.0f - v) * static_cast<float>(ContentConstants::t1));
			for (uint x = 0; x <= detail; x++)
			{
				float u = static_cast<float>(x) / static_cast<float>(detail);
				vertex.xy.setX(core::Math::lerp(-w, w, u));
				vertex.u = static_cast<ushort>(u * static_cast<float>(ContentConstants::t1));;
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

	ContentImage* ContentImage::create()
	{
		return new ContentImage();
	}

	void ContentImage::internalLoad(const LoadArgs& e)
	{
		int detail = 1;
		bool mipmaps = false;
		resources::ContentManager content = resources::ContentManager::Default;
		if (!resources::ContentProvider::existContent(e.fullPath.c_str()))
		{
			// todo: handle error
			//ZomboLog::Default.e("Unable to find resource: " + fileName);
		}
		else
		{
			graphics::Image2dData* imageData = content.loadUnregisteredTexture(e.fullPath.c_str());
			if (imageData != nullptr)
			{
				// todo: configure loading;
				texture = new graphics::TextureImage2d(mipmaps, true);
				texture->create();
				texture->setData(imageData);
				name = e.path;
				initWithQuad(
					static_cast<float>(imageData->Width) * e.owner->ContentScale,
					static_cast<float>(imageData->Height) * e.owner->ContentScale,
					detail);
			}
			else
			{
				// todo: handle error
				//ZomboLog::Default.e("Unable to load png: " + fileName);
			}
		}
	}

	void ContentImage::dispose()
	{
		delete this;
	}

	void ContentImage::load(const LoadArgs& e)
	{
		internalLoad(e);
		e.status->invokeCallback();
	}

	ContentImage::ContentImage(): texture(nullptr)
	{
	}

	ContentImage::~ContentImage()
	{		
	}
}