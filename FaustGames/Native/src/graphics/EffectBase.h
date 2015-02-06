#ifndef EFFECT_BASE_H
#define EFFECT_BASE_H

#include "graphics_classes.h"
#include "Effect.h"

namespace graphics
{
	class EffectBase
	{
	public:
		Effect *getEffect();
	protected:
		Effect _effect;
	private:
	};
}

#endif /*EFFECT_BASE_H*/
