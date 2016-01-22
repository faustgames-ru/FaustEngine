#ifndef ZOMBO_CLASSES_H
#define ZOMBO_CLASSES_H

#include "zombo.h"
#include "zombo_references.h"
#include "zombo_platform.h"

namespace zombo
{
	struct TextureCoords
	{
		unsigned short u;
		unsigned short v;
	};
	
	struct ColorVertex
	{
		ColorVertex()
			: color(0)
		{};
		ColorVertex(float x, float y, float z, uint c)
			: color(0)
		{
			xyz.setX(x);
			xyz.setY(y);
			xyz.setZ(z);
			color = c;
		};
		core::Vector3 xyz;
		uint color;
	};

	struct RenderVertex
	{
		core::Vector3 xyz;
		uint color;
		TextureCoords uv;
		RenderVertex(): color(0)
		{}
		RenderVertex(float x, float y, float z, uint c, ushort u, ushort v)
		{
			xyz.setX(x);
			xyz.setY(y);
			xyz.setZ(z);
			color = c;
			uv.u = u;
			uv.v = v;
		}
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
		static const ushort t0 = 1;
		static const ushort t1 = 65535;
		static ushort quadIndices[6];
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
