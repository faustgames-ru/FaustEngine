#include "EffectTextureColorHighlight.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "../../shaders/texture_color_highlight_frag.h"
#include "../../shaders/texture_color_highlight_vert.h"

namespace graphics
{
	EffectTextureColorHighlight::EffectTextureColorHighlight()
	{
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::time(), UniformValues::time());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addAttribute(Attributes::color());
	}
	EffectTextureColorHighlight::~EffectTextureColorHighlight()
	{
	}

	void EffectTextureColorHighlight::create()
	{
		_effect.create((char *)shader_texture_color_highlight_vert, shader_texture_color_highlight_vert_size, (char *)shader_texture_color_highlight_frag, shader_texture_color_highlight_frag_size);
	}
}