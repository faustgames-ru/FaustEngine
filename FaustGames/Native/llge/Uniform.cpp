#include "Uniform.h"
#include "UniformInfo.h"
#include "Errors.h"
#include "Math.h"
#include "Effect.h"

namespace graphics
{
	Uniform::Uniform() : _samplerIndex(-1)
	{
	}
	
	Uniform::~Uniform()
	{
	}

	UniformInfo *Uniform::getInfo()
	{
		return _info;
	}

	void Uniform::init(UniformInfo *value, UniformValue *uniformValue)
	{
		_info = value;
		valueContainer = uniformValue;
		uniformValue->setUniform(this);
	}

	void Uniform::create(Effect * effect)
	{
		_parameterHandler = glGetUniformLocation(effect->getHandle(), _info->getName());
		Errors::check(Errors::GetUniformLocation);
		if (_info->getType() == UniformType::Sampler2D)
		{
			_samplerIndex = effect->generateSamplerIndex();
			glUniform1i(_parameterHandler, _samplerIndex);
			Errors::check(Errors::Uniform1f);
		}
	}

	GLuint Uniform::getHandle()
	{
		return _parameterHandler;
	}

	UniformValueFloat::UniformValueFloat() : _value(core::Math::MaxValue)
	{
	}

	void UniformValueFloat::setValue(float value)
	{
		_equal = core::Math::equals(value, _value);
		_value = value;
	}

	void UniformValueFloat::apply()
	{
		if (_equal) return;
		glUniform1f(_uniform->getHandle(), _value);
		Errors::check(Errors::Uniform1f);
	}

	UniformValueMatrix::UniformValueMatrix()
	{
		_value.id = -1;
	}
	void UniformValueMatrix::setValue(const core::MatrixContainer &value)
	{
		_equal = _value.id == value.id;
		if (_equal) return;
		_value.Value = value.Value;
		_value.id = value.id;
	}
	void UniformValueMatrix::apply()
	{
		if (_equal) return;
		glUniformMatrix4fv(_uniform->getHandle(), 1, false, _value.Value.getData());
		Errors::check(Errors::UniformMatrix4fv);
	}

	UniformValueTexture::UniformValueTexture() : _value(0)
	{
	}
	void UniformValueTexture::setValue(GLuint value)
	{
		_equal = value == _value;
		_value = value;
	}
	void UniformValueTexture::apply()
	{
		//if (_equal) return;		
		
		// todo: optimize sampler states
		glActiveTexture(GL_TEXTURE0 + _uniform->getSamplerIndex());
		Errors::check(Errors::ActiveTexture);
		glBindTexture(GL_TEXTURE_2D, _value);
		Errors::check(Errors::BindTexture);
	}

}