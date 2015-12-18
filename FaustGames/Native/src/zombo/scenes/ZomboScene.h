#ifndef ZOMBO_SCENE_H
#define ZOMBO_SCENE_H

#include "../zombo_classes.h"
#include "../common/states/ZomboStateController.h"
#include "../content/ZomboContentBlock.h"
#include "../common/ZomboCamera.h"

namespace zombo
{

	class ZomboScene
	{
	public:
		std::vector<ZomboCamera *> cameras;
		std::vector<ZomboEntity *> players;

		ZomboContentBlock content;
		ZomboScene();
		~ZomboScene();
		void update(float ellapsedTime) const;
	private:
	};

}

#endif /*ZOMBO_SCENE_H*/
