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
			Read = 0x01,
			Write = 0x02,
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
			Float3x3,
			Float4x4,
			Sampler2D,
		};
	};

	class Uniforms;
	class Effects;

	class Effect;
	class UniformInfo;
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