#include "ContentArray.h"

namespace content
{
	ContentValue* ContentArray::operator[](int i)
	{
		return &_values[i];
	}

	void ContentArray::resize(int value)
	{
		_values.resize(value);
	}

	int ContentArray::size() const
	{
		return _values.size();
	}

	std::string ContentArray::toString()
	{
		std::string result = "[";
		for (uint i = 0; i < _values.size(); i++)
		{
			result += _values[i].toString();
			result += ",";
		}
		result += "]";
		return result;
	}
}
