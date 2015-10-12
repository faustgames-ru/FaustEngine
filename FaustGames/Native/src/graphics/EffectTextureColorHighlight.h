#ifndef EFFECT_TEXTURE_COLOR_HIGHLIGHT_H
#define EFFECT_TEXTURE_COLOR_HIGHLIGHT_H

#include "graphics_classes.h"
#include "Effect.h"
#include "EffectBase.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "UniformValueMatrix.h"
#include "UniformValueTexture.h"

namespace graphics
{
	class EffectTextureColorHighlight : public EffectBase
	{
	public:
		EffectTextureColorHighlight();
		~EffectTextureColorHighlight();
		void create();
	protected:
	private:
	};
}

#endif /*EFFECT_TEXTURE_COLOR_HIGHLIGHT_H*/
