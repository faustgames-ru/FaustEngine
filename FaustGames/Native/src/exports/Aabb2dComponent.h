#ifndef LLGE_AABB_2D_COMPONENT_H
#define LLGE_AABB_2D_COMPONENT_H


#include "llge.h"
#include "..\entities\entity.h"

namespace llge
{
	class Aabb2dComponent : public IAabb2dComponent
	{
	public:
		virtual void API_CALL upateAabb2d(float minX, float minY, float maxX, float maxY);
	};
}

#endif /*LLGE_AABB_2D_COMPONENT_H*/