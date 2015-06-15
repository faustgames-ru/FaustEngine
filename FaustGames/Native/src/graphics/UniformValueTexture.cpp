#include "GraphicsDevice.h"
#include "UniformValueTexture.h"
#include "Errors.h"
#include "Math.h"

namespace graphics
{
	UniformValueTexture::UniformValueTexture() : _value(0)
	{
	}

	void UniformValueTexture::setSamplerIndex(int samplerIndex)
	{
		_samplerIndex = samplerIndex;
	}


	void UniformValueTexture::setValue(GLuint value)
	{
		_equal = value == _value;
		_value = value;
	}
	void UniformValueTexture::apply(Uniform *uniform)
	{
		if (_equal) return;
		//glUniform1i(uniform->getHandle(), GraphicsConstants::Samplers2DStart + _samplerIndex);
		//Errors::check(Errors::Uniform1f);
        GraphicsDevice::Default.setActiveTexture(_samplerIndex);
        //glActiveTexture(GL_TEXTURE0 + GraphicsConstants::Samplers2DStart + _samplerIndex);
		//Errors::check(Errors::ActiveTexture);
		glBindTexture(GL_TEXTURE_2D, _value);
		Errors::check(Errors::BindTexture);
	}

	void UniformValueTexture::reset()
	{
		_equal = false;
		_value = 0;
	}

}