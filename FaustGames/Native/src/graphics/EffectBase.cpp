#include "EffectBase.h"

namespace graphics
{
	EffectBase::EffectBase(): _configSize(0)
	{
	}

	Effect *EffectBase::getEffect()
	{
		return &_effect;
	}
		
	bool EffectBase::isConfigEqual(const void* c0, const void* c1) const
	{
		const char *p0 = static_cast<const char *>(c0);
		const char *p1 = static_cast<const char *>(c1);
		for (int i = 0; i < _configSize; i++)
		{
			if (p0[i] != p1[i])
			{
				return false;
			}
		}
		return true;
	}

	void EffectBase::configCopy(void* dst, const void* src) const
	{
		const char *p0 = static_cast<const char *>(src);
		char *p1 = static_cast<char *>(dst);
		for (int i = 0; i < _configSize; i++)
		{
			p1[i] = p0[i];
		}
	}
}
