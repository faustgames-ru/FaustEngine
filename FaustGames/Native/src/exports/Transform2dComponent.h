#ifndef LLGE_TRANSFORM_2D_COMPONENT_H
#define LLGE_TRANSFORM_2D_COMPONENT_H


#include "llge.h"
#include "..\entities\entity.h"

namespace llge
{
	class Transform2dComponent : public ITransform2dComponent
	{
	public:
		virtual void API_CALL upateTransform(float x, float y, float z, float rotation, float scale);
		virtual void API_CALL upatePivot(float x, float y);
	};
}

#endif /*LLGE_TRANSFORM_2D_COMPONENT_H*/