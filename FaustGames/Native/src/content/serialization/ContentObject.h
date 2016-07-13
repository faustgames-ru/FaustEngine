#ifndef CONTENT_OBJECT_H
#define CONTENT_OBJECT_H

#include "../content_classes.h"
#include "ContentValue.h"

namespace content
{
	class ContentObject
	{
	public:
		ContentObject();
		~ContentObject();
		ContentValue* operator[](const char *fieldName);
		std::string toString();
		typedef std::map<std::string, ContentValue *> ValuesMap;
		ValuesMap& getValuesMap();
	private:
		ValuesMap _values;
	};
}

#endif /*CONTENT_OBJECT_H*/