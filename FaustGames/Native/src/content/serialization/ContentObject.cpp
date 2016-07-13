#include "ContentObject.h"

namespace content
{
	ContentObject::ContentObject()
	{
	}

	ContentObject::~ContentObject()
	{
		for (ValuesMap::iterator i = _values.begin(); i != _values.end(); ++i)
		{
			delete i->second;
		}
	}

	ContentValue* ContentObject::operator[](const char* fieldName)
	{
		std::string name(fieldName);
		ValuesMap::const_iterator it = _values.find(name);
		if (it == _values.end())
			_values[name] = new ContentValue();
		return _values[name];
	}

	std::string ContentObject::toString()
	{
		std::string result = "{";
		for (ValuesMap::iterator i = _values.begin(); i != _values.end(); ++i)
		{
			ContentValue *v = i->second;
			result += i->first;
			result += ":";
			result += v->toString();
			result += ",";
		}
		result += "}";
		return result;
	}

	ContentObject::ValuesMap& ContentObject::getValuesMap()
	{
		return _values;
	}
}
