#ifndef EFFECTS_H
#define EFFECTS_H

#include "graphics_classes.h"
#include "Effect.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "EffectSolid.h"
#include "EffectSolidColor.h"
#include "EffectTextureColor.h"
#include "EffectTextureAlphamaskColor.h"
#include "EffectTextureColorHighlight.h"
#include "EffectTextureLightMapColor.h"
#include "EffectsPostProcess.h"

namespace graphics
{
	class Effects
	{
	public:
		static void create();
		static void cleanup();
		static EffectSolid * solid();
		static EffectSolidColor * solidColor();
		static EffectTextureColor * textureColor();
		static EffectTextureColorEtc1A8 * textureColorEtc1A8();
		static EffectTextureColorFog * textureColorFog();
		static EffectTextureColorFogEtc1A8 * textureColorFogEtc1A8();
		static EffectTextureAlphamaskColor * textureAlphamaskColor();
		static EffectTextureColorHighlight * textureColorHighlight();
		//static EffectTextureLightMapColor * textureLightmapColor();
		static EffectTextureRgbTransformEtc1A8* textureRgbTransformEtc1A8();
		static EffectTextureRgbTransform* textureRgbTransform();

		static EffectPostProcessToneMapping * postProcessToneMapping();
		static EffectPostProcessBloomFilter * postProcessBloomFilter();
		static EffectPostProcessEmpty * postProcessEmpty();
		static EffectPostProcessEmptyColor * postProcessEmptyColor();
		static EffectPostProcessVBlurFilter * postProcessVBlurFilter();
		static EffectPostProcessHBlurFilter * postProcessHBlurFilter();
		static EffectPostProcessBlurPass * postProcessBlurPassFilter();
		static EffectPostProcessBloomAddFilter * postProcessBloomAddFilter();
		static EffectPostProcessRgbTransform * postProcessRgbTransform();
		
	protected:
	private:
		static EffectSolid _solid;
		static EffectSolidColor _solidColor;
		static EffectTextureColor _textureColor;		
		static EffectTextureColorEtc1A8 _textureColorEtc1A8;
		static EffectTextureColorFog _textureColorFog;
		static EffectTextureColorFogEtc1A8 _textureColorFogEtc1A8;
		static EffectTextureAlphamaskColor _textureAlphamaskColor;
		static EffectTextureColorHighlight _textureColorHighlight;
		//static EffectTextureLightMapColor _textureLightmapColor;
		static EffectTextureRgbTransformEtc1A8 _textureRgbTransformEtc1A8;
		static EffectTextureRgbTransform _textureRgbTransform;

		static EffectPostProcessToneMapping _postProcessToneMapping;
		static EffectPostProcessBloomFilter _postProcessBloomFilter;
		static EffectPostProcessEmpty _postProcessEmpty;
		static EffectPostProcessEmptyColor _postProcessEmptyColor;
		static EffectPostProcessVBlurFilter _postProcessVBlurFilter;
		static EffectPostProcessHBlurFilter _postProcessHBlurFilter;
		static EffectPostProcessBloomAddFilter _postProcessBloomAddFilter;
		static EffectPostProcessBlurPass _postProcessBlurPass;
		static EffectPostProcessRgbTransform _postProcessRgbTransform;

	};
}


#endif /*EFFECTS_H*/