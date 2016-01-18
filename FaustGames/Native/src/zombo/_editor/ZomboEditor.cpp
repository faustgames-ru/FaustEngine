#include "ZomboEditor.h"
#include "ZomboEditorViewport.h"
#include "ZomboEditorRenderService.h"
#include "commands/ZomboEditorCommands.h"
#include "commands/ZomboSetEditorMode.h"
#include "modes/ZomboEditorSelectionMode.h"
#include "ZomboEditorCamera.h"
#include "../common/ZomboGameEnvironment.h"

#include "../../fonts/FontsManager.h"
#include "../ZomboLog.h"
#include "ZomboEditorLogDisplayer.h"

namespace zombo
{
	std::string ZomboEditorConstants::LogDisplayerFont("Content/Quicksand-Regular[30px].otf");


	ZomboEditor ZomboEditor::Default;


	ZomboEditor::ZomboEditor() : _needCallContetnLoaded(true)
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
		internalContent.setRoot(_rootPath);
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

	float scaleFonts = 1.0f;
	float fontSize = 32.0f;

	void ZomboEditor::init()
	{
		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.renderState = graphics::RenderState(); /// ???
		graphics::GraphicsDevice::Default.renderState.init();
		graphics::GraphicsDevice::Default.create();
		EFFECTS_CALL_CREATE
		FORMATS_CALL_CREATE

		fonts::FontsManager::Default.init();
		resources::ContentManager::Default.open();
		internalContent.enqueueResource(ZomboEditorConstants::LogDisplayerFont.c_str());
		_needCallContetnLoaded = true;

		/*
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

		_font = fonts::FontsManager::Default.createOutlineVectorFont(buffer, len,
			&fonts::FontCharSet::latin
			//new fonts::FontCharSet("i")
			);

		_bitmapFont = fonts::FontsManager::Default.createBitmapFont(buffer, len, fontSize*scaleFonts,
			&fonts::FontCharSet::latin
			//new fonts::FontCharSet("123")
			);

		resources::ContentManager::Default.close();
		*/
	}

	

	void ZomboEditor::update(float ellapsedTime)
	{
		ZomboGameEnvironment::update(ellapsedTime);
		ZomboEditorRenderService::Default.resetBuffers();
		ZomboEditorRenderService::Gui.resetBuffers();
		
		if (internalContent.isLoaded())
		{
			if (_needCallContetnLoaded)
			{
				resources::ContentManager::Default.close();
				contentLoaded();
				_needCallContetnLoaded = false;
			}
			else
			{
				internalUpdate();
			}
		}		
	}
		
	void ZomboEditor::render()
	{		
		internalContent.update();

		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.setViewport(0, 0, ZomboEditorViewport::Default.w, ZomboEditorViewport::Default.h);
		graphics::GraphicsDevice::Default.clear();

		graphics::UniformValues::projection()->setValue(ZomboEditorCamera::Default.matrix);
		
		ZomboEditorRenderService::Default.applyRenderCommands();
		renderGui();
	}

	void ZomboEditor::release()
	{
		delete this;
	}

	void ZomboEditor::renderGui()
	{
		_guiMatrix.setValue(core::Matrix::mul(
			core::Matrix::createOrtho(ZomboEditorViewport::Default.getAspect(), static_cast<float>(ZomboEditorViewport::Default.h) / 2.0f, 1.0f),
			core::Matrix::createTranslate(-1.0f, -1.0f, 0.0f)));
		graphics::UniformValues::projection()->setValue(_guiMatrix);// todo: create gui matrix

		ZomboEditorRenderService::Gui.applyRenderCommands();
	}

	void ZomboEditor::internalUpdate()
	{
		ZomboEditorCamera::Default.update();
		_mode->update();
		ZomboEditorLogDisplayer::Default.update();
	}

	void ZomboEditor::contentLoaded()
	{
		ZomboEditorLogDisplayer::Default.load();
	}

	extern "C" DLLEXPORT IZomboEditor * API_CALL getZomboEditor()
	{
		return &ZomboEditor::Default;
	}

}
