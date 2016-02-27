#include "ValuesAnimator.h"

namespace zombo
{
	ValuesAnimator ValuesAnimator::Default;

	ValuesAnimator::ValuesAnimator()
	{
	}

	void ValuesAnimator::animate(float* value, float target)
	{
		typename MapFloat::iterator it = _valuesFloat.find(value);
		SEntryFloat* entry;
		if (it == _valuesFloat.end())
		{
			_valuesFloat[value] = entry = new SEntryFloat(value);
		}
		else
		{
			entry = _valuesFloat[value];
		}
		entry->setTarget(target);
	}
}
