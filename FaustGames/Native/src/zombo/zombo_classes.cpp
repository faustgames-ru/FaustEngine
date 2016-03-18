#include "zombo_classes.h"

namespace zombo
{
	ushort ZomboConstants::indicesTriangle1[3] = { 0, 1, 2 };
	ushort ZomboConstants::indicesTriangle2[6] = { 0, 1, 2, 3, 4, 5 };
	ushort ZomboConstants::indicesQuad[6] = { 0, 1, 2, 0, 2, 3 };


	float ZomboConstants::GameScale = 0.01f; // 100 dip per meter
	float ZomboConstants::SmoothLevel = 1.0f; // 2px
	float ZomboConstants::circlesDetail = 2.0f; // 2px
}