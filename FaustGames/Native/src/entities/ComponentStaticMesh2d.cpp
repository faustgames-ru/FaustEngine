#include "ComponentStaticMesh2d.h"

namespace entities
{
	graphics::StaticBatch<VertexMesh2d> ComponentStaticMesh2d::StaticBatch;

	StaticComponentsTypes::e ComponentStaticMesh2d::Type(StaticComponentsTypes::StaticMesh2d);
	StaticComponentsMasks::e ComponentStaticMesh2d::Mask(StaticComponentsMasks::StaticMesh2d);
}