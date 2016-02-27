#include "ZomboStatistics.h"

namespace zombo
{
	ZomboStatistics ZomboStatistics::Default;


	float ZomboStatistics::getFramesLimit() const
	{
		return _framesLimit;
	}

	ZomboStatistics::ZomboStatistics(): FpsMin(0), FpsMax(0), FpsAv(0), _framesLimit(100)
	{
	}

	void ZomboStatistics::update(float ellapsedSeconds)
	{
		if (ellapsedSeconds < 0.0001f) 
			return;
		frames.push_back(1.0f / ellapsedSeconds);
		while (frames.size() > _framesLimit)
		{
			frames.pop_front();
		}
		History.clear();
		FpsAv = 0;
		FpsMin = core::Math::MaxValue;
		FpsMax = 0;
		for (FramesContainer::iterator i = frames.begin(); i != frames.end(); ++i)
		{
			FpsAv += *i;
			if (FpsMin > *i)
				FpsMin = *i;
			if (FpsMax < *i)
				FpsMax = *i;
			History.push_back(*i);
		}
		if (History.size() > 0)
		{
			FpsAv /= History.size();
		}
		else
		{
			FpsAv = 0;
			FpsMin = 0;
			FpsMax = 0;
		}
	}
}
