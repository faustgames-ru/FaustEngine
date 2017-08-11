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

	class TonemapFilter
	{
	public:
		virtual void execute(graphics::Texture* source, graphics::IRenderTarget *target, uint tonemapId);
	};

	class ColorTransformFilter
	{
	public:
		core::Matrix3Container _colorTransform;
		virtual void execute(graphics::Texture* source, const core::Vector3 &offset, const core::Matrix3 &colorTransform, graphics::IRenderTarget *target);
	};

	class BloomFilter
	{
	public:
		virtual void execute(graphics::Texture* source, graphics::IRenderTarget *target);
	};

	class EmptyProcess
	{
	public:
		virtual void execute(graphics::Texture* source, graphics::IRenderTarget *target);
	};

	class VignettingProcess
	{
	public:
		virtual void execute(graphics::Texture* source);
	private:
		void addVertex(core::Vector2 v);
		std::vector<Mesh2dVertex> _vertices;
		std::vector<ushort> _indices;
	};

	class FilterVBlur
	{
	public:
		virtual void execute(graphics::Texture* source, graphics::IRenderTarget *target);
	};

	
	class FilterHBlur 
	{
	public:
		virtual void execute(graphics::Texture* source, graphics::IRenderTarget *target);
	};


	class Refractor
	{
	public:
		Refractor();
		void update();
		std::vector<PostProcessVertex> _refractVertices;
		std::vector<ushort> _refractIndices;
	private:
		core::Vector2 getNormal(float u, float v);
		core::Vector2 _offset;
		core::Vector2 _velocity;
		static const int size = 16;
		PostProcessVertex _vertices[size][size];
		core::Vector2 _normals[size][size];
	};

	class FilterAdd
	{
	public:
		Refractor refractor;
		virtual void execute(graphics::Texture* value0, graphics::Texture* value1, graphics::IRenderTarget *target);
	};

	class PostProcessBloom
	{
	public:
		PostProcessBloom();
		virtual ~PostProcessBloom()
		{
		}

		virtual void beginRender(uint tonemapId);
		virtual void finishRender();
		bool isAvaliable();
		graphics::Texture* getBlurMap();
	private:
		ColorTransformFilter _colorFilter;
		BloomFilter _filter;		
		EmptyProcess _empty;
		FilterVBlur _vBlur;
		FilterHBlur _hBlur;		
		FilterAdd _add;
		VignettingProcess _vignetting;
		TonemapFilter _tonemap;
		uint _tonemapId;
		graphics::IPostProcessTarget *_source;
		graphics::IRenderTarget *_beginTarget;
		graphics::IPostProcessTarget *_blurMap;

		/*
		graphics::TextureRenderTarget2d *_target0;
		graphics::TextureRenderTarget2d *_target1;
		*/
	};

	PostProcessVertex* quadVertices();
	ushort* quadIndices();
}

#endif /*POSTPROCESS_H*/
