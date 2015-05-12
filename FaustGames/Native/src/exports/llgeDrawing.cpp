#include "llge.h"
#include "..\drawing\Renderer2d.h"
#include "..\drawing\Batcher.h"
#include "..\graphics\Texture.h"

#include <limits>

namespace llge
{
	extern "C" DLLEXPORT  IBatch2d * API_CALL createBatch2d()
	{
		return new drawing::Batcher();
	}

	extern "C" DLLEXPORT ITexture * API_CALL createTextureByID(uint id)
	{
		graphics::Texture *t = new graphics::Texture();
		t->setHandle(id);
		return t;
	}

}