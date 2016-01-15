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

	float scaleFonts = 0.3f;


	void ZomboEditor::init()
	{
		resources::ContentManager::Default.open();
		std::string fontPath = _rootPath + std::string("Content/arial.ttf");
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

		_bitmapFont = fonts::FontsManager::Default.createBitmapFont(buffer, len, 32.0f*scaleFonts,
			&fonts::FontCharSet::latin
			//new fonts::FontCharSet("123")
			);

		resources::ContentManager::Default.close();

		graphics::GraphicsDevice::Default.setClearState(0x805050, 1.0f);
		graphics::GraphicsDevice::Default.renderState = graphics::RenderState(); /// ???
		graphics::GraphicsDevice::Default.renderState.init();
		graphics::GraphicsDevice::Default.create();
		EFFECTS_CALL_CREATE
		FORMATS_CALL_CREATE

		_textureFiltered = new graphics::TextureImage2d(false, true);
		_texture = new graphics::TextureImage2d(false, false);
		_textureFiltered->create();
		_texture->create();

		uint pixels[4] = { 
			0xffffffff, 0x00ffffff,
			0x00ffffff, 0xffffffff, };

		_textureFiltered->LoadPixels(2, 2, llge::Rgba, pixels);
		_texture->LoadPixels(2, 2, llge::Rgba, pixels);

	}

	

	void ZomboEditor::update(float ellapsedTime)
	{
		ZomboGameEnvironment::update(ellapsedTime);
		ZomboEditorRenderService::Default.resetBuffers();
		ZomboEditorCamera::Default.update();
		
		_mode->update();
	
		const char *zomboString = "Field of view";
		//float smoothScale =2.0f * ZomboEditorCamera::Default.getInterpoaltedScale() / ZomboEditorViewport::Default.h;
		float smoothScale = 2.0f / ZomboEditorViewport::Default.h;

		_bitmapFont->render(core::Vector3::empty, smoothScale, zomboString, &ZomboEditorBitmapFontRenderer::Default);
		
		float vectorScale = scaleFonts * 84.0f / 512.0f;
		//core::Vector2 size = _font->getSize(vectorScale, zomboString);
		//_font->renderTringlesEdges((size * -0.5f).toVector3(), 0.25f, zomboString, &ZomboEditorFontRenderer::Default);
		//_font->render((size * -0.5f).toVector3(), 0.25f, zomboString, &ZomboEditorFontRenderer::Default);
		//size.inverseX();
		//size.setY(0);
		core::Vector3 p = core::Vector3(0, 0.15f, 0);
		_font->renderTringles(p, vectorScale, zomboString, &ZomboEditorFontRenderer::Default);
		smoothScale = ZomboEditorCamera::Default.getInterpoaltedScale() / ZomboEditorViewport::Default.h;
		_font->renderTringlesSmooth(p, vectorScale, smoothScale, zomboString, &ZomboEditorFontRenderer::Default);
			
		/*
		ushort t0 = 1;
		ushort t1 = 65535;

		RenderVertex quad0[4] = 
		{ 
			RenderVertex(-3, -1, 0, 0xffffffff, t0, t0),
			RenderVertex(-3,  1, 0, 0xffffffff, t0, t1),
			RenderVertex(-1,  1, 0, 0xffffffff, t1, t1),
			RenderVertex(-1, -1, 0, 0xffffffff, t1, t0),
		};
		
		RenderVertex quad1[4] =
		{
			RenderVertex(1, -1, 0, 0xffffffff, t0, t0),
			RenderVertex(1,  1, 0, 0xffffffff, t0, t1),
			RenderVertex(3,  1, 0, 0xffffffff, t1, t1),
			RenderVertex(3, -1, 0, 0xffffffff, t1, t0),
		};

		ushort indices[6] = { 0, 1, 2, 0, 2, 3 };

		if (_bitmapFont->_textures.size() > 0)
		{
			ZomboEditorRenderService::Default.drawTrianglesTextured(_bitmapFont->_textures[0], quad0, 4, indices, 2);
		}
		if (_bitmapFont->_textures.size() > 1)
		{
			ZomboEditorRenderService::Default.drawTrianglesTextured(_bitmapFont->_textures[1], quad1, 4, indices, 2);
		}
		*/
	}

	ushort m = 65535;
	RenderVertex quad0[4] =
	{
		RenderVertex(-3, -1, 0, 0xffffffff, 0, 0),
		RenderVertex(-3,  1, 0, 0xffffffff, 0, m),
		RenderVertex(-1,  1, 0, 0xffffffff, m, 0),
		RenderVertex(-1, -1, 0, 0xffffffff, m, m),
	};
	ushort indices[6] = { 0, 1, 2, 0, 2, 3 };

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
