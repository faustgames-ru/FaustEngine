#include "Effects.h"
#include "Effect.h"
#include "UniformInfo.h"
#include "Uniforms.h"
#include "Attributes.h"

namespace graphics
{
	EffectSolid Effects::_solid;
	EffectSolidColor Effects::_solidColor;
	EffectTextureColor Effects::_textureColor;
	EffectTextureColorFog Effects::_textureColorFog;
	EffectTextureAlphamaskColor Effects::_textureAlphamaskColor;
	EffectTextureColorHighlight Effects::_textureColorHighlight;
	EffectTextureLightMapColor Effects::_textureLightmapColor;
	EffectTextureRgbTransform Effects::_textureRgbTransform;

	EffectPostProcessToneMapping Effects::_postProcessToneMapping;
	EffectPostProcessBloomFilter Effects::_postProcessBloomFilter;
	EffectPostProcessEmpty Effects::_postProcessEmpty;
	EffectPostProcessEmptyColor Effects::_postProcessEmptyColor;

	EffectPostProcessVBlurFilter Effects::_postProcessVBlurFilter;
	EffectPostProcessHBlurFilter Effects::_postProcessHBlurFilter;
	EffectPostProcessBloomAddFilter Effects::_postProcessBloomAddFilter;
	EffectPostProcessBlurPass Effects::_postProcessBlurPass;

	void Effects::create()
	{
        UniformValues::initSamplers();
        
		_solid.create();
		_solidColor.create();
		_textureColor.create();
		_textureColorFog.create();
		_textureAlphamaskColor.create();
		_textureColorHighlight.create();
		_textureLightmapColor.create();
		_textureRgbTransform.create();

		_postProcessToneMapping.create();
		_postProcessBloomFilter.create();
		_postProcessEmpty.create();
		_postProcessEmptyColor.create();
		_postProcessVBlurFilter.create();
		_postProcessHBlurFilter.create();
		_postProcessBlurPass.create();
		_postProcessBloomAddFilter.create();
	}

	void Effects::cleanup()
	{
		_solid.getEffect()->cleanup();
		_solidColor.getEffect()->cleanup();
		_textureColor.getEffect()->cleanup();
		_textureColorFog.getEffect()->cleanup();
		_textureAlphamaskColor.getEffect()->cleanup();
		_textureColorHighlight.getEffect()->cleanup();

		_textureLightmapColor.getEffect()->cleanup();
		_textureRgbTransform.getEffect()->cleanup();

		_postProcessBloomFilter.getEffect()->cleanup();
		_postProcessVBlurFilter.getEffect()->cleanup();
		_postProcessHBlurFilter.getEffect()->cleanup();
		_postProcessBlurPass.getEffect()->cleanup();

		_postProcessBloomAddFilter.getEffect()->cleanup();

	}

	EffectSolid * Effects::solid()
	{
		return &_solid;
	}

	EffectSolidColor * Effects::solidColor()
	{
		return &_solidColor;
	}

	EffectTextureColor * Effects::textureColor()
	{
		return &_textureColor;
	}

	EffectTextureColorFog* Effects::textureColorFog()
	{
		return &_textureColorFog;
	}

	EffectTextureAlphamaskColor * Effects::textureAlphamaskColor() 
	{
		return &_textureAlphamaskColor;
	}
	
	EffectTextureColorHighlight * Effects::textureColorHighlight()
	{
		return &_textureColorHighlight;
	}

	EffectTextureLightMapColor * Effects::textureLightmapColor()
	{
		return &_textureLightmapColor;
	}

	EffectTextureRgbTransform* Effects::textureRgbTransform()
	{
		return &_textureRgbTransform;
	}

	EffectPostProcessToneMapping * Effects::postProcessToneMapping()
	{
		return &_postProcessToneMapping;
	}

	EffectPostProcessBloomFilter * Effects::postProcessBloomFilter()
	{
		return &_postProcessBloomFilter;
	}

	EffectPostProcessEmpty* Effects::postProcessEmpty()
	{
		return &_postProcessEmpty;
	}

	EffectPostProcessEmptyColor* Effects::postProcessEmptyColor()
	{
		return &_postProcessEmptyColor;
	}

	EffectPostProcessVBlurFilter * Effects::postProcessVBlurFilter()
	{
		return &_postProcessVBlurFilter;
	}

	EffectPostProcessHBlurFilter * Effects::postProcessHBlurFilter()
	{
		return &_postProcessHBlurFilter;
	}

	EffectPostProcessBlurPass* Effects::postProcessBlurPassFilter()
	{
		return &_postProcessBlurPass;
	}

	EffectPostProcessBloomAddFilter * Effects::postProcessBloomAddFilter()
	{
		return &_postProcessBloomAddFilter;
	}

}