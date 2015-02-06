#include "Uniforms.h"

namespace graphics
{
	UniformInfo Uniforms::_projection("projection", UniformType::Matrix4);
	UniformInfo Uniforms::_texture("texture", UniformType::Sampler2D);
	UniformInfo Uniforms::_shadowmap("shadowmap", UniformType::Sampler2D);

	UniformInfo * Uniforms::projection()
	{
		return &_projection;
	}
	UniformInfo * Uniforms::texture()
	{
		return &_texture;
	}
	UniformInfo * Uniforms::shadowmap()
	{
		return &_shadowmap;
	}	

	UniformValueMatrix UniformValues::_projection;
	UniformValueTexture UniformValues::_texture;
	UniformValueTexture UniformValues::_shadowmap;

}