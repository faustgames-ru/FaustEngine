#include "ZomboEditor.h"

namespace zombo
{
	void ZomboEditor::updateMouse(int w, int h, int x, int y, uint buttons)
	{
	}

	void ZomboEditor::render(int w, int h)
	{
		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.setViewport(0, 0, w, h);
		graphics::GraphicsDevice::Default.clear();
	}

	void ZomboEditor::release()
	{
		delete this;
	}

	extern "C" DLLEXPORT IZomboEditor * API_CALL createZomboEditor()
	{
		return new ZomboEditor();
	}

}
