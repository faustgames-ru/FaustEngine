#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include "game_classes.h"

namespace game
{
	class Component: IBaseObject
	{
	public:
		Entity* owner;
		byte updateOrder;
		virtual void load() = 0;
		virtual void update(const UpdateArgs& e) = 0;
	private:
	};
}


#endif