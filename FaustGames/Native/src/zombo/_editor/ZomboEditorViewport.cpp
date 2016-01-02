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

	extern "C" DLLEXPORT IZomboEditorViewport* API_CALL getZomboEditorViewport()
	{
		return &ZomboEditorViewport::Default;
	}

}
