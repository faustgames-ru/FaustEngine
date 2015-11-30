#ifndef ZOMBO_CLASSES_H
#define ZOMBO_CLASSES_H

#include "zombo.h"
#include "zombo_references.h"
#include "zombo_platform.h"

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

	struct ZomboRect
	{
		core::Vector2 position;
		core::Vector2 size;
	};	

	class ZomboSpriteImage
	{
	public:
		uint textureId;
		TextureCoords uvs[4];
	};

	template <typename TResult>
	class ZomboAsyncState;
}

#endif /* ZOMBO_CLASSES_H */
