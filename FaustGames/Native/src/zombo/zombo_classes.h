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

	struct ZomboComponentTypes
	{
		enum e
		{
			Bounds = 0x0,
			Transform = 0x1,
			RenderSprite = 0x2,
			FrameAnimation = 0x3,
			Behaviors = 0x3,
			Count = 0x4,
		};
	};

	class ZomboConstants
	{
	public:
		static const float GameScale; // 100 dip per meter
		static const uint LongTimeScale = 0xffffffff; // does 136 years of game time good enough?
	};

	class ZomboEntity;
	
	struct ZomboType
	{
		enum e
		{
			None,
			Int,
			Bool,
			Float,
			String,
			Array,
			Object
		};
	};
	
	class ZomboValue;
	class ZomboObject;
	class ZomboArray;

	class IZomboSerialable
	{
	public:
		void deserialize(ZomboObject* source);
		void serialize(ZomboObject* target);
	};
}



#endif /* ZOMBO_CLASSES_H */
