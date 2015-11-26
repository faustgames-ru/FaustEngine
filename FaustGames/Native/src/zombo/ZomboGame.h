#ifndef ZOMBO_GAME_H
#define ZOMBO_GAME_H

#include "../../CommonSources/include/zombo.h"
#include "zombo_classes.h"

namespace zombo
{
	class ZomboGame : public IZomboGame
	{
	public:
		virtual void API_CALL load(int w, int h) OVERRIDE;
		virtual void API_CALL update(int w, int h, float ellapsedTime) OVERRIDE;
		virtual void API_CALL render(int w, int h, float ellapsedTime) OVERRIDE;
		virtual void API_CALL release() OVERRIDE;
	};
}

#endif /* ZOMBO_GAME_H */