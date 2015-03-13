#include "UniformValueVector3.h"
#include "Errors.h"

namespace graphics
{
	UniformValueVector3::UniformValueVector3() : _value(core::Math::MaxValue, core::Math::MaxValue, core::Math::MaxValue), _applyMask(0)
	{
	}

	void UniformValueVector3::setValue(const core::Vector3 &value)
	{
		_equal = core::Vector3::equals(value, _value);
		_value = value;
		_applyMask = 0;
	}

	void UniformValueVector3::apply(Uniform *uniform)
	{
		if (_equal) return;
		unsigned int shaderMask = uniform->getShaderMask();
		if ((_applyMask & shaderMask) != 0) return;
		_applyMask |= shaderMask;
		glUniform3f(uniform->getHandle(), _value.getX(), _value.getY(), _value.getZ());
		Errors::check(Errors::Uniform3f);
	}

	void UniformValueVector3::reset()
	{
		_equal = false;
	}

}