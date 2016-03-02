#include "ZomboEditorViewport.h"

namespace zombo
{
	ZomboEditorViewport ZomboEditorViewport::Default;
	
	float ZomboEditorViewport::getAspect() const
	{
		return static_cast<float>(w) / static_cast<float>(h);
	}

	void ZomboEditorViewport::update(int width, int height)
	{
		w = width;
		h = height;
	}

	core::Vector2 ZomboEditorViewport::getCenter()
	{
		return core::Vector2(w*0.5f, h*0.5f);
	}

	core::Vector2 ZomboEditorViewport::getSize()
	{
		return core::Vector2(w, h);
	}

	extern "C" DLLEXPORT IZomboEditorViewport* API_CALL getZomboEditorViewport()
	{
		return &ZomboEditorViewport::Default;
	}

}
