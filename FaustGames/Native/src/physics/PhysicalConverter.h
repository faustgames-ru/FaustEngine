#ifndef PHYSICAL_CONVERTER_H
#define PHYSICAL_CONVERTER_H

#include "physics_classes.h"

namespace physics
{
	class PhysicalConverter
	{
	public:
		PhysicalConverter();
		~PhysicalConverter();
		float toWorld(float value);
		float fromWorld(float value);

		void setup(float scale);
	private:
		float _scale;
		float _unScale;
	};
}

#endif /*PHYSICAL_CONVERTER_H*/