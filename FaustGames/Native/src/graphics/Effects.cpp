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
	EffectTextureColorEtc1A8 Effects::_textureColorEtc1A8;
	EffectTextureColorFog Effects::_textureColorFog;
	EffectTextureColorFogEtc1A8 Effects::_textureColorFogEtc1A8;
	EffectTextureAlphamaskColor Effects::_textureAlphamaskColor;
	EffectTextureColorHighlight Effects::_textureColorHighlight;
	//EffectTextureLightMapColor Effects::_textureLightmapColor;
	EffectTextureRgbTransformEtc1A8 Effects::_textureRgbTransformEtc1A8;
	EffectTextureRgbTransform Effects::_textureRgbTransform;

	EffectPostProcessToneMapping Effects::_postProcessToneMapping;
	EffectPostProcessBloomFilter Effects::_postProcessBloomFilter;
	EffectPostProcessEmpty Effects::_postProcessEmpty;
	EffectPostProcessEmptyColor Effects::_postProcessEmptyColor;

	EffectPostProcessVBlurFilter Effects::_postProcessVBlurFilter;
	EffectPostProcessHBlurFilter Effects::_postProcessHBlurFilter;
	EffectPostProcessBloomAddFilter Effects::_postProcessBloomAddFilter;
	EffectPostProcessBlurPass Effects::_postProcessBlurPass;
	EffectPostProcessRgbTransform Effects::_postProcessRgbTransform;

	void Effects::create()
	{
        UniformValues::initSamplers();
        
		_solid.create();
		_solidColor.create();
		_textureColor.create();
		_textureColorEtc1A8.create();
		
		_textureColorFog.create();
		_textureColorFogEtc1A8.create();

		_textureAlphamaskColor.create();
		_textureColorHighlight.create();
		
		_textureRgbTransform.create();
		_textureRgbTransformEtc1A8.create();

		_postProcessToneMapping.create();
		_postProcessBloomFilter.create();
		_postProcessEmpty.create();
		_postProcessEmptyColor.create();
		_postProcessVBlurFilter.create();
		_postProcessHBlurFilter.create();
		_postProcessBlurPass.create();
		_postProcessBloomAddFilter.create();
		_postProcessRgbTransform.create();
	}

	void Effects::cleanup()
	{
		_solid.getEffect()->cleanup();
		_solidColor.getEffect()->cleanup();
		_textureColor.getEffect()->cleanup();
		_textureColorEtc1A8.getEffect()->cleanup();
		_textureColorFog.getEffect()->cleanup();		
		_textureColorFogEtc1A8.getEffect()->cleanup();

		_textureAlphamaskColor.getEffect()->cleanup();
		_textureColorHighlight.getEffect()->cleanup();

		//_textureLightmapColor.getEffect()->cleanup();
		_textureRgbTransform.getEffect()->cleanup();
		_textureRgbTransformEtc1A8.getEffect()->cleanup();

		_postProcessBloomFilter.getEffect()->cleanup();
		_postProcessVBlurFilter.getEffect()->cleanup();
		_postProcessHBlurFilter.getEffect()->cleanup();
		_postProcessBlurPass.getEffect()->cleanup();

		_postProcessBloomAddFilter.getEffect()->cleanup();
		_postProcessRgbTransform.getEffect()->cleanup();

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

	EffectTextureColorEtc1A8* Effects::textureColorEtc1A8()
	{
		return &_textureColorEtc1A8;
	}

	EffectTextureColorFog* Effects::textureColorFog()
	{
		return &_textureColorFog;
	}

	EffectTextureColorFogEtc1A8* Effects::textureColorFogEtc1A8()
	{
		return &_textureColorFogEtc1A8;
	}

	EffectTextureAlphamaskColor * Effects::textureAlphamaskColor() 
	{
		return &_textureAlphamaskColor;
	}
	
	EffectTextureColorHighlight * Effects::textureColorHighlight()
	{
		return &_textureColorHighlight;
	}

	//EffectTextureLightMapColor * Effects::textureLightmapColor()
	//{
	//	return &_textureLightmapColor;
	//}

	EffectTextureRgbTransformEtc1A8* Effects::textureRgbTransformEtc1A8() 
	{
		return &_textureRgbTransformEtc1A8;
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

	EffectPostProcessRgbTransform* Effects::postProcessRgbTransform()
	{
		return &_postProcessRgbTransform;
	}
}
