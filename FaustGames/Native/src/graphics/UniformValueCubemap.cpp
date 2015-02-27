#include "UniformValueCubemap.h"
#include "Errors.h"
#include "Math.h"

namespace graphics
{
	int UniformValueCubemap::_samplerCounter(1);
	core::StaticArray<UniformValueCubemap*, GraphicsConstants::SamplersCubeLimit> UniformValueCubemap::_samplers;

	UniformValueCubemap::UniformValueCubemap() : _value(0), _samplerIndex(_samplerCounter++)
	{
		_samplers.add(this);
	}
	void UniformValueCubemap::setValue(GLuint value)
	{
		_equal = value == _value;
		_value = value;
	}
	void UniformValueCubemap::apply(Uniform *uniform)
	{
		if (_equal) return;
		glActiveTexture(GL_TEXTURE0 + GraphicsConstants::SamplersCubeStart + _samplerIndex);
		Errors::check(Errors::ActiveTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _value);
		Errors::check(Errors::BindTexture);
	}

	void UniformValueCubemap::reset()
	{
		_equal = false;
		_value = 0;
	}

	void UniformValueCubemap::resetSamplers()
	{
		for (int i = 0; i < _samplers.count; i++)
			_samplers.data[i]->reset();
	}
}