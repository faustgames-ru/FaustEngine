#ifndef GRAPHICS_H
#define GRAPHICS_H

namespace graphics
{
	class Effect
	{
	public:
	};

	class EffectParameters
	{
	public:
	};

	class VertexDeclaration
	{

	};

	class Texture
	{
	public:
	};

	class RenderTarget
	{
	public:
	};

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

	class RenderState 
	{
	public:
		BlendState::e Blend;
		DepthState::e Depth;
		Effect * Effect;
		EffectParameters* EffectParameters;
		VertexDeclaration * VertexDeclaration;
	};

	class GraphicsDevice
	{
	public:
		void setViewPort(float x, float y, float width, float height);
		void setRenderTarget(RenderTarget *renderPath);
		void clear(unsigned int color, float depth);
		void drawPrimitives(RenderState *renderState, void *vertexBuffer, unsigned short *indexBuffer, int primitivesCount);
	};
}

#endif /*GRAPHICS_H*/