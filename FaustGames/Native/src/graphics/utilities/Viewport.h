#ifndef GRPAHICS_UTILITIES_VIEWPORT_H
#define GRPAHICS_UTILITIES_VIEWPORT_H

#include "../graphics_classes.h"

namespace graphics
{
	class Viewport
	{
	public:
		int w;
		int h;
		Viewport();
		float getAspect() const;
		void update(int width, int height);
		core::Vector2 getCenter();
		core::Vector2 getSize();
	};
}

#endif /*GRPAHICS_UTILITIES_VIEWPORT_H*/
