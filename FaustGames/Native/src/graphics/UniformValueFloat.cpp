#include "UniformValueFloat.h"
#include "Errors.h"

namespace graphics
{
	UniformValueFloat::UniformValueFloat() : _value(core::Math::MaxValue)
	{
	}

	void UniformValueFloat::setValue(float value)
	{
		_equal = core::Math::equals(value, _value);
		_value = value;
	}

	void UniformValueFloat::apply(Uniform *uniform)
	{
		if (_equal) return;
		glUniform1f(uniform->getHandle(), _value);
		Errors::check(Errors::Uniform1f);
	}

	void UniformValueFloat::reset()
	{
		_equal = false;
	}

}