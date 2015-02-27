#ifndef RENDER_ITEM_H
#define RENDER_ITEM_H

#include "drawing_classes.h"
#include "Renderer2d.h"

namespace drawing
{
	class RenderItem2d
	{
	public:
		virtual void render(Renderer2d *renderer) = 0;
	private:
	};
}

#endif /*RENDER_ITEM_H*/