#include "EffectTextureColor.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "..\..\shaders\texture_color_frag.h"
#include "..\..\shaders\texture_color_vert.h"

namespace graphics
{
	EffectTextureColor::EffectTextureColor()
	{
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addAttribute(Attributes::color());
	}
	EffectTextureColor::~EffectTextureColor()
	{
	}

	void EffectTextureColor::create()
	{
		_effect.create((char *)shader_texture_color_vert, shader_texture_color_vert_size, (char *)shader_texture_color_frag, shader_texture_color_frag_size);
	}
}