#ifndef UNIFORM_H
#define UNIFORM_H

#include "graphics.h"
#include "Matrix.h"
#include "Vector.h"
#include "UniformValue.h"

namespace graphics
{
	class Uniform
	{
	public:
		Uniform();
		~Uniform();
		UniformInfo *getInfo();
		void init(UniformInfo *info, UniformValue *value);
		void create(Effect *effect);
		GLuint getHandle();
		inline UniformValue *getValueContainer(){ return _valueContainer; }
	protected:
	private:
		UniformValue *_valueContainer;
		GLuint _parameterHandler;
		UniformInfo *_info;
	};
}

#endif /*UNIFORM_H*/