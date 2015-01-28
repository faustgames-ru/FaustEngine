#ifndef UNIFORM_INFO_H
#define UNIFORM_INFO_H

#include "graphics.h"

namespace graphics
{
	class UniformInfo
	{
	private:
		std::string _name;
		UniformType::e _type;
	public:
		UniformInfo(const char *name, UniformType::e type);
		const char *getName();
		const UniformType::e getType();
	};
}

#endif /*UNIFORM_INFO_H*/