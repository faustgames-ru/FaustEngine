#include "EffectsPostProcess.h"
#include "Uniforms.h"
#include "Attributes.h"
#include "../../shaders/post_process_vert.h"
#include "../../shaders/post_process_bloom_add_filter_frag.h"
#include "../../shaders/post_process_bloom_depth_add_filter.h"
#include "../../shaders/post_process_tonemapping.h"
#include "../../shaders/post_process_bloom_filter_frag.h"
#include "../../shaders/post_process_vblur_vert.h"
#include "../../shaders/post_process_hblur_vert.h"
#include "../../shaders/post_process_blur_frag.h"
#include "../../shaders/post_process_empty_frag.h"
#include "../../shaders/post_process_empty_color_frag.h"
#include "../../shaders/post_process_color_vert.h"
#include "../../shaders/post_process_mix_frag.h"
#include "../../shaders/post_process_blur_pass_vert.h"
#include "../../shaders/post_process_blur_pass_frag.h"


namespace graphics
{
	EffectPostProcessToneMapping::EffectPostProcessToneMapping()
	{
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::lightmap(), UniformValues::lightmap());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
	}

	EffectPostProcessToneMapping::~EffectPostProcessToneMapping()
	{
	}
	
	void EffectPostProcessToneMapping::create()
	{
		_effect.create(
			(char *)shader_post_process_vert,
			shader_post_process_vert_size,
			(char *)shader_post_process_tonemapping_frag,
			shader_post_process_tonemapping_frag_size);
	}

	EffectPostProcessBloomFilter::EffectPostProcessBloomFilter()
	{
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addConstant("threshold", graphics::UniformType::Float);
		_effect.addConstant("alpha", graphics::UniformType::Float);
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

	EffectPostProcessEmpty::EffectPostProcessEmpty()
	{
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
	}

	EffectPostProcessEmpty::~EffectPostProcessEmpty()
	{
	}

	void EffectPostProcessEmpty::create()
	{
		_effect.create(
			(char *)shader_post_process_vert,
			shader_post_process_vert_size,
			(char *)shader_post_process_empty_frag,
			shader_post_process_empty_frag_size);
	}

	EffectPostProcessEmptyColor::EffectPostProcessEmptyColor()
	{
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
		_effect.addAttribute(Attributes::color());
	}

	EffectPostProcessEmptyColor::~EffectPostProcessEmptyColor()
	{
	}

	void EffectPostProcessEmptyColor::create()
	{
		_effect.create(
			(char *)shader_post_process_color_vert,
			shader_post_process_color_vert_size,
			(char *)shader_post_process_empty_color_frag,
			shader_post_process_empty_color_frag_size);
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

	EffectPostProcessBloomDepthAddFilter::EffectPostProcessBloomDepthAddFilter()
	{
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::lightmap(), UniformValues::lightmap());
		_effect.addUniform(Uniforms::depthmap(), UniformValues::depthmap());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
	}

	EffectPostProcessBloomDepthAddFilter::~EffectPostProcessBloomDepthAddFilter()
	{
	}

	void EffectPostProcessBloomDepthAddFilter::create()
	{
		_effect.create(
			(char *)shader_post_process_vert,
			shader_post_process_vert_size,
			(char *)shader_post_process_bloom_depth_add_filter_frag,
			shader_post_process_bloom_depth_add_filter_frag_size);
	}

	EffectPostProcessMix::EffectPostProcessMix()
	{
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::lightmap(), UniformValues::lightmap());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
	}

	EffectPostProcessMix::~EffectPostProcessMix()
	{
	}

	void EffectPostProcessMix::create()
	{
		_effect.create(
			(char *)shader_post_process_vert,
			shader_post_process_vert_size,
			(char *)shader_post_process_mix_frag,
			shader_post_process_mix_frag_size);
	}

	EffectPostProcessBlurPass::EffectPostProcessBlurPass()
	{
		_effect.addUniform(Uniforms::texture(), UniformValues::texture());
		_effect.addUniform(Uniforms::pixelOffset(), UniformValues::pixelOffset());
		_effect.addAttribute(Attributes::position());
		_effect.addAttribute(Attributes::textureCoords());
	}

	EffectPostProcessBlurPass::~EffectPostProcessBlurPass()
	{
	}

	void EffectPostProcessBlurPass::create()
	{
		_effect.create(
			(char *)shader_post_process_blur_pass_vert,
			shader_post_process_blur_pass_vert_size,
			(char *)shader_post_process_blur_pass_frag,
			shader_post_process_blur_pass_frag_size);
	}
}