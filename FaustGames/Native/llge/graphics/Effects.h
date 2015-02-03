#ifndef EFFECTS_H
#define EFFECTS_H

#include "graphics_classes.h"
#include "Effect.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "EffectSolid.h"
#include "EffectSolidColor.h"
#include "EffectTextureColor.h"

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
	protected:
	private:
		static EffectSolid _solid;
		static EffectSolidColor _solidColor;
		static EffectTextureColor _textureColor;
	};
}


#endif /*EFFECTS_H*/