#include "Uniforms.h"

namespace graphics
{
	UniformInfo Uniforms::_cameraPosition("cameraPosition", UniformType::Vector3);
	UniformInfo Uniforms::_projection("projection", UniformType::Matrix4);
	UniformInfo Uniforms::_colorTransform("colorTransform", UniformType::Matrix3);
	UniformInfo Uniforms::_texture("texture", UniformType::Sampler2D);
	UniformInfo Uniforms::_alpha("alpha", UniformType::Sampler2D);
	UniformInfo Uniforms::_lightmap("lightmap", UniformType::Sampler2D);
	UniformInfo Uniforms::_environment("environment", UniformType::Sampler2D);
	UniformInfo Uniforms::_depthmap("depthmap", UniformType::Sampler2D);
	UniformInfo Uniforms::_normalmap("normalmap", UniformType::Sampler2D);
	UniformInfo Uniforms::_time("time", UniformType::Float);	
	UniformInfo Uniforms::_pixelSize("pixelSize", UniformType::Vector2);
	UniformInfo Uniforms::_pixelOffset("pixelOffset", UniformType::Vector2);
	UniformInfo Uniforms::_highlightColor("highlightColor", UniformType::Vector3);
	UniformInfo Uniforms::_paintmask("paintmask", UniformType::Sampler2D);

	UniformInfo Uniforms::_fogStart("fogStart", UniformType::Float);
	UniformInfo Uniforms::_fogDensity("fogDensity", UniformType::Float);
	UniformInfo Uniforms::_fogScale("fogScale", UniformType::Float);
	UniformInfo Uniforms::_fogColor("fogColor", UniformType::Vector3);


	
	UniformInfo *Uniforms::cameraPosition()
	{
		return &_cameraPosition;
	}
	
	UniformInfo * Uniforms::projection()
	{
		return &_projection;
	}

	UniformInfo * Uniforms::colorTransform()
	{
		return &_colorTransform;
	}


	UniformInfo * Uniforms::texture()
	{
		return &_texture;
	}

	UniformInfo* Uniforms::alpha()
	{
		return &_alpha;
	}

	UniformInfo * Uniforms::lightmap()
	{
		return &_lightmap;
	}	
	UniformInfo * Uniforms::environment()
	{
		return &_environment;
	}

	UniformInfo *Uniforms::depthmap()
	{
		return &_depthmap;
	}
	
	UniformInfo *Uniforms::normalmap()
	{
		return &_normalmap;
	}

	UniformInfo *Uniforms::time()
	{
		return &_time;
	}
	
	UniformInfo *Uniforms::pixelSize()
	{
		return &_pixelSize;
	}

	UniformInfo* Uniforms::pixelOffset()
	{
		return &_pixelOffset;
	}

	UniformInfo* Uniforms::highlightColor()
	{
		return &_highlightColor;
	}

	UniformInfo* Uniforms::fogStart()
	{
		return &_fogStart;
	}

	UniformInfo* Uniforms::fogDensity()
	{
		return &_fogDensity;
	}

	UniformInfo* Uniforms::fogColor()
	{
		return &_fogColor;
	}

	UniformInfo* Uniforms::fogScale()
	{
		return &_fogScale;
	}

	UniformInfo* Uniforms::paintmask()
	{
		return &_paintmask;
	}

	UniformValueVector3 UniformValues::_cameraPosition;
	UniformValueMatrix UniformValues::_projection;
	UniformValueMatrix3 UniformValues::_colorTransform;
	UniformValueTexture UniformValues::_texture;
	UniformValueTexture UniformValues::_alpha;
	UniformValueTexture UniformValues::_lightmap;
	UniformValueTexture UniformValues::_environment;
	UniformValueTexture UniformValues::_depthmap;
	UniformValueTexture UniformValues::_normalmap;
	UniformValueFloat UniformValues::_time;
	UniformValueVector2 UniformValues::_pixelSize;
	UniformValueVector2 UniformValues::_pixelOffset;	
	UniformValueVector3 UniformValues::_highlightColor;
	UniformValueFloat UniformValues::_fogStart;
	UniformValueFloat UniformValues::_fogDensity;
	UniformValueFloat UniformValues::_fogScale;
	UniformValueVector3 UniformValues::_fogColor;
	UniformValueTexture UniformValues::_paintmask;



	void UniformValues::initSamplers()
	{
		for (int i = 0; i < GraphicsConstants::Samplers2DLimit; i++)
		{
			_samplers[i] = nullptr;
		}

		initSampler(&_texture, 0);
		initSampler(&_alpha, 1);
		initSampler(&_lightmap, 2);
		//initSampler(&_environment, 2);
		initSampler(&_depthmap, 3);
		initSampler(&_normalmap, 4);
		initSampler(&_paintmask, 5);
	}
	
	void UniformValues::resetSamplers()
	{
		for (int i = 0; i < GraphicsConstants::Samplers2DLimit; i++)
		{
			if (_samplers[i] != nullptr)
				_samplers[i]->reset();
		}
	}

	void UniformValues::initSampler(UniformValueTexture *sampler, int index)
	{
		sampler->setSamplerIndex(index);
		_samplers[index] = sampler;
	}

	UniformValueTexture* UniformValues::_samplers[GraphicsConstants::Samplers2DLimit];
}