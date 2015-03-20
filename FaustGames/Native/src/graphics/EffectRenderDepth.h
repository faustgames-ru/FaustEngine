#ifndef EFFECT_RENDER_DEPTH_H
#define EFFECT_RENDER_DEPTH_H

#include "graphics_classes.h"
#include "Effect.h"
#include "EffectBase.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "UniformValueMatrix.h"
#include "UniformValueTexture.h"

namespace graphics
{
	class EffectRenderDepth : public EffectBase
	{
	public:
		EffectRenderDepth();
		~EffectRenderDepth();
		void create();
	protected:
	private:
	};
}

#endif /*EFFECT_RENDER_DEPTH_H*/
