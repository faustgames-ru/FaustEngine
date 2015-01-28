#include "Uniforms.h"
#include "UniformInfo.h"

namespace graphics
{
	Uniforms::Uniforms() :
		_projection("projection", UniformType::Float4x4),
		_texture("projection", UniformType::Sampler2D),
		_shadowmap("projection", UniformType::Sampler2D)
	{
	}

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
	
	Uniforms * Uniforms::_instance(0);

	Uniforms * Uniforms::instance()
	{
		if (!_instance)
			_instance = new Uniforms();
		return _instance;
	}
}