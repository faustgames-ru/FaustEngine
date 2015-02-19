#ifndef COMPONENT_AABB_2D_H
#define COMPONENT_AABB_2D_H

#include "entities_classes.h"

namespace entities
{
	class ComponentAabb2d
	{
	public:
		int RenderId;
		int ZLayer;
		geometry::Aabb2d Aabb;
		
		static StaticComponentsTypes::e Type;
		static StaticComponentsMasks::e Mask;

	};
}

#endif /*COMPONENT_AABB_2D_H*/