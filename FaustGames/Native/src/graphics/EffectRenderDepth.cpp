#include "EffectRenderDepth.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "../../shaders/shader_render_depth_frag.h"
#include "../../shaders/shader_render_depth_vert.h"

namespace graphics
{
	EffectRenderDepth::EffectRenderDepth()
	{
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
	}
	EffectRenderDepth::~EffectRenderDepth()
	{
	}

	void EffectRenderDepth::create()
	{
		_effect.create((char *)shader_render_depth_vert, shader_render_depth_vert_size, (char *)shader_render_depth_frag, shader_render_depth_frag_size);
	}
}