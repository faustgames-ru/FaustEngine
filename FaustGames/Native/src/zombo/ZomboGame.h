#ifndef ZOMBO_GAME_H
#define ZOMBO_GAME_H

#include "zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "content/ZomboContentBlock.h"
#include "ZomboWorld.h"

namespace zombo
{
	class ZomboGame : public IZomboGame
	{
	public:
		virtual void API_CALL setRootPath(String rootPath) OVERRIDE;
		virtual void API_CALL load(int w, int h) OVERRIDE;
		virtual void API_CALL update(int w, int h, float ellapsedTime) OVERRIDE;
		virtual void API_CALL render(int w, int h, float ellapsedTime) OVERRIDE;
		virtual void API_CALL release() OVERRIDE;
	private:
		ZomboWorld zomboWorld;
		
		ZomboContentBlock _contentBlock;
		ZomboContentFrameAnimation *_aniamtion;
		std::string _rootPath;
	};
}

#endif /* ZOMBO_GAME_H */