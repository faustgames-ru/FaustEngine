#include "Viewport.h"

namespace graphics
{
	Viewport::Viewport(): w(800), h(450)
	{
	}

	float Viewport::getAspect() const
	{
		return static_cast<float>(w) / static_cast<float>(h);
	}

	void Viewport::update(int width, int height)
	{
		w = width;
		h = height;
	}

	core::Vector2 Viewport::getCenter()
	{
		return core::Vector2(w*0.5f, h*0.5f);
	}

	core::Vector2 Viewport::getSize()
	{
		return core::Vector2(w, h);
	}
}