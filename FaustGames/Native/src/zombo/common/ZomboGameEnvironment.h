#ifndef ZOMBO_GAME_ENVIRONMENT_H
#define ZOMBO_GAME_ENVIRONMENT_H

#include "../zombo_classes.h"

namespace zombo
{
	class ZomboGameEnvironment
	{
	public:
		static ulong gameTime;
		static ulong ellapsedTime;
		static float ellapsedSeconds;
		static void update(float ellapsedTime);
	};
}

#endif /* ZOMBO_GAME_ENVIRONMENT_H */