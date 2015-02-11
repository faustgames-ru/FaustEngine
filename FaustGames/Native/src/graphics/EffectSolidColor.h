#ifndef EFFECT_SOLID_COLOR_H
#define EFFECT_SOLID_COLOR_H

#include "graphics_classes.h"
#include "Effect.h"
#include "EffectBase.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "UniformValueMatrix.h"

namespace graphics
{
	class EffectSolidColor : public EffectBase
	{
	public:
		EffectSolidColor();
		~EffectSolidColor();
		void create();
	protected:
	private:
	};
}

#endif /*EFFECT_SOLID_COLOR_H*/
