#ifndef GRAPHICS_CLASSES_H
#define GRAPHICS_CLASSES_H

#include "llge.h"
#include "config_defines.h"
#include "graphics_platform.h"
#include "graphics_references.h"

namespace graphics
{
	struct Image2dFormat
	{
		enum e
		{
			Rgba = 0x0,
			Rgb = 0x1,
		};
	};

	struct BlendState
	{
		enum e
		{
			None = 0x0,
			Alpha = 0x1,
			Additive = 0x2,
		};
	};

	struct DepthState
	{
		enum e
		{
			None = 0x0,
			Read = 0x01,
			Write = 0x02,
			ReadWrite = 0x03
		};
	};

	struct UniformType
	{
		enum e
		{
			Float,
			Vector2,
			Vector3,
			Vector4,
			Matrix3,
			Matrix4,
			Color,
			Sampler2D,
			SamplerCube,
		};
	};

	struct AttributeType
	{
		enum e
		{
			Float,
			Vector2,
			Vector3,
			Vector4,
			Color,
			Normal,
			TextureCoord,
			TextureCoordPacked,
		};
	};


	class Uniforms;

	class Effect;
	class UniformInfo;
	class UniformValue;
	class Uniform;
	class VertexDeclaration;
	class Texture;
	class RenderState;
	class GraphicsDevice;
	class Color;
	class Errors;

	class GraphicsConstants
	{
	public:
		static const int Samplers2DStart = 1;
		static const int Samplers2DLimit = 5;
		static const int SamplersCubeStart = 6;
		static const int SamplersCubeLimit = 2;
		static const int StaticBatchBufferSize = 32768;
		static const int StaticBatchBlocksLimit = 2;
		static const int LocalBufferSize = 32768;
	};
}

#endif /*GRAPHICS_CLASSES_H*/