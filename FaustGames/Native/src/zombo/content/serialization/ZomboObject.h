#ifndef ZOMBO_OBJECT_H
#define ZOMBO_OBJECT_H

#include "../../zombo_classes.h"
#include "ZomboValue.h"

namespace zombo
{
	class ZomboObject
	{
	public:
		ZomboValue* operator[](const char *fieldName);
		std::string toString();
		typedef std::map<std::string, ZomboValue *> ValuesMap;
	private:
		ValuesMap _values;
	};
}

#endif /*ZOMBO_OBJECT_H*/