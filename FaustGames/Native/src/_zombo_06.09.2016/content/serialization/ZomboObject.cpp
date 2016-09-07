#include "ZomboObject.h"

namespace zombo
{
	ZomboObject::ZomboObject()
	{
	}

	ZomboObject::~ZomboObject()
	{
		for (ValuesMap::iterator i = _values.begin(); i != _values.end(); ++i)
		{
			delete i->second;
		}
	}

	ZomboValue* ZomboObject::operator[](const char* fieldName)
	{
		std::string name(fieldName);
		ValuesMap::const_iterator it = _values.find(name);
		if (it == _values.end())
			_values[name] = new ZomboValue();
		return _values[name];
	}

	std::string ZomboObject::toString()
	{
		std::string result = "{";
		for (ValuesMap::iterator i = _values.begin(); i != _values.end(); ++i)
		{
			ZomboValue *v = i->second;
			result += i->first;
			result += ":";
			result += v->toString();
			result += ",";
		}
		result += "}";
		return result;
	}

	ZomboObject::ValuesMap& ZomboObject::getValuesMap()
	{
		return _values;
	}
}
