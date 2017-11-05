#ifndef EFFECT_BASE_H
#define EFFECT_BASE_H

#include "graphics_classes.h"
#include "Effect.h"

namespace graphics
{
	class EffectBase: IBaseObject
	{
	public:
		EffectBase();
		Effect *getEffect();
		bool isConfigEqual(const void *c0, const void *c1) const;
		void configCopy(void *dst, const void *src) const;
		virtual void configApply(const void *config) {}
		virtual EffectBase* GetEtc1A8Effect() = 0;// { return this; }
		virtual EffectBase* ApplyEtc1A8(bool useAlpha) 

		{
			if (useAlpha)
			{
				return GetEtc1A8Effect();
			}
			else 
			{
				return this;
			}
		}
	protected:
		Effect _effect;
		int _configSize;
	private:
	};
}

#endif /*EFFECT_BASE_H*/
