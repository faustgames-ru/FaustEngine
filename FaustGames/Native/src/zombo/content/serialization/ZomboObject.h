#ifndef ZOMBO_OBJECT_H
#define ZOMBO_OBJECT_H

#include "../../zombo_classes.h"
#include "ZomboValue.h"

namespace zombo
{
	class ZomboObject
	{
	public:
		ZomboValue* operator[](const char *fieldName) const;
		typedef std::map<std::string, ZomboValue *> ValuesMap;
	private:
		std::map<std::string, ZomboValue *> _values;
	};
}

#endif /*ZOMBO_OBJECT_H*/