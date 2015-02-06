#ifndef UNIFORM_VALUE_H
#define UNIFORM_VALUE_H

#include "graphics_classes.h"
#include "Uniform.h"

namespace graphics
{
	class UniformValue
	{
	public:
		virtual void apply(Uniform *uniform) = 0;
		virtual void reset() = 0;
		virtual int getSamplerIndex(){ return -1; };
	private:
	};
}

#endif /*UNIFORM_VALUE_H*/
