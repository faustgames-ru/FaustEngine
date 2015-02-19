#ifndef COMPONENT_MESH_2D_H
#define COMPONENT_MESH_2D_H

#include "entities_classes.h"

namespace entities
{
	struct MeshVertex2d
	{
		float X;
		float Y;
		float Z;
		unsigned short U;
		unsigned short V;
		unsigned int Color;
	};	
}

#endif /*COMPONENT_MESH_2D_H*/
