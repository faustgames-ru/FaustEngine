#ifndef EFFECTS_H
#define EFFECTS_H

#include "graphics_classes.h"
#include "Effect.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "EffectSolid.h"
#include "EffectSolidColor.h"
#include "EffectTextureColor.h"
#include "EffectTextureLightMapColor.h"
#include "EffectWater.h"
#include "EffectRenderDepth.h"
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
		static EffectTextureLightMapColor * textureLightmapColor();
		//static EffectWater * water();
		static EffectRenderDepth * renderDepth();
		static EffectPostProcessToneMapping * postProcessToneMapping();
		static EffectPostProcessBloomFilter * postProcessBloomFilter();
		static EffectPostProcessVBlurFilter * postProcessVBlurFilter();
		static EffectPostProcessHBlurFilter * postProcessHBlurFilter();
		static EffectPostProcessBloomAddFilter * postProcessBloomAddFilter();
	protected:
	private:
		static EffectSolid _solid;
		static EffectSolidColor _solidColor;
		static EffectTextureColor _textureColor;
		static EffectTextureLightMapColor _textureLightmapColor;
		//static EffectWater _water;
		static EffectRenderDepth _renderDepth;
		static EffectPostProcessToneMapping _postProcessToneMapping;
		static EffectPostProcessBloomFilter _postProcessBloomFilter;
		static EffectPostProcessVBlurFilter _postProcessVBlurFilter;
		static EffectPostProcessHBlurFilter _postProcessHBlurFilter;
		static EffectPostProcessBloomAddFilter _postProcessBloomAddFilter;
	};
}


#endif /*EFFECTS_H*/