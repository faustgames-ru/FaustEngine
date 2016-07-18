#include "ContentJson.h"
#include "ContentStatus.h"
#include "serialization/ContentValue.h"
#include "serialization/ContentSerializer.h"

namespace content
{
	ContentJson* ContentJson::create()
	{
		return new ContentJson();
	}

	void ContentJson::internalLoad(const LoadArgs& e)
	{
		resources::ContentManager* content = &resources::ContentManager::Default;
		value = ContentSerializer::deserializeFromJson(content->loadString(e.fullPath.c_str()));
		e.status->invokeCallback();
	}

	void ContentJson::dispose()
	{
		delete this;
	}

	void ContentJson::load(const LoadArgs& e)
	{
		internalLoad(e);
	}

	ContentJson::ContentJson(): value(nullptr)
	{
	}

	ContentJson::~ContentJson()
	{
		if (value != nullptr)
		{
			value->dispose();
		}
	}
}