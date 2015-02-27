#ifndef AABB_2D_COMPONENT_H
#define AABB_2D_COMPONENT_H

#include "entities_classes.h"
#include "Component.h"

namespace entities
{
	class Aadd2dComponent : public Component
	{
	public:
		geometry::Aabb2d Aadd;
		float ZOrder;
		static ComponentType::e Type;
	private:
	};
}

#endif /*AABB_2D_COMPONENT_H*/