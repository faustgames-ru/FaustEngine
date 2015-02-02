#ifndef UNIFORM_VALUE_FLOAT_H
#define UNIFORM_VALUE_FLOAT_H

#include "graphics.h"
#include "UniformValue.h"

namespace graphics
{
	class UniformValueFloat : public UniformValue
	{
	public:
		UniformValueFloat();
		void setValue(float value);
		virtual void apply(Uniform *uniform);
		virtual void reset();
	protected:
	private:
		float _value;
		bool _equal;
	};
}

#endif /*UNIFORM_VALUE_FLOAT_H*/
