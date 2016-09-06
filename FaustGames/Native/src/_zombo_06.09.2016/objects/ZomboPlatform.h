#ifndef ZOMBO_PLATFORM_H
#define ZOMBO_PLATFORM_H

#include "../zombo_classes.h"
#include "../content/ZomboAtlas.h"

namespace zombo
{
	class ZomboPlatform
	{
	public:
		core::Vector3 _p0;
		core::Vector3 _p1;
		void update();
	};
}

#endif /*ZOMBO_PLATFORM_H*/