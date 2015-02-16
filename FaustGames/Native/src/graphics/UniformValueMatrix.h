#ifndef UNIFORM_VALUE_MATRIX_H
#define UNIFORM_VALUE_MATRIX_H

#include "graphics_classes.h"
#include "UniformValue.h"

namespace graphics
{
	class UniformValueMatrix : public UniformValue
	{
	public:
		UniformValueMatrix();
		void setValue(const core::MatrixContainer &value);
		virtual void apply(Uniform *uniform);
		virtual void reset();
	protected:
	private:
		core::MatrixContainer _value;
		bool _equal;
		unsigned int _applyMask;
	};
}

#endif /*UNIFORM_VALUE_MATRIX_H*/
