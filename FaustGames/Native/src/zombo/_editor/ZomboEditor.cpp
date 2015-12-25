#include "ZomboEditor.h"
#include "ZomboEditorViewport.h"
#include "ZomboEditorRenderService.h"

namespace zombo
{
	void ZomboEditor::setMode(String modeName)
	{
	}

	void ZomboEditor::update()
	{
	}

	void ZomboEditor::render()
	{
		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.setViewport(0, 0, ZomboEditorViewport::Default.w, ZomboEditorViewport::Default.h);
		graphics::GraphicsDevice::Default.clear();

		ZomboEditorRenderService::Default.applyRenderCommands();
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
