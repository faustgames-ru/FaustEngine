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
	std::string ZomboEditorConstants::LogDisplayerFont("InternalContent/Quicksand-Regular[30px].otf");
	std::string ZomboEditorConstants::GameFile("Content/main.game");


	ZomboEditor ZomboEditor::Default;


	ZomboEditor::ZomboEditor() : _needCallContetnLoaded(true), _game(nullptr), _scene(nullptr)
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
		gameContent.setRoot(_rootPath);
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
		gameContent.enqueueResource(ZomboEditorConstants::GameFile.c_str());
		_needCallContetnLoaded = true;

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
				contentLoaded();
				_needCallContetnLoaded = false;
			}
			else
			{
				internalUpdate();
			}
		}	

		// todo: async loading tree

		if (gameContent.isLoaded())
		{
			if (_game == nullptr)
			{
				_game = gameContent.getGame(ZomboEditorConstants::GameFile.c_str());
				gameContent.enqueueResource((_game->thisDir + _game->startup).c_str());
			}
			else
			{
				if (_scene == nullptr)
				{
					_scene = gameContent.getScene((_game->thisDir + _game->startup).c_str());
					for (uint i = 0; i < _scene->resources.size(); i++)
					{
						gameContent.enqueueResource((_scene->thisDir + _scene->resources[i]).c_str());
					}
					for (uint i = 0; i < _scene->platforms.size(); i++)
					{
						gameContent.enqueueResource((_scene->thisDir + _scene->platforms[i]).c_str());
					}
				}
				else
				{
					if (_platforms.empty())
					{
						for (uint i = 0; i < _scene->platforms.size(); i++)
						{
							_platforms.push_back(gameContent.getPlatform((_scene->thisDir + _scene->platforms[i]).c_str()));
							ZomboToolBox::Default.addItem(new ZomboToolBoxItem());
						}
						resources::ContentManager::Default.close();
					}
				}
			}
		}
	}
		
	void ZomboEditor::render()
	{		
		internalContent.update();
		gameContent.update();

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
		ZomboToolBox::Default.updateInput();
		ZomboEditorCamera::Default.update();
		_mode->update();
		ZomboEditorLogDisplayer::Default.update();
		ZomboToolBox::Default.update();
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
