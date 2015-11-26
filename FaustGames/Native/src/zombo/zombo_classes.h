#ifndef ZOMBO_CLASSES_H
#define ZOMBO_CLASSES_H

#include "zombo.h"
#include "zombo_references.h"

namespace zombo
{
	struct TextureCoords
	{
		ushort u;
		ushort v;
	};
	
	struct RenderVertex
	{
		core::Vector3 xyz;
		uint color;
		TextureCoords uv;
	};

	class ZomboSpriteImage
	{
	public:
		uint textureId;
		TextureCoords uvs[4];
	};

}

#endif /* ZOMBO_CLASSES_H */
