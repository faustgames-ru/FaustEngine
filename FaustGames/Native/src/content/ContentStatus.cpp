#include "ContentStatus.h"
#include "ContentImage.h"
#include "ContentFrameAnimation.h"
#include "ContentBitmapFont.h"
#include "../core/Path.h"

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
		if (ext == "png")
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
	}

	void ContentStatus::createContentInstance()
	{
		ContentImage* image;
		ContentBitmapFont* font;
		ContentFrameAnimation* frameAnimation;
		switch (type)
		{
		case ContentType::None: break;
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
		default: break;
		}
	}
}
