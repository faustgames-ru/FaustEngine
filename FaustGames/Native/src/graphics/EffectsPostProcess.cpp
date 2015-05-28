#include "EffectsPostProcess.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "..\..\shaders\post_process_vert.h"
#include "..\..\shaders\post_process_bloom_add_filter_frag.h"
#include "..\..\shaders\post_process_bloom_filter_frag.h"
#include "..\..\shaders\post_process_vblur_vert.h"
#include "..\..\shaders\post_process_hblur_vert.h"
#include "..\..\shaders\post_process_blur_frag.h"


namespace graphics
{
	EffectPostProcessBloomFilter::EffectPostProcessBloomFilter()
	{
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
	}

	EffectPostProcessBloomFilter::~EffectPostProcessBloomFilter()
	{
	}

	void EffectPostProcessBloomFilter::create()
	{
		_effect.create(
			(char *)shader_post_process_vert, 
			shader_post_process_vert_size, 
			(char *)shader_post_process_bloom_filter_frag, 
			shader_post_process_bloom_filter_frag_size);
	}

	EffectPostProcessVBlurFilter::EffectPostProcessVBlurFilter()
	{
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::pixelSize(), UniformValues::pixelSize());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
	}
	
	EffectPostProcessVBlurFilter::~EffectPostProcessVBlurFilter()
	{
	}
	
	void EffectPostProcessVBlurFilter::create()
	{
		_effect.create(
			(char *)shader_post_process_vblur_vert,
			shader_post_process_vblur_vert_size,
			(char *)shader_post_process_blur_frag,
			shader_post_process_blur_frag_size);
	}

	EffectPostProcessHBlurFilter::EffectPostProcessHBlurFilter()
	{
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::pixelSize(), UniformValues::pixelSize());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
	}

	EffectPostProcessHBlurFilter::~EffectPostProcessHBlurFilter()
	{
	}

	void EffectPostProcessHBlurFilter::create()
	{
		_effect.create(
			(char *)shader_post_process_hblur_vert,
			shader_post_process_hblur_vert_size,
			(char *)shader_post_process_blur_frag,
			shader_post_process_blur_frag_size);
	}

	EffectPostProcessBloomAddFilter::EffectPostProcessBloomAddFilter()
	{
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::lightmap(), UniformValues::lightmap());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
	}

	EffectPostProcessBloomAddFilter::~EffectPostProcessBloomAddFilter()
	{
	}

	void EffectPostProcessBloomAddFilter::create()
	{
		_effect.create(
			(char *)shader_post_process_vert,
			shader_post_process_vert_size,
			(char *)shader_post_process_bloom_add_filter_frag,
			shader_post_process_bloom_add_filter_frag_size);
	}

}