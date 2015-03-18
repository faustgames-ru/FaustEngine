#ifndef AABB_2D_COMPONENT_H
#define AABB_2D_COMPONENT_H

#include "entities_classes.h"
#include "Component.h"

namespace entities
{
	class Aadd2dComponent : public Component, public llge::IAabb2d
	{
	public:
		geometry::Aabb2d Aadd;
		float ZOrder;
		static ComponentType::e Type;		
		virtual void API_CALL update(float minX, float minY, float maxX, float maxY, float zOrder);
	private:
	};
}

#endif /*AABB_2D_COMPONENT_H*/