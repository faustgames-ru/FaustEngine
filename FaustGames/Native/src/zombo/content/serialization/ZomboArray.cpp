#include "ZomboArray.h"

namespace zombo
{
	const ZomboValue* ZomboArray::operator[](int i) const
	{
		return &_values[i];
	}

	void ZomboArray::resize(int value)
	{
		_values.resize(value);
	}

	int ZomboArray::size()
	{
		return _values.size();
	}
}
