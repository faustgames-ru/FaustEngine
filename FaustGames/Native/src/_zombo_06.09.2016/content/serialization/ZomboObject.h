#ifndef ZOMBO_OBJECT_H
#define ZOMBO_OBJECT_H

#include "../../zombo_classes.h"
#include "ZomboValue.h"

namespace zombo
{
	class ZomboObject
	{
	public:
		ZomboObject();
		~ZomboObject();
		ZomboValue* operator[](const char *fieldName);
		std::string toString();
		typedef std::map<std::string, ZomboValue *> ValuesMap;
		ValuesMap& getValuesMap();
	private:
		ValuesMap _values;
	};
}

#endif /*ZOMBO_OBJECT_H*/