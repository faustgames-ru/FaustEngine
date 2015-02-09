#ifndef COMPONENT_TRANSFORM_2D_H
#define COMPONENT_TRANSFORM_2D_H

#include "entities_classes.h"

namespace entities
{
	struct ComponentTransform2d
	{
		float LocalX;
		float LocalY;
		float LocalZ;
		float LocalPivotX;
		float LocalPivotY;
		float LocalRotation;
		float LocalScale;

		float GlobalX;
		float GlobalY;
		float GlobalZ;
		float GlobalPivotX;
		float GlobalPivotY;
		float GlobalRotation;
		float GlobalScale;

		int ParenOffset;
		int NextNodeId;
		int FirtsChildId;
	};
}

#endif /*COMPONENT_TRANSFORM_2D_H*/
