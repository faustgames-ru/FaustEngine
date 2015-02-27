#ifndef RENDER_2D_COMPONENT_H
#define RENDER_2D_COMPONENT_H

#include "entities_classes.h"
#include "Component.h"

namespace entities
{
	class Render2dComponent : public Component
	{
	public:
		drawing::RenderItem2d * RenderItem;
		static ComponentType::e Type;
	private:
	};
}

#endif /*RENDER_2D_COMPONENT_H*/