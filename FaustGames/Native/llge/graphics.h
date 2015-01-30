#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "stdafx.h"

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
			Float1,
			Float2,
			Float3,
			Float4,
			Matrix3,
			Matrix4,
			Sampler2D,
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
	class RenderTarget;
	class RenderState;
	class GraphicsDevice;
	class Color;
	class Errors;
}

#endif /*GRAPHICS_H*/