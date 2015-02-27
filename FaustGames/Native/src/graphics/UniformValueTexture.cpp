#include "UniformValueTexture.h"
#include "Errors.h"
#include "Math.h"

namespace graphics
{
	int UniformValueTexture::_samplerCounter(1);
	core::StaticArray<UniformValueTexture*, GraphicsConstants::Samplers2DLimit> UniformValueTexture::_samplers;

	UniformValueTexture::UniformValueTexture() : _value(0), _samplerIndex(_samplerCounter++)
	{
		_samplers.add(this);
	}
	void UniformValueTexture::setValue(GLuint value)
	{
		_equal = value == _value;
		_value = value;
	}
	void UniformValueTexture::apply(Uniform *uniform)
	{
		if (_equal) return;
		glActiveTexture(GL_TEXTURE0 + GraphicsConstants::Samplers2DStart + _samplerIndex);
		Errors::check(Errors::ActiveTexture);
		glBindTexture(GL_TEXTURE_2D, _value);
		Errors::check(Errors::BindTexture);
	}

	void UniformValueTexture::reset()
	{
		_equal = false;
		_value = 0;
	}

	void UniformValueTexture::resetSamplers()
	{
		for (int i = 0; i < _samplers.count; i++)
			_samplers.data[i]->reset();
	}
}