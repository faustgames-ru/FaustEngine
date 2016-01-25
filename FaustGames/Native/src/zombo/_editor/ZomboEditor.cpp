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
#include "curves/CurvesManager.h"
#include "drawing/Drawer.h"
#include "ZomboEditorInput.h"

namespace zombo
{
	std::string ZomboEditorConstants::LogDisplayerFont("InternalContent/Quicksand-Regular[30px].otf");
	std::string ZomboEditorConstants::GameFile("Content/main.game");


	ZomboEditor ZomboEditor::Default;


	ZomboEditor::ZomboEditor() : 
		_needCallContetnLoaded(true), 
		_game(nullptr), 
		_scene(nullptr), 
		_background(nullptr), 
		_renderBackgroundEdges(false)
	{
		_mode = new ZomboEditorSelectionMode();
		_backgrounAlpha.setDuration(1.0f);
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
		bool res = ZomboEditorCommands::Default.isRedoAvaliable();
		return res;
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

		ZomboToolBox::Default.load();

		CurvesManager::Default.addCurve(core::Vector2(-1, -0.5f), core::Vector2(-0.5f, 0.25f), core::Vector2(0.5f, 0.25f), core::Vector2(1, -0.5f));
		CurvesManager::Default.addCurve(core::Vector2(-1, -1.5f), core::Vector2(-0.5f, -0.75f), core::Vector2(0.5f, -0.75f), core::Vector2(1, -1.5f));
		CurvesManager::Default.addCurve(core::Vector2(-1, 0.5f), core::Vector2(-0.5f, 1.25f), core::Vector2(0.5f, 1.25f), core::Vector2(1, 0.5f));
	}

	void ZomboEditor::update(float ellapsedTime)
	{

		ZomboGameEnvironment::update(ellapsedTime);
		ZomboEditorInput::Default.mouse.internalUpdate();
			
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
						_background = gameContent.getImage((_scene->thisDir + _scene->background).c_str());
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

		//graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.setClearState(0x0, 1.0f);
		graphics::GraphicsDevice::Default.setViewport(0, 0, ZomboEditorViewport::Default.w, ZomboEditorViewport::Default.h);
		graphics::GraphicsDevice::Default.clear();

		//renderBackground();
		renderBackgroundAsSkybox();
		graphics::UniformValues::projection()->setValue(ZomboEditorCamera::Default.matrix);
		ZomboEditorRenderService::Default.applyRenderCommands();
		renderGui();
	}

	void ZomboEditor::release()
	{
		delete this;
	}

	void ZomboEditor::renderBackground()
	{
		if (_background != nullptr)
		{
			_backgrounAlpha.setTargetValueIfNotEqual(1.0f);
			_backgrounAlpha.update();
			float scale = 2.0f / (_background->bounds.Max.getY() - _background->bounds.Min.getY());

			_backgroundMatrix.setValue(core::Matrix::createScale(scale / ZomboEditorViewport::Default.getAspect(), -scale, 1.0f));
			
			graphics::GraphicsDevice::Default.renderState.setEffect(graphics::EffectsBasic::textureColor());
			graphics::UniformValues::projection()->setValue(_backgroundMatrix);
			graphics::UniformValues::texture()->setValue(_background->texture->getHandle());

			ushort t0 = ZomboConstants::t0;
			ushort t1 = ZomboConstants::t1;
			uint tilesCount = 3;
			_backgroundVertices.resize(_background->vertices.size() * tilesCount);
			_backgroundIndices.resize(_background->indices.size() * tilesCount);
			float sizeX = _background->bounds.Max.getX() - _background->bounds.Min.getX();
			float posX = -sizeX * (tilesCount / 2);
			for (uint j = 0; j < tilesCount; j++)
			{
				for (uint i = 0; i < _background->indices.size(); i++)
				{
					_backgroundIndices[j * _background->indices.size() + i] = j * _background->vertices.size() + _background->indices[i];
				}
				for (uint i = 0; i < _background->vertices.size(); i++)
				{
					uint o = j*_background->vertices.size();
					_backgroundVertices[o + i].xyz = core::Vector3(_background->vertices[i].xy.getX() + posX, _background->vertices[i].xy.getY(), 0);
					_backgroundVertices[o + i].uv.u = _background->vertices[i].u;
					_backgroundVertices[o + i].uv.v = _background->vertices[i].v;
					_backgroundVertices[o + i].color = graphics::Color::mulA(0xffffffff, _backgrounAlpha.getValue());
				}
				posX += sizeX;
			}
			graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormatsBasic::positionColorTexture(), 
				_backgroundVertices.data(), _backgroundIndices.data(), _backgroundIndices.size() / 3);
			
			if (_renderBackgroundEdges)
			{
				_backgroundEdgesVertices.clear();
				for (uint i = 0; i < (_backgroundIndices.size() / 3); i++)
				{
					addBackgroundEdgeVertex(_backgroundIndices[i * 3 + 0]);
					addBackgroundEdgeVertex(_backgroundIndices[i * 3 + 1]);
					addBackgroundEdgeVertex(_backgroundIndices[i * 3 + 1]);
					addBackgroundEdgeVertex(_backgroundIndices[i * 3 + 2]);
					addBackgroundEdgeVertex(_backgroundIndices[i * 3 + 2]);
					addBackgroundEdgeVertex(_backgroundIndices[i * 3 + 0]);
				}
				graphics::GraphicsDevice::Default.renderState.setEffect(graphics::EffectsBasic::solidColor());
				graphics::GraphicsDevice::Default.drawEdges(graphics::VertexFormatsBasic::positionColor(),
					_backgroundEdgesVertices.data(), _backgroundEdgesVertices.size() / 2);
			}
		}
	}

	core::Vector3 getPosFromAngles(float a, float h)
	{
		float y = -core::Math::sin(h);
		float l = core::Math::cos(h);
		float x = l * core::Math::sin(a);
		float z = l * core::Math::cos(a);
		return core::Vector3(x, y, z);
	}

	void ZomboEditor::renderBackgroundAsSkybox()
	{
		if (_background == nullptr) return;
		_backgrounAlpha.setTargetValueIfNotEqual(1.0f);
		_backgrounAlpha.update();
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::EffectsBasic::textureColor());
		graphics::UniformValues::projection()->setValue(ZomboEditorCamera::Default.skyboxMatrix);
		graphics::UniformValues::texture()->setValue(_background->texture->getHandle());

		_backgroundVertices.clear();
		_backgroundIndices.clear();

		uint detailX = 32;
		uint detailY = 8;
		uint tilesPerPlane = 1;
		for (uint j = 0; j < detailX; j++)
		{
			float a0 = j * core::Math::Pi * 2.0f / detailX;
			float a1 = (j+1) * core::Math::Pi * 2.0f / detailX;

			ushort tx0 = static_cast<ushort>(j * ZomboConstants::t1 * 4* tilesPerPlane / detailX);
			ushort tx1 = static_cast<ushort>((j + 1) * ZomboConstants::t1 * 4* tilesPerPlane / detailX);

			float hAngle = core::Math::Pi *0.25f;
			float hAngleOffset = hAngle*0.8f;

			if (tx0 > tx1)
			{
				tx0 = 0;
			}

			for (uint i = 0; i < detailY; i++)
			{
				float h0 = i * hAngle / detailY;
				float h1 = (i + 1) * hAngle / detailY;

				ushort ty0 = static_cast<ushort>(i * ZomboConstants::t1 / detailY);
				ushort ty1 = static_cast<ushort>((i + 1) * ZomboConstants::t1 / detailY);


				h0 -= hAngleOffset;
				h1 -= hAngleOffset;

				core::Vector3 p00 = getPosFromAngles(a0, h0);
				core::Vector3 p01 = getPosFromAngles(a0, h1);
				core::Vector3 p10 = getPosFromAngles(a1, h0);
				core::Vector3 p11 = getPosFromAngles(a1, h1);

				uint k = _backgroundVertices.size();
				_backgroundIndices.push_back(k + 0);
				_backgroundIndices.push_back(k + 1);
				_backgroundIndices.push_back(k + 2);
				_backgroundIndices.push_back(k + 0);
				_backgroundIndices.push_back(k + 2);
				_backgroundIndices.push_back(k + 3);
				uint c = graphics::Color::mulA(0xffffffff, _backgrounAlpha.getValue());;
				_backgroundVertices.push_back(RenderVertex(p00, c, tx0, ty0));
				_backgroundVertices.push_back(RenderVertex(p01, c, tx0, ty1));
				_backgroundVertices.push_back(RenderVertex(p11, c, tx1, ty1));
				_backgroundVertices.push_back(RenderVertex(p10, c, tx1, ty0));
			}

			uint k = _backgroundVertices.size();
			_backgroundIndices.push_back(k + 0);
			_backgroundIndices.push_back(k + 1);
			_backgroundIndices.push_back(k + 2);
			_backgroundIndices.push_back(k + 3);
			_backgroundIndices.push_back(k + 4);
			_backgroundIndices.push_back(k + 5);
			uint c = graphics::Color::mulA(0xffffffff, _backgrounAlpha.getValue());;

			core::Vector3 p00 = getPosFromAngles(a0, -hAngleOffset);
			core::Vector3 p10 = getPosFromAngles(a1, -hAngleOffset);
			core::Vector3 p01 = getPosFromAngles(a0, -hAngleOffset + hAngle);
			core::Vector3 p11 = getPosFromAngles(a1, -hAngleOffset + hAngle);

			ushort ty0 = ZomboConstants::t0;
			ushort ty1 = ZomboConstants::t1;

			_backgroundVertices.push_back(RenderVertex(p00, c, tx0, ty0));
			_backgroundVertices.push_back(RenderVertex(p10, c, tx1, ty0));
			_backgroundVertices.push_back(RenderVertex(core::Vector3(0, 1, 0), c, tx1, ty0));

			_backgroundVertices.push_back(RenderVertex(p01, c, tx0, ty1));
			_backgroundVertices.push_back(RenderVertex(p11, c, tx1, ty1));
			_backgroundVertices.push_back(RenderVertex(core::Vector3(0, -1, 0), c, tx1, ty1));
		}

		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormatsBasic::positionColorTexture(),
			_backgroundVertices.data(), _backgroundIndices.data(), _backgroundIndices.size() / 3);

	}

	void ZomboEditor::renderGui()
	{
		_guiMatrix.setValue(core::Matrix::mul(
			core::Matrix::createOrtho(ZomboEditorViewport::Default.getAspect(), static_cast<float>(ZomboEditorViewport::Default.h) / 2.0f, 1.0f),
			core::Matrix::createTranslate(-1.0f, -1.0f, 0.0f)));
		graphics::UniformValues::projection()->setValue(_guiMatrix);

		ZomboEditorRenderService::Gui.applyRenderCommands();
	}

	void ZomboEditor::internalUpdate()
	{
		ZomboToolBox::Default.updateInput();
		ZomboEditorCamera::Default.update();

		ZomboDrawer::Default.setScaleToPixels(ZomboEditorViewport::Default.h * 0.5f / ZomboEditorCamera::Default.getInterpoaltedScale());
		ZomboDrawer::Gui.setScaleToPixels(1.0f);
		ZomboDrawer::Default.setSmoothR(ZomboConstants::SmoothLevel);
		ZomboDrawer::Gui.setSmoothR(ZomboConstants::SmoothLevel);

		_mode->update();
		CurvesManager::Default.update();
		ZomboEditorLogDisplayer::Default.update();
		ZomboToolBox::Default.update();
	}

	void ZomboEditor::contentLoaded()
	{
		ZomboEditorLogDisplayer::Default.load();
	}

	void ZomboEditor::addBackgroundEdgeVertex(uint i)
	{
		RenderVertex r = _backgroundVertices[i];
		ColorVertex c;
		c.xyz = r.xyz;
		c.color = r.color;
		_backgroundEdgesVertices.push_back(c);
	}

	extern "C" DLLEXPORT IZomboEditor * API_CALL getZomboEditor()
	{
		return &ZomboEditor::Default;
	}

}
