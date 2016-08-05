#ifndef EFFECT_TEXTURE_LIGHTMAP_COLOR_H
#define EFFECT_TEXTURE_LIGHTMAP_COLOR_H

#include "graphics_classes.h"
#include "Effect.h"
#include "EffectBase.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "UniformValueMatrix.h"
#include "UniformValueTexture.h"

namespace graphics
{
	class EffectTextureLightMapColor : public EffectBase
	{
	public:
		EffectTextureLightMapColor();
		~EffectTextureLightMapColor();
		void create();
		virtual void configApply(const void *config) OVERRIDE;
	protected:
	private:
	};

	class EffectTextureBlurColor : public EffectBase
	{
	public:
		EffectTextureBlurColor();
		~EffectTextureBlurColor();
		void create();
		virtual void configApply(const void *config) OVERRIDE;
	protected:
	private:
	};
}

#endif /*EFFECT_TEXTURE_LIGHTMAP_COLOR_H*/
