#include "Errors.h"
#include "UniformInfo.h"

namespace graphics
{
	UniformInfo::UniformInfo(const char *name, UniformType::e type) : _name(name), _type(type)
	{
	}

	const char *UniformInfo::getName()
	{
		return _name.c_str();
	}

	const UniformType::e UniformInfo::getType()
	{
		return _type;
	}
}