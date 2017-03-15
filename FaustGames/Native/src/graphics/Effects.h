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
		static EffectTextureColorFog * textureColorFog();
		static EffectTextureAlphamaskColor * textureAlphamaskColor();
		static EffectTextureColorHighlight * textureColorHighlight();
		static EffectTextureLightMapColor * textureLightmapColor();
		static EffectTextureRgbTransform* textureRgbTransform();

		static EffectPostProcessToneMapping * postProcessToneMapping();
		static EffectPostProcessBloomFilter * postProcessBloomFilter();
		static EffectPostProcessEmpty * postProcessEmpty();
		static EffectPostProcessEmptyColor * postProcessEmptyColor();
		static EffectPostProcessVBlurFilter * postProcessVBlurFilter();
		static EffectPostProcessHBlurFilter * postProcessHBlurFilter();
		static EffectPostProcessBlurPass * postProcessBlurPassFilter();
		static EffectPostProcessBloomAddFilter * postProcessBloomAddFilter();

	protected:
	private:
		static EffectSolid _solid;
		static EffectSolidColor _solidColor;
		static EffectTextureColor _textureColor;
		static EffectTextureColorFog _textureColorFog;
		static EffectTextureAlphamaskColor _textureAlphamaskColor;
		static EffectTextureColorHighlight _textureColorHighlight;
		static EffectTextureLightMapColor _textureLightmapColor;
		static EffectTextureRgbTransform _textureRgbTransform;

		static EffectPostProcessToneMapping _postProcessToneMapping;
		static EffectPostProcessBloomFilter _postProcessBloomFilter;
		static EffectPostProcessEmpty _postProcessEmpty;
		static EffectPostProcessEmptyColor _postProcessEmptyColor;
		static EffectPostProcessVBlurFilter _postProcessVBlurFilter;
		static EffectPostProcessHBlurFilter _postProcessHBlurFilter;
		static EffectPostProcessBloomAddFilter _postProcessBloomAddFilter;
		static EffectPostProcessBlurPass _postProcessBlurPass;
		
	};
}


#endif /*EFFECTS_H*/