#include "EffectTextureColor.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "../../shaders/texture_color_frag.h"
#include "../../shaders/texture_color_vert.h"
#include "../../shaders/texture_color_fog_frag.h"
#include "../../shaders/texture_color_fog_vert.h"
#include "../../shaders/texture_blurmap_color_frag.h";
#include "../../shaders/texture_blurmap_color_vert.h";
#include "../../shaders/texture_color_hsv_fog_vert.h"
#include "../../shaders/texture_color_hsv_fog_frag.h"
#include "../../shaders/texture_color_frag_paint.h"
#include "../../shaders/texture_color_vert_paint.h"

namespace graphics
{
	EffectTexturePackedColor::EffectTexturePackedColor()
	{		
		_configSize = sizeof(llge::EffectConfig);
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoordsPacked());
		_effect.addAttribute(Attributes::color());
	}

	EffectTexturePackedColor::~EffectTexturePackedColor()
	{
	}

	void EffectTexturePackedColor::create()
	{
		_effect.create((char *)shader_texture_color_vert, shader_texture_color_vert_size, (char *)shader_texture_color_frag, shader_texture_color_frag_size);
	}

	void EffectTexturePackedColor::configApply(const void* config)
	{
		const llge::EffectConfig* ec = static_cast<const llge::EffectConfig*>(config);
		graphics::UniformValues::texture()->setValue(ec->texture);
	}

	EffectTextureColor::EffectTextureColor()
	{
		_configSize = sizeof(llge::EffectConfig);
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

	void EffectTextureColor::configApply(const void* config)
	{
		const llge::EffectConfig* ec = static_cast<const llge::EffectConfig*>(config);
		graphics::UniformValues::texture()->setValue(ec->texture);		
	}

	EffectTextureColorFog::EffectTextureColorFog()
	{
		_configSize = sizeof(llge::EffectConfig);
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::fogStart(), UniformValues::fogStart());
		_effect.addUniform(Uniforms::fogDensity(), UniformValues::fogDensity());
		_effect.addUniform(Uniforms::fogColor(), UniformValues::fogColor());
		_effect.addUniform(Uniforms::fogScale(), UniformValues::fogScale());

		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addAttribute(Attributes::color());
	}

	EffectTextureColorFog::~EffectTextureColorFog()
	{
	}

	void EffectTextureColorFog::create()
	{
		_effect.create((char *)shader_texture_color_fog_vert, shader_texture_color_fog_vert_size, (char *)shader_texture_color_fog_frag, shader_texture_color_fog_frag_size);
	}

	void EffectTextureColorFog::configApply(const void* config)
	{
		const llge::EffectConfig* ec = static_cast<const llge::EffectConfig*>(config);
		graphics::UniformValues::texture()->setValue(ec->texture);
	}

	EffectTextureColorHsvFog::EffectTextureColorHsvFog()
	{
		_configSize = sizeof(llge::EffectConfig);
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::fogStart(), UniformValues::fogStart());
		_effect.addUniform(Uniforms::fogDensity(), UniformValues::fogDensity());
		_effect.addUniform(Uniforms::fogColor(), UniformValues::fogColor());
		_effect.addUniform(Uniforms::fogScale(), UniformValues::fogScale());

		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addAttribute(Attributes::color());
	}

	EffectTextureColorHsvFog::~EffectTextureColorHsvFog()
	{
	}

	void EffectTextureColorHsvFog::create()
	{
		_effect.create((char *)shader_texture_color_hsv_fog_vert, shader_texture_color_hsv_fog_vert_size, (char *)shader_texture_color_hsv_fog_frag, shader_texture_color_hsv_fog_frag_size);
	}

	void EffectTextureColorHsvFog::configApply(const void* config)
	{
		const llge::EffectConfig* ec = static_cast<const llge::EffectConfig*>(config);
		graphics::UniformValues::texture()->setValue(ec->texture);
	}

	uint EffectTextureBlurmap::blurmap(0);


	EffectTextureBlurmap::EffectTextureBlurmap()
	{
		_configSize = sizeof(llge::EffectConfig);
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::lightmap(), UniformValues::lightmap());

		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addAttribute(Attributes::color());
	}

	EffectTextureBlurmap::~EffectTextureBlurmap()
	{
	}

	void EffectTextureBlurmap::create()
	{
		_effect.create((char *)shader_texture_blurmap_color_vert, shader_texture_blurmap_color_vert_size, (char *)shader_texture_blurmap_color_frag, shader_texture_blurmap_color_frag_size);
	}

	void EffectTextureBlurmap::configApply(const void* config)
	{
		const llge::EffectConfig* ec = static_cast<const llge::EffectConfig*>(config);
		graphics::UniformValues::texture()->setValue(ec->texture);
		graphics::UniformValues::lightmap()->setValue(blurmap);
	}

	EffectTexturePaint::EffectTexturePaint()
	{
		_configSize = sizeof(llge::LightingConfig);

		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::paintmask(), UniformValues::paintmask());

		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addAttribute(Attributes::color());
	}

	EffectTexturePaint::~EffectTexturePaint()
	{
	}

	void EffectTexturePaint::create()
	{
		_effect.create((char *)shader_texture_color_paint_vert, shader_texture_color_paint_vert_size, (char *)shader_texture_color_paint_frag, shader_texture_color_paint_frag_size);
	}

	void EffectTexturePaint::configApply(const void* config)
	{
		const llge::LightingConfig* ec = static_cast<const llge::LightingConfig*>(config);
		graphics::UniformValues::texture()->setValue(ec->texture);
		graphics::UniformValues::paintmask()->setValue(ec->lightmap);
	}
}
