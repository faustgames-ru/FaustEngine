#include "ZomboEditor.h"
#include "ZomboEditorViewport.h"
#include "ZomboEditorRenderService.h"
#include "commands/ZomboEditorCommands.h"
#include "commands/ZomboSetEditorMode.h"
#include "modes/ZomboEditorSelectionMode.h"
#include "ZomboEditorCamera.h"
#include "../common/ZomboGameEnvironment.h"

#include "../../fonts/FontsManager.h"

namespace zombo
{
	FT_Library  library;
	FT_Face     face;
	unsigned char* faceBuffer;

	ZomboEditor ZomboEditor::Default;


	ZomboEditor::ZomboEditor()
	{
		_mode = new ZomboEditorSelectionMode();
	}

	String ZomboEditor::getEditorModeInternal() const
	{
		return _actualModeName.c_str();;
	}

	void ZomboEditor::setEditorModeInternal(String mode)
	{
		_actualModeName = mode;
	}

	void ZomboEditor::setRootPath(String rootPath)
	{
		_rootPath = rootPath;
	}

	IntPtr ZomboEditor::getMode()
	{
		char *str = const_cast<char *>(_actualModeName.c_str());
		return str;
	}

	void ZomboEditor::setMode(String modeName)
	{		
		ZomboEditorCommand *command = new ZomboSetEditorMode(modeName);
		if (ZomboEditorCommands::Default.doCommand(command) == CommandExecutonStatus::CommandExecutionNotAvaliable)
		{
			delete command;
		}
	}

	bool ZomboEditor::isUndoAvaliable()
	{
		return ZomboEditorCommands::Default.isUndoAvaliable();
	}

	bool ZomboEditor::isRedoAvaliable()
	{
		return ZomboEditorCommands::Default.isRedoAvaliable();
	}

	void ZomboEditor::undo()
	{
		ZomboEditorCommands::Default.undo();
	}

	void ZomboEditor::redo()
	{
		ZomboEditorCommands::Default.redo();
	}

	void ZomboEditor::init()
	{
		resources::ContentManager::Default.open();
		std::string fontPath = _rootPath + std::string("Content/Quicksand-Regular.otf");
		resources::ContentProvider::openContent(fontPath.c_str());
		unsigned char* buffer = static_cast<unsigned char*>(resources::ContentManager::Default.getBuffer());
		int len = 0;
		const int pageSize = 256 * 1024;
		int count = 0;
		while ((count = resources::ContentProvider::read(buffer + len, pageSize)) > 0)
		{
			len += count;
		}

		fonts::FontsManager::Default.init();
		_font = fonts::FontsManager::Default.createOutlineVectorFont(buffer, len,
			&fonts::FontCharSet::latin
			/*new fonts::FontCharSet("i")*/
			);
		resources::ContentManager::Default.close();

		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.renderState = graphics::RenderState(); /// ???
		graphics::GraphicsDevice::Default.renderState.init();
		graphics::GraphicsDevice::Default.create();
		EFFECTS_CALL_CREATE
		FORMATS_CALL_CREATE
	}

	void ZomboEditor::update(float ellapsedTime)
	{
		ZomboGameEnvironment::update(ellapsedTime);
		ZomboEditorRenderService::Default.resetBuffers();
		ZomboEditorCamera::Default.update();
		_mode->update();
		
		const char *zomboString = "Zombo Editor";
		core::Vector2 size = _font->getSize(0.25f, zomboString);
		//_font->render((size * -0.5f).toVector3(), 0.25f, zomboString, &ZomboEditorFontRenderer::Default);
		_font->renderTringles((size * -0.5f).toVector3(), 0.25f, zomboString, &ZomboEditorFontRenderer::Default);
		float smoothScale = ZomboEditorCamera::Default.getInterpoaltedScale() / ZomboEditorViewport::Default.h;
		_font->renderTringlesSmooth((size * -0.5f).toVector3(), 0.25f, smoothScale, zomboString, &ZomboEditorFontRenderer::Default);
	}

	void ZomboEditor::render()
	{		
		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.setViewport(0, 0, ZomboEditorViewport::Default.w, ZomboEditorViewport::Default.h);
		graphics::GraphicsDevice::Default.clear();

		graphics::UniformValues::projection()->setValue(ZomboEditorCamera::Default.matrix);
		ZomboEditorRenderService::Default.applyRenderCommands();
	}

	void ZomboEditor::release()
	{
		delete this;
	}

	extern "C" DLLEXPORT IZomboEditor * API_CALL getZomboEditor()
	{
		return &ZomboEditor::Default;
	}

}
