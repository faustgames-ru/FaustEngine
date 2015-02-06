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
		void setValue(GLuint value);
		virtual void apply(Uniform *uniform);
		virtual void reset();
		virtual int getSamplerIndex(){ return _samplerIndex; };
		static void resetSamplers();
	protected:
	private:
		static int _samplerCounter;
		static core::StaticArray<UniformValueTexture*, 32> _samplers;
		GLuint _value;
		bool _equal;
		int _samplerIndex;
	};
}

#endif /*UNIFORM_VALUE_TEXTURE_H*/
