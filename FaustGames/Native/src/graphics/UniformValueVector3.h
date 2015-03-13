#ifndef UNIFORM_VALUE_VECTOR3_H
#define UNIFORM_VALUE_VECTOR3_H

#include "graphics_classes.h"
#include "UniformValue.h"

namespace graphics
{
	class UniformValueVector3 : public UniformValue
	{
	public:
		UniformValueVector3();
		void setValue(const core::Vector3 &value);
		virtual void apply(Uniform *uniform);
		virtual void reset();
	protected:
	private:
		core::Vector3 _value;
		bool _equal;
		unsigned int _applyMask;
	};
}

#endif /*UNIFORM_VALUE_FLOAT_H*/
