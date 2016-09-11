#ifndef ZOMBO_EDITOR_COMPONENTS_H
#define ZOMBO_EDITOR_COMPONENTS_H

#include "../../game/Component.h"

namespace zombo
{
	class Grid : public game::Component
	{
	public:
		Grid();
		virtual void enqueueResources(const game::LoadArgs& e) OVERRIDE;
	private:
	};

	class GridEdge : public game::Component
	{
	public:
		core::Vector3 p0;
		core::Vector3 p1;
		uint color;
		GridEdge();
		virtual void loaded(const game::LoadedArgs& e) OVERRIDE;
		virtual void render(const game::RenderArgs& e) OVERRIDE;
	private:
	};
}


#endif /*ZOMBO_EDITOR_COMPONENTS_H*/