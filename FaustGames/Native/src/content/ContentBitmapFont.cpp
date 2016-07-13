#include "ContentBitmapFont.h"
#include "ContentStatus.h"

namespace content
{
	ContentBitmapFont* ContentBitmapFont::create()
	{
		return new ContentBitmapFont();
	}

	void ContentBitmapFont::dispose()
	{
		delete this;
	}

	void ContentBitmapFont::load(const LoadArgs& e)
	{
		e.status->invokeCallback();
	}

	ContentBitmapFont::ContentBitmapFont(): font(nullptr)
	{
	}

	ContentBitmapFont::~ContentBitmapFont()
	{
	}
}