#ifndef ZOMBO_CLASSES_H
#define ZOMBO_CLASSES_H

#include "zombo.h"
#include "zombo_references.h"
#include "zombo_platform.h"

namespace zombo
{
	class ZomboConstants
	{
	public:
		static const ushort t0 = 1;
		static const ushort t05 = 32768;
		static const ushort t1 = 65535;
		static float GameScale; // 100 dip per meter
		static float SmoothLevel;
		static float circlesDetail;
		const static uint LongTimeScale = 0xffffffff; // does 136 years of game time good enough?
		static ushort indicesTriangle1[3];
		static ushort indicesTriangle2[6];
		static ushort indicesQuad[6];
	};

	struct TextureCoords
	{
		unsigned short u;
		unsigned short v;
		static TextureCoords fromVector(core::Vector2 uvPosition, float min, float max)
		{
			TextureCoords uv;
			uv.u = static_cast<ushort>(core::Math::round(ZomboConstants::t0 + (uvPosition.getX() - min) / (max - min) * (ZomboConstants::t1 - ZomboConstants::t0)));
			uv.v = static_cast<ushort>(core::Math::round(ZomboConstants::t0 + (uvPosition.getY() - min) / (max - min) * (ZomboConstants::t1 - ZomboConstants::t0)));
			return uv;
		}
	};
	
	struct ColorVertex
	{
		ColorVertex()
			: color(0)
		{};
		ColorVertex(core::Vector2 position, uint c)
		{
			xyz = position.toVector3();
			color = c;
		}
		ColorVertex(core::Vector3 position, uint c)
		{
			xyz = position;
			color = c;
		}
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
		RenderVertex(core::Vector3 position, uint c, TextureCoords uvPosition)
		{
			xyz = position;
			color = c;
			uv = uvPosition;
		}

		RenderVertex(core::Vector3 position, uint c, ushort u, ushort v)
		{
			xyz = position;
			color = c;
			uv.u = u;
			uv.v = v;
		}
		
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
