#include "EffectSolidColor.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "..\..\shaders\solid_color_frag.h"
#include "..\..\shaders\solid_color_vert.h"

namespace graphics
{
	EffectSolidColor::EffectSolidColor()
	{
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::color());
	}

	EffectSolidColor::~EffectSolidColor()
	{
	}

	void EffectSolidColor::create()
	{
		_effect.create((char *)shader_solid_color_vert, shader_solid_color_vert_size, (char *)shader_solid_color_frag, shader_solid_color_frag_size);
	}
}