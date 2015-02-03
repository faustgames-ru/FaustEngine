#include "UniformValueMatrix.h"
#include "Errors.h"
#include "Math.h"

namespace graphics
{
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
	void UniformValueMatrix::apply(Uniform *uniform)
	{
		if (_equal) return;
		glUniformMatrix4fv(uniform->getHandle(), 1, false, _value.Value.getData());
		Errors::check(Errors::UniformMatrix4fv);
	}

	void UniformValueMatrix::reset()
	{
		_equal = false;
		_value.id = -1;
	}

}