#include "EffectTextureColor.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "../../shaders/texture_color_frag.h"
#include "../../shaders/texture_color_rgb_transform_frag.h"
#include "../../shaders/texture_color_vert.h"
#include "../../shaders/texture_color_fog_frag.h"
#include "../../shaders/texture_color_fog_vert.h"
#include "../../shaders/texture_color_etc1a8_frag.h"


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

	EffectTextureColorEtc1A8::EffectTextureColorEtc1A8()
	{
		_configSize = sizeof(llge::EffectConfig);
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::alpha(), UniformValues::alpha());

		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addAttribute(Attributes::color());
	}

	EffectTextureColorEtc1A8::~EffectTextureColorEtc1A8()
	{
	}

	void EffectTextureColorEtc1A8::create()
	{
		_effect.create((char *)shader_texture_color_vert, shader_texture_color_vert_size, (char *)shader_texture_color_etc1a8_frag, shader_texture_color_etc1a8_frag_size);
	}

	void EffectTextureColorEtc1A8::configApply(const void* config)
	{
		const llge::EffectConfig* ec = static_cast<const llge::EffectConfig*>(config);
		graphics::UniformValues::texture()->setValue(ec->texture);
		graphics::UniformValues::alpha()->setValue(ec->alpha);
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
	
	EffectTextureRgbTransform::EffectTextureRgbTransform()
	{
		_configSize = sizeof(llge::EffectConfig);
		_effect.addUniform(Uniforms::projection(), UniformValues::projection());
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::colorTransform(), UniformValues::colorTransform());

		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addAttribute(Attributes::color());
	}

	EffectTextureRgbTransform::~EffectTextureRgbTransform()
	{
	}

	void EffectTextureRgbTransform::create()
	{
		_effect.create((char *)shader_texture_color_vert, shader_texture_color_vert_size, (char *)shader_texture_color_rgb_transform_frag, shader_texture_color_rgb_transform_frag_size);
	}

	void EffectTextureRgbTransform::configApply(const void* config)
	{
		const llge::EffectConfig* ec = static_cast<const llge::EffectConfig*>(config);
		graphics::UniformValues::texture()->setValue(ec->texture);
	}
}
