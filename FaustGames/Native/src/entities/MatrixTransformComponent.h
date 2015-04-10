#ifndef MATRIX_TRANSFORM_COMPONENT_H
#define MATRIX_TRANSFORM_COMPONENT_H

#include "entities_classes.h"
#include "Component.h"

namespace entities
{
	class MatrixTransformComponent : public Component, public llge::IMatrixTransform
	{
	public:
		core::Matrix Matrix;
		static ComponentType::e Type;

		virtual void API_CALL setTransform(void *floatMatrix)
		{
			Matrix.setData((float *)floatMatrix);
		}
	};
}

#endif /*MATRIX_TRANSFORM_COMPONENT_H*/