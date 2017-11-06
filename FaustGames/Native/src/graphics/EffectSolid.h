#ifndef EFFECT_SOLID_H
#define EFFECT_SOLID_H

#include "graphics_classes.h"
#include "Effect.h"
#include "EffectBase.h"
#include "UniformInfo.h"
#include "Uniform.h"
#include "UniformValueMatrix.h"

namespace graphics
{
	class EffectSolid : public EffectBase
	{
	public:
		EffectSolid();
		~EffectSolid();
		void create();
		virtual EffectBase* GetEtc1A8Effect() { return this; }

	protected:
	private:
	};
}

#endif /*EFFECT_SOLID_H*/
