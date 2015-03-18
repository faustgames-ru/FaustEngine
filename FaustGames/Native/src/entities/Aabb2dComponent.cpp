#include "Aabb2dComponent.h"

namespace entities
{
	ComponentType::e Aadd2dComponent::Type(ComponentType::Aadd2d);

	void API_CALL Aadd2dComponent::update(float minX, float minY, float maxX, float maxY, float zOrder)
	{
		Aadd.Min.setX(minX);
		Aadd.Min.setY(minY);
		Aadd.Max.setX(maxX);
		Aadd.Max.setY(maxY);
		ZOrder = zOrder;
	}

}