#ifndef UNIFORM_H
#define UNIFORM_H

#include "graphics_classes.h"
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
		inline int getShaderId(){ return _shaderId; }
		inline unsigned int getShaderMask(){ return _shaderMask; }
	protected:
	private:
		UniformValue *_valueContainer;
		GLuint _parameterHandler;
		UniformInfo *_info;
		int _shaderId;
		unsigned int _shaderMask;
	};
}

#endif /*UNIFORM_H*/