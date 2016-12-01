#ifndef GRAPHICS_CLASSES_H
#define GRAPHICS_CLASSES_H

#include "llge.h"
#include "config_defines.h"
#include "graphics_platform.h"
#include "graphics_references.h"

namespace graphics
{
	struct Image2dBlocksOrder
	{
		enum e
		{
			Normal = 0x0,
			Morton = 0x1,
		};
	};

	struct Image2dFormat
	{
		enum e
		{
			Rgba = 0x0,
			Rgb = 0x1,
			Pvrtc12 = 0x2,
			Pvrtc14 = 0x3,
			Etc1 = 0x4,
			Etc2 = 0x5,
			Rgba4444 = 0x6,
			Atc = 0x7,
			Astc = 0x8,
		};
	};

	struct BlendState
	{
		enum e
		{
			None = 0x0,
			Alpha = 0x1,
			Additive = 0x2,
			Normal = 0x3,
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

	struct DepthFunc
	{
		enum e
		{
			None = 0x0,
			Less = 0x01,
			LessEqual = 0x02,
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

	struct FilterMode
	{
		enum e
		{
			Nearest = 0x0,
			Linear = 0x1,
			NearestMipmapNearest = 0x2,
			LinearMipmapNearest = 0x3,
			NearestMipmapLinear = 0x4,
			LinearMipmapLinear = 0x5,
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
		static const int Samplers2DLimit = 6;
		static const int SamplersCubeStart = 7;
		static const int SamplersCubeLimit = 2;
		static const int StaticBatchBufferSize = 32768;
		static const int StaticBatchBlocksLimit = 2;
		static const int LocalBufferSize = 32768;
	};
}

#endif /*GRAPHICS_CLASSES_H*/