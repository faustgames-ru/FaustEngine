#include "EffectTextureLightMapColor.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "../../shaders/texture_lightmap_color_frag.h"
#include "../../shaders/texture_lightmap_color_vert.h"

namespace graphics
{
	EffectTextureLightMapColor::EffectTextureLightMapColor()
	{
		_configSize = sizeof(llge::LightingConfig);
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::lightmap(), UniformValues::lightmap());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addAttribute(Attributes::color());
	}
	EffectTextureLightMapColor::~EffectTextureLightMapColor()
	{
	}

	void EffectTextureLightMapColor::create()
	{
		_effect.create((char *)shader_texture_lightmap_color_vert, shader_texture_lightmap_color_vert_size, (char *)shader_texture_lightmap_color_frag, shader_texture_lightmap_color_frag_size);
	}

	void EffectTextureLightMapColor::configApply(const void* config)
	{
		const llge::LightingConfig* ec = static_cast<const llge::LightingConfig*>(config);
		graphics::UniformValues::texture()->setValue(ec->texture);
		graphics::UniformValues::lightmap()->setValue(ec->lightmap);
	}
}
