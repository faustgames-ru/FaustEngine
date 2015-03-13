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
		static EffectWater * water();
	protected:
	private:
		static EffectSolid _solid;
		static EffectSolidColor _solidColor;
		static EffectTextureColor _textureColor;
		static EffectTextureLightMapColor _textureLightmapColor;
		static EffectWater _water;

	};
}


#endif /*EFFECTS_H*/