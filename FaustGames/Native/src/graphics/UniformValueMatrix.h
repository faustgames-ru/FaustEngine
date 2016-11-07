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
		virtual void apply(Uniform *uniform) OVERRIDE;
		virtual void reset() OVERRIDE;
	protected:
	private:
		core::MatrixContainer _value;
		bool _equal;
		unsigned int _applyMask;
	};

	class UniformValueMatrix3 : public UniformValue
	{
	public:
		UniformValueMatrix3();
		void setValue(const core::Matrix3Container &value);
		virtual void apply(Uniform *uniform) OVERRIDE;
		virtual void reset() OVERRIDE;
	protected:
	private:
		core::Matrix3Container _value;
		bool _equal;
		unsigned int _applyMask;
	};
}

#endif /*UNIFORM_VALUE_MATRIX_H*/
