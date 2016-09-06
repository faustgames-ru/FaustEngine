#ifndef ZOMBO_STATISTICS_H
#define ZOMBO_STATISTICS_H

#include "../zombo_classes.h"

namespace zombo
{
	class ZomboStatistics
	{
	public:
		static ZomboStatistics Default;
		float FpsMin;
		float FpsMax;
		float FpsAv;
		float getFramesLimit() const;
		std::vector<float> History;
		ZomboStatistics();
		void update(float ellapsedSeconds);
		typedef std::list<float> FramesContainer;
	private:
		FramesContainer frames;
		int _framesLimit;
	};
}

#endif /*ZOMBO_STATISTICS_H*/
