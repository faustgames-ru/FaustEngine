#ifndef RAZOR_COMPONENTS_GRID_H
#define RAZOR_COMPONENTS_GRID_H

#include "../razor_classes.h"
#include "../../game/Component.h"

namespace razor
{
	class Grid : public game::Component
	{
	public:
		Grid();
		virtual void enqueueResources(const game::LoadArgs& e) OVERRIDE;
	private:
	};

	class GridNode : public game::Component
	{
	public:
		GridNode();
		virtual void enqueueResources(const game::LoadArgs& e) OVERRIDE;
		virtual void loaded() OVERRIDE;
		virtual void render() OVERRIDE;
	};
}


#endif /*RAZOR_COMPONENTS_SPACESHIP_H*/