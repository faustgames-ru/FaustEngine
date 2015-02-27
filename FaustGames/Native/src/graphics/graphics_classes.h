#ifndef GRAPHICS_CLASSES_H
#define GRAPHICS_CLASSES_H

#include "graphics_platform.h"
#include "graphics_references.h"

namespace graphics
{
	struct BlendState
	{
		enum e
		{
			Alpha, 
			Additive,
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
		};
	};


	class Uniforms;
	class Effects;

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
		static const int Samplers2DStart = 0;
		static const int Samplers2DLimit = 5;
		static const int SamplersCubeStart = 5;
		static const int SamplersCubeLimit = 3;
		static const int StaticBatchBufferSize = 32768;
		static const int StaticBatchBlocksLimit = 2;
	};
}

#endif /*GRAPHICS_CLASSES_H*/