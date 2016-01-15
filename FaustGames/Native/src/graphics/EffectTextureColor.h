#ifndef EFFECT_TEXTURE_COLOR_H
#define EFFECT_TEXTURE_COLOR_H

#include "graphics_classes.h"
#include "Effect.h"
#include "EffectBase.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "UniformValueMatrix.h"
#include "UniformValueTexture.h"

namespace graphics
{
	class EffectTexturePackedColor : public EffectBase
	{
	public:
		EffectTexturePackedColor();
		~EffectTexturePackedColor();
		void create();
	protected:
	private:
	};
	class EffectTextureColor : public EffectBase
	{
	public:
		EffectTextureColor();
		~EffectTextureColor();
		void create();
	protected:
	private:
	};
}

#endif /*EFFECT_TEXTURE_COLOR_H*/
