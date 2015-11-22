#ifndef ZOMBO_GAME_H
#define ZOMBO_GAME_H

#include "../../CommonSources/include/zombo.h"
#include "zombo_classes.h";

namespace zombo
{
	class ZomboGame : IZomboGame
	{
	public:
		virtual void API_CALL load() OVERRIDE;
		virtual void API_CALL update(float ellapsedTime) OVERRIDE;
		virtual void API_CALL render() OVERRIDE;
	};
}

#endif /* ZOMBO_GAME_H */