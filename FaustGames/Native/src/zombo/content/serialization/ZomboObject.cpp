#include "ZomboObject.h"

namespace zombo
{
	ZomboValue* ZomboObject::operator[](const char* fieldName) const
	{
		std::string name(fieldName);
		ValuesMap::const_iterator it = _values.find(name);
		/*
		if (it == _values.end())
			_values[name] = new ZomboValue();
		return _values[name];
		*/
		return nullptr;
	}
}
