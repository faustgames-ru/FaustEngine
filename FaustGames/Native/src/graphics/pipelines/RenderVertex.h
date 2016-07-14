#ifndef GRPAHICS_PIPELINES_RENDER_VERTEX_H
#define GRPAHICS_PIPELINES_RENDER_VERTEX_H

#include "../graphics_classes.h"
#include "../graphics.h"

namespace graphics
{
	struct SpriteVertex
	{
		static VertexFormat * format();
		core::Vector3 xyz;
		uint color;
		ushort u;
		ushort v;
	};
	
	struct ColorVertex
	{
		static VertexFormat * format();
		core::Vector3 xyz;
		uint color;
	};
}

#endif /*GRPAHICS_PIPELINES_RENDER_VERTEX_H*/