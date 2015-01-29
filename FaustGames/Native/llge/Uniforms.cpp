#include "Uniforms.h"
#include "Lazy.h"

namespace graphics
{
	Uniforms::Uniforms() :
		_projection("projection", UniformType::Float4x4),
		_texture("projection", UniformType::Sampler2D),
		_shadowmap("projection", UniformType::Sampler2D)
	{
	}

	Uniforms::~Uniforms()
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
	
	Uniforms * Uniforms::instanciate()
	{
		return new Uniforms();
	}
	void Uniforms::deinstanciate(Uniforms *value)
	{
		delete value;
	}

	core::Lazy<Uniforms> Uniforms::_instance(Uniforms::instanciate, Uniforms::deinstanciate);
	
	Uniforms * Uniforms::instance()
	{		
		return _instance.value();
	}
}