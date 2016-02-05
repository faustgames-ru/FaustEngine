#include "EffectTextureAlphamaskColor.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "../../shaders/texture_alphamask_color_frag.h"
#include "../../shaders/texture_alphamask_color_vert.h"

namespace graphics
{
	EffectTextureAlphamaskColor::EffectTextureAlphamaskColor()
	{
		_configSize = sizeof(llge::EffectConfig);
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::lightmap(), UniformValues::lightmap());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addAttribute(Attributes::color());
	}
	EffectTextureAlphamaskColor::~EffectTextureAlphamaskColor()
	{
	}

	void EffectTextureAlphamaskColor::create()
	{
		_effect.create((char *)shader_texture_alphamask_color_vert, shader_texture_alphamask_color_vert_size, (char *)shader_texture_alphamask_color_frag, shader_texture_alphamask_color_frag_size);
	}

	void EffectTextureAlphamaskColor::configApply(const void* config)
	{
		const llge::EffectConfig* ec = static_cast<const llge::EffectConfig*>(config);
		graphics::UniformValues::texture()->setValue(ec->texture);
	}
}
