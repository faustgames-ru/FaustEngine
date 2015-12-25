#include "ZomboEditorViewport.h"

namespace zombo
{
	ZomboEditorViewport ZomboEditorViewport::Default;


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
