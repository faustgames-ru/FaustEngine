#include "ContentFrameAnimation.h"
#include "ContentImage.h"
#include "ContentStatus.h"
#include "serialization/ContentSerializer.h"
#include "serialization/ContentValue.h"
#include "serialization/ContentObject.h"
#include "serialization/ContentArray.h"
#include "../core/Path.h"

namespace content
{
	ContentFrameAnimation* ContentFrameAnimation::create()
	{
		return new ContentFrameAnimation();
	}

	ContentImage* ContentFrameAnimation::frameFromPercent(float percent)
	{
		if (frames.size() == 0)
		{
			return nullptr;
		}
		int i = static_cast<int>(core::Math::trunc(percent * frames.size()));
		if (i >= frames.size())
		{
			i = frames.size() - 1;
		}
		if (i < 0)
		{
			i = 0;
		}
		return frames[i];
	}

	void ContentFrameAnimation::dispose()
	{
		delete this;
	}

	void ContentFrameAnimation::load(const LoadArgs& e)
	{
		resources::ContentManager* content = &resources::ContentManager::Default;
		ContentValue* rootValue = ContentSerializer::deserializeFromJson(content->loadString(e.fullPath.c_str()));
		ContentObject* root = rootValue->asObject();
		float frameRate = (*root)["frames_rate"]->asFloat();
		ContentArray* framesArray = (*root)["frames"]->asArray();
		frames.resize(framesArray->size());
		std::string thisDir = core::Path::getFilePath(e.path);
		std::string thisFullDir = core::Path::getFilePath(e.fullPath);
		for (uint i = 0; i < frames.size(); i++)
		{
			ContentImage* frame = ContentImage::create();
			LoadArgs loadFrameArgs;
			loadFrameArgs.status = nullptr;
			loadFrameArgs.owner = e.owner;
			const char* name = (*framesArray)[i]->asString();
			loadFrameArgs.path = thisDir + name;
			loadFrameArgs.fullPath = thisFullDir + name;
			frame->internalLoad(loadFrameArgs);
			frames[i] = frame;
		}
		rootValue->dispose();
		e.status->invokeCallback();
	}
}