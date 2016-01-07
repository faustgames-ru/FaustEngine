#include "ZomboArray.h"

namespace zombo
{
	ZomboValue* ZomboArray::operator[](int i)
	{
		return &_values[i];
	}

	void ZomboArray::resize(int value)
	{
		_values.resize(value);
	}

	int ZomboArray::size() const
	{
		return _values.size();
	}

	std::string ZomboArray::toString()
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
