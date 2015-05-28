#ifndef UNIFORM_VALUE_TEXTURE_H
#define UNIFORM_VALUE_TEXTURE_H

#include "graphics_classes.h"
#include "UniformValue.h"

namespace graphics
{
	class UniformValueTexture : public UniformValue
	{
	public:
		UniformValueTexture();
		void setSamplerIndex(int samplerIndex);
		void setValue(GLuint value);
		virtual void apply(Uniform *uniform);
		virtual void reset();
		virtual int getSamplerIndex(){ return _samplerIndex; };
	protected:
	private:
		GLuint _value;
		bool _equal;
		int _samplerIndex;
	};
}

#endif /*UNIFORM_VALUE_TEXTURE_H*/
