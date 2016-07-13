#ifndef CONTENT_SERIALIZER_H
#define CONTENT_SERIALIZER_H

#include "../content_classes.h"

namespace content
{
	class ContentSerializer
	{
	public:
		static ContentValue * deserializeFromJson(const char* jsonString);
		static std::string serializeToJson(ContentValue *value);
	private:
	};
}

#endif /*CONTENT_SERIALIZER_H*/