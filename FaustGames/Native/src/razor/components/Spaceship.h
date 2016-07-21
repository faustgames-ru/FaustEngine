#ifndef RAZOR_COMPONENTS_SPACESHIP_H
#define RAZOR_COMPONENTS_SPACESHIP_H

#include "../razor_classes.h"
#include "../../content/ContentFrameAnimation.h"
#include "../../game/Component.h"
#include "../../game/components/FrameAnimation.h"

namespace razor
{
	class Spaceship : public game::Component
	{
	public:
		Spaceship();
		virtual void loaded() OVERRIDE;
		virtual void update(const game::UpdateArgs& e) OVERRIDE;
	private:
		game::FrameAnimation* _animation;
	};
}


#endif /*RAZOR_COMPONENTS_SPACESHIP_H*/