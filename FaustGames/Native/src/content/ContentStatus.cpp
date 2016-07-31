#include "ContentStatus.h"
#include "ContentImage.h"
#include "ContentFrameAnimation.h"
#include "ContentBitmapFont.h"
#include "../core/Path.h"
#include "ContentJson.h"
#include "ContentMesh.h"

namespace content
{
	ContentStatus::ContentStatus(ContentBlock *ownerBlock, const char* contentPath): owner(ownerBlock), path(contentPath), type(ContentType::None), _castInstance(nullptr), _instance(nullptr), _callback(nullptr)
	{
		detectType();
		createContentInstance();
	}

	ContentStatus::~ContentStatus()
	{
		if (_instance != nullptr)
			_instance->dispose();
		if (_callback != nullptr)
			_callback->dispose();
	}

	IContent* ContentStatus::asContent() const
	{
		return _instance;
	}

	ContentMesh* ContentStatus::asMesh() const
	{
		return static_cast<ContentMesh*>(_castInstance);
	}

	ContentFrameAnimation* ContentStatus::asFrameAnimation() const
	{
		return static_cast<ContentFrameAnimation*>(_castInstance);
	}

	ContentImage* ContentStatus::asImage() const
	{
		return static_cast<ContentImage*>(_castInstance);
	}

	ContentBitmapFont* ContentStatus::asBitmapFont() const
	{
		return static_cast<ContentBitmapFont*>(_castInstance);
	}

	ContentJson* ContentStatus::asJson() const
	{
		return static_cast<ContentJson*>(_castInstance);
	}

	void ContentStatus::invokeCallback()
	{
		if (_callback != nullptr)
		{
			_callback->invoke(this);
		}
	}

	void ContentStatus::detectType()
	{
		std::string ext = core::Path::getFileExt(path);
		if (ext == "mesh")
		{
			type = ContentType::Mesh;
		}
		else if (ext == "png")
		{
			type = ContentType::Image;
		}
		else if (ext == "frame_animation")
		{
			type = ContentType::FrameAnimation;
		}
		else if (ext == "otf" || ext == "ttf")
		{
			type = ContentType::BitmapFont;
		}
		else
		{
			type = ContentType::Json;
		}
	}

	void ContentStatus::createContentInstance()
	{
		ContentJson* json;
		ContentImage* image;
		ContentMesh* mesh;
		ContentBitmapFont* font;
		ContentFrameAnimation* frameAnimation;
		switch (type)
		{
		case ContentType::None: break;
		case ContentType::Mesh:
			mesh = ContentMesh::create();
			_instance = mesh;
			_castInstance = mesh;
			break;
		case ContentType::Image:
			image = ContentImage::create();
			_instance = image;
			_castInstance = image;
			break;
		case ContentType::FrameAnimation: 
			frameAnimation = ContentFrameAnimation::create();
			_instance = frameAnimation;
			_castInstance = frameAnimation;
			break;
		case ContentType::BitmapFont: 
			font = ContentBitmapFont::create();
			_instance = font;
			_castInstance = font;
			break;
		case ContentType::Json:
			json = ContentJson::create();
			_instance = json;
			_castInstance = json;
			break;
		default: break;
		}
	}
}
