#include "Uniforms.h"

namespace graphics
{
	UniformInfo Uniforms::_cameraPosition("cameraPosition", UniformType::Vector3);
	UniformInfo Uniforms::_projection("projection", UniformType::Matrix4);
	UniformInfo Uniforms::_texture("texture", UniformType::Sampler2D);
	UniformInfo Uniforms::_lightmap("lightmap", UniformType::Sampler2D);
	UniformInfo Uniforms::_environment("environment", UniformType::Sampler2D);
	UniformInfo Uniforms::_normalmap("normalmap", UniformType::Sampler2D);
	UniformInfo Uniforms::_time("time", UniformType::Float);	
	
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

	UniformInfo *Uniforms::normalmap()
	{
		return &_normalmap;
	}

	UniformInfo *Uniforms::time()
	{
		return &_time;
	}


	UniformValueVector3 UniformValues::_cameraPosition;
	UniformValueMatrix UniformValues::_projection;
	UniformValueTexture UniformValues::_texture;
	UniformValueTexture UniformValues::_lightmap;
	UniformValueTexture UniformValues::_environment;
	UniformValueTexture UniformValues::_normalmap;
	UniformValueFloat UniformValues::_time;



}