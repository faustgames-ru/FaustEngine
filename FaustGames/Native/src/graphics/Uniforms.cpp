#include "Uniforms.h"

namespace graphics
{
	UniformInfo Uniforms::_cameraPosition("cameraPosition", UniformType::Vector3);
	UniformInfo Uniforms::_projection("projection", UniformType::Matrix4);
	UniformInfo Uniforms::_texture("texture", UniformType::Sampler2D);
	UniformInfo Uniforms::_lightmap("lightmap", UniformType::Sampler2D);
	UniformInfo Uniforms::_environment("environment", UniformType::Sampler2D);
	UniformInfo Uniforms::_depthmap("depthmap", UniformType::Sampler2D);
	UniformInfo Uniforms::_normalmap("normalmap", UniformType::Sampler2D);
	UniformInfo Uniforms::_time("time", UniformType::Float);	
	UniformInfo Uniforms::_pixelSize("pixelSize", UniformType::Vector2);

	
	UniformInfo *Uniforms::cameraPosition()
	{
		return &_cameraPosition;
	}
	
	UniformInfo * Uniforms::projection()
	{
		return &_projection;
	}

	UniformInfo * Uniforms::texture()
	{
		return &_texture;
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

	UniformValueVector3 UniformValues::_cameraPosition;
	UniformValueMatrix UniformValues::_projection;
	UniformValueTexture UniformValues::_texture;
	UniformValueTexture UniformValues::_lightmap;
	UniformValueTexture UniformValues::_environment;
	UniformValueTexture UniformValues::_depthmap;
	UniformValueTexture UniformValues::_normalmap;
	UniformValueFloat UniformValues::_time;
	UniformValueVector2 UniformValues::_pixelSize;


	void UniformValues::initSamplers()
	{
		initSampler(&_texture, 0);
		initSampler(&_lightmap, 1);
		initSampler(&_environment, 2);
		initSampler(&_depthmap, 3);
		initSampler(&_normalmap, 4);
		for (int i = 5; i < GraphicsConstants::Samplers2DLimit; i++)
		{
			_samplers[i] = 0;
		}
	}
	
	void UniformValues::resetSamplers()
	{
		for (int i = 0; i < GraphicsConstants::Samplers2DLimit; i++)
		{
			if (_samplers[i])
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