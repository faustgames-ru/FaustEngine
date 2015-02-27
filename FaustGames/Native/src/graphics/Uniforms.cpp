#include "Uniforms.h"

namespace graphics
{
	UniformInfo Uniforms::_projection("projection", UniformType::Matrix4);
	UniformInfo Uniforms::_texture("texture", UniformType::Sampler2D);
	UniformInfo Uniforms::_lightmap("lightmap", UniformType::Sampler2D);
	UniformInfo Uniforms::_environment("environment", UniformType::SamplerCube);

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

	UniformValueMatrix UniformValues::_projection;
	UniformValueTexture UniformValues::_texture;
	UniformValueTexture UniformValues::_lightmap;
	UniformValueCubemap UniformValues::_environment;

}