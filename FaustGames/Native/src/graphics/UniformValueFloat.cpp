#include "UniformValueFloat.h"
#include "Errors.h"

namespace graphics
{
	UniformValueFloat::UniformValueFloat() : _value(core::Math::MaxValue), _applyMask(0)
	{
	}

	void UniformValueFloat::setValue(float value)
	{
		_equal = core::Math::equals(value, _value);
		_value = value;
		_applyMask = 0;
	}

	void UniformValueFloat::apply(Uniform *uniform)
	{
		//if (_equal) return;
		unsigned int shaderMask = uniform->getShaderMask();
		if ((_applyMask & shaderMask) != 0) return;
		_applyMask |= shaderMask;
		glUniform1f(uniform->getHandle(), _value);
		Errors::check(Errors::Uniform1f);
	}

	void UniformValueFloat::reset()
	{
		_equal = false;
	}

}