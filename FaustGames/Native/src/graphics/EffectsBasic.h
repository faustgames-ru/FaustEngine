#ifndef EFFECTS_BASIC_H
#define EFFECTS_BASIC_H

#include "graphics_classes.h"
#include "Effect.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "EffectSolidColor.h"
#include "EffectTextureColor.h"


namespace graphics
{
	class EffectsBasic
	{
	public:
		static void create();
		static void cleanup();
		static EffectSolidColor * solidColor();
		static EffectTexturePackedColor * textureColor();
	private:
		static EffectSolidColor _solidColor;
		static EffectTexturePackedColor _textureColor;
	};
}


#endif /*EFFECTS_BASIC_H*/