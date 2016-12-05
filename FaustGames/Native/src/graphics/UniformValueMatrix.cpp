#include "UniformValueMatrix.h"
#include "Errors.h"
#include "Math.h"

namespace graphics
{
	UniformValueMatrix::UniformValueMatrix() : _equal(false), _applyMask(0)
	{
		_value.id = -1;
	}
	void UniformValueMatrix::setValue(const core::MatrixContainer &value)
	{
		_equal = _value.id == value.id;
		if (_equal) return;
		_value.Value = value.Value;
		_value.id = value.id;
		_applyMask = 0;
	}
	void UniformValueMatrix::apply(Uniform *uniform)
	{
		//if (_equal) return;
		
		unsigned int shaderMask = uniform->getShaderMask();		
		if ((_applyMask & shaderMask) != 0) return;		
		_applyMask |= shaderMask;
		
		const float * data = _value.Value.getData();
		glUniformMatrix4fv(uniform->getHandle(), 1, false, data);
		Errors::check(Errors::UniformMatrix4fv);
	}

	void UniformValueMatrix::reset()
	{
		_equal = false;
		_value.id = -1;
	}

	UniformValueMatrix3::UniformValueMatrix3(): _equal(false), _applyMask(0)
	{
		_value.id = -1;
	}

	void UniformValueMatrix3::setValue(const core::Matrix3Container& value)
	{
		_equal = _value.id == value.id;
		if (_equal) return;
		_value.Value = value.Value;
		_value.id = value.id;
		_applyMask = 0;
	}

	void UniformValueMatrix3::apply(Uniform* uniform)
	{

		//if (_equal) return;
		
		unsigned int shaderMask = uniform->getShaderMask(); 
		if ((_applyMask & shaderMask) != 0) return;
		_applyMask |= shaderMask;
		
		const float * data = _value.Value.getData();
		glUniformMatrix3fv(uniform->getHandle(), 1, false, data);
		Errors::check(Errors::UniformMatrix3fv);
	}

	void UniformValueMatrix3::reset()
	{
		_equal = false;
		_value.id = -1;
	}
}
