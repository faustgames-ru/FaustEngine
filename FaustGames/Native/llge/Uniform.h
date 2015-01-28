#ifndef UNIFORM_H
#define UNIFORM_H

#include "graphics.h"

namespace graphics
{
	class Uniform
	{
	public:
		Uniform(UniformInfo *info);
		const UniformInfo *getInfo();
	};
}

#endif /*UNIFORM_H*/