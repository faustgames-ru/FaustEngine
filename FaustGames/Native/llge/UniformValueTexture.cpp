#include "UniformValueTexture.h"
#include "Errors.h"
#include "Math.h"
#include "StaticArray.h"

namespace graphics
{
	int UniformValueTexture::_samplerCounter(1);
	core::StaticArray<UniformValueTexture*, 32> UniformValueTexture::_samplers;

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
		glActiveTexture(GL_TEXTURE0 + _samplerIndex);
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