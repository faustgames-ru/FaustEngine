#include "Uniform.h"
#include "UniformInfo.h"
#include "Errors.h"
#include "Math.h"

namespace graphics
{
	Uniform::Uniform()
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

	void Uniform::create(GLuint shaderProgram)
	{
		_parameterHandler = glGetUniformLocation(shaderProgram, _info->getName());
		Errors::check(Errors::GetUniformLocation);
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
	void UniformValueMatrix::setValue(const core::MatrixContainer value)
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
}