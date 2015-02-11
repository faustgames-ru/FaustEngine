#include "EffectSolid.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "..\..\shaders\solid_frag.h"
#include "..\..\shaders\solid_vert.h"


namespace graphics
{
	EffectSolid::EffectSolid()
	{
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addAttribute(Attributes::position());
	}

	EffectSolid::~EffectSolid()
	{
	}

	void EffectSolid::create()
	{
		_effect.create((char *)shader_solid_vert, shader_solid_vert_size, (char *)shader_solid_frag, shader_solid_frag_size);
	}
}