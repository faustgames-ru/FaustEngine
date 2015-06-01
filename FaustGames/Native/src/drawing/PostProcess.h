#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include "drawing_classes.h"

namespace drawing
{
	struct PostProcessVertex
	{
		float X;
		float Y;
		float Z;
		float U;
		float V;
		PostProcessVertex(){}
		PostProcessVertex(
			float x,
			float y,
			float z,
			float u,
			float v)
		{
			X = x;
			Y = y;
			Z = z;
			U = u;
			V = v;
		}
		~PostProcessVertex(){}
	};

	class PostProcess
	{
	public:
		virtual void execute(graphics::TextureRenderTarget2d *source) = 0;
	private:
	};

	class PostProcessExecutor
	{
	public:
		virtual void execute(graphics::TextureRenderTarget2d *source, graphics::TextureRenderTarget2d *target) = 0;
	};

	class BloomFilter
	{
	public:
		virtual void execute(graphics::TextureRenderTarget2d *source, graphics::TextureRenderTarget2d *target, uint tonemapId);
	};

	class FilterVBlur : public PostProcessExecutor
	{
	public:
		virtual void execute(graphics::TextureRenderTarget2d *source, graphics::TextureRenderTarget2d *target);
	};

	
	class FilterHBlur : public PostProcessExecutor
	{
	public:
		virtual void execute(graphics::TextureRenderTarget2d *source, graphics::TextureRenderTarget2d *target);
	};


	class FilterAdd
	{
	public:
		virtual void execute(graphics::TextureRenderTarget2d *value0, graphics::TextureRenderTarget2d *value1, graphics::TextureRenderTarget2d *target);
	};
	
	class PostProcessBloom
	{
	public:
		virtual void execute(graphics::TextureRenderTarget2d *source, uint tonemapId);
	private:
		BloomFilter _filter;		
		FilterVBlur _vBlur;		
		FilterHBlur _hBlur;		
		FilterAdd _add;
		
		graphics::TextureRenderTarget2d *_target0;
		graphics::TextureRenderTarget2d *_target1;
	};
}

#endif /*POSTPROCESS_H*/
