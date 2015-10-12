#ifndef EFFECT_TEXTURE_ALPHAMASK_COLOR_H
#define EFFECT_TEXTURE_ALPHAMASK_COLOR_H

#include "graphics_classes.h"
#include "Effect.h"
#include "EffectBase.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "UniformValueMatrix.h"
#include "UniformValueTexture.h"

namespace graphics
{
	class EffectTextureAlphamaskColor : public EffectBase
	{
	public:
		EffectTextureAlphamaskColor();
		~EffectTextureAlphamaskColor();
		void create();
	protected:
	private:
	};
}

#endif /*EFFECT_TEXTURE_ALPHAMASK_COLOR_H*/
