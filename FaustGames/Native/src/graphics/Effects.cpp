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
	EffectTextureColorHsvFog Effects::_textureColorHsvFog;
	EffectTextureAlphamaskColor Effects::_textureAlphamaskColor;
	EffectTextureColorHighlight Effects::_textureColorHighlight;
	EffectTextureLightMapColor Effects::_textureLightmapColor;
	EffectTextureBlurColor Effects::_textureBlurColor;
	EffectTextureBlurmap Effects::_textureBlurmap;
	EffectTexturePaint Effects::_texturePaint;

	//EffectWater Effects::_water;
	EffectRenderDepth Effects::_renderDepth;
	EffectPostProcessToneMapping Effects::_postProcessToneMapping;
	EffectPostProcessBloomFilter Effects::_postProcessBloomFilter;
	EffectPostProcessEmpty Effects::_postProcessEmpty;
	EffectPostProcessEmptyColor Effects::_postProcessEmptyColor;

	EffectPostProcessVBlurFilter Effects::_postProcessVBlurFilter;
	EffectPostProcessHBlurFilter Effects::_postProcessHBlurFilter;
	EffectPostProcessBloomAddFilter Effects::_postProcessBloomAddFilter;

	void Effects::create()
	{
        UniformValues::initSamplers();
        
		_solid.create();
		_solidColor.create();
		_textureColor.create();
		_textureColorFog.create();
		_textureColorHsvFog.create();
		_textureAlphamaskColor.create();
		_textureColorHighlight.create();
		_textureLightmapColor.create();
		_textureBlurColor.create();
		_textureBlurmap.create();
		_texturePaint.create();
		//_water.create();
		_renderDepth.create();
		_postProcessToneMapping.create();
		_postProcessBloomFilter.create();
		_postProcessEmpty.create();
		_postProcessEmptyColor.create();
		_postProcessVBlurFilter.create();
		_postProcessHBlurFilter.create();
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
		_textureBlurColor.getEffect()->cleanup();
		_textureBlurmap.getEffect()->cleanup();
		_texturePaint.getEffect()->cleanup();
		//_water.getEffect()->cleanup();
		_renderDepth.getEffect()->cleanup();
		_postProcessBloomFilter.getEffect()->cleanup();
		_postProcessVBlurFilter.getEffect()->cleanup();
		_postProcessHBlurFilter.getEffect()->cleanup();
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

	EffectTextureColorHsvFog* Effects::textureColorHsvFog()
	{
		return &_textureColorHsvFog;
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

	EffectTextureBlurColor* Effects::textureBlurColor()
	{
		return &_textureBlurColor;
	}

	EffectTextureBlurmap* Effects::textureBlurmap()
	{
		return &_textureBlurmap;
	}

	EffectTexturePaint* Effects::texturePaint()
	{
		return &_texturePaint;
	}

	/*
	EffectWater * Effects::water()
	{
		return &_water;
	}
	*/
	EffectRenderDepth * Effects::renderDepth()
	{
		return &_renderDepth;
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

	EffectPostProcessBloomAddFilter * Effects::postProcessBloomAddFilter()
	{
		return &_postProcessBloomAddFilter;
	}

}