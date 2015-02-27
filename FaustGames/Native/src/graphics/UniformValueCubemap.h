#ifndef UNIFORM_VALUE_CUBEMAP_H
#define UNIFORM_VALUE_CUBEMAP_H

#include "graphics_classes.h"
#include "UniformValue.h"

namespace graphics
{
	class UniformValueCubemap : public UniformValue
	{
	public:
		UniformValueCubemap();
		void setValue(GLuint value);
		virtual void apply(Uniform *uniform);
		virtual void reset();
		virtual int getSamplerIndex(){ return _samplerIndex; };
		static void resetSamplers();
	protected:
	private:
		static int _samplerCounter;
		static core::StaticArray<UniformValueCubemap*, GraphicsConstants::SamplersCubeLimit> _samplers;
		GLuint _value;
		bool _equal;
		int _samplerIndex;
	};
}

#endif /*UNIFORM_VALUE_CUBEMAP_H*/
