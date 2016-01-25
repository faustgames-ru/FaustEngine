#include "zombo_classes.h"

namespace zombo
{
	ushort ZomboConstants::quadIndices[6] = { 0, 1, 2, 0, 2, 3 };
	float ZomboConstants::GameScale = 0.01f; // 100 dip per meter
	float ZomboConstants::SmoothLevel = 1.0f; // 2px
	float ZomboConstants::circlesDetail = 2.0f; // 2px
}