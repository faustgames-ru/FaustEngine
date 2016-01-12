#include "ZomboEditor.h"
#include "ZomboEditorViewport.h"
#include "ZomboEditorRenderService.h"
#include "commands/ZomboEditorCommands.h"
#include "commands/ZomboSetEditorMode.h"
#include "modes/ZomboEditorSelectionMode.h"
#include "ZomboEditorCamera.h"
#include "../common/ZomboGameEnvironment.h"

#include <ft2build.h>
#include FT_FREETYPE_H

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
		FT_Error error = FT_Init_FreeType(&library);
		if (error)
		{
			//... an error occurred during library initialization ...			
		}
		
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
		faceBuffer = static_cast<unsigned char*>(core::Mem::allocate(len));
		memcpy(faceBuffer, buffer, len);
		resources::ContentProvider::closeContent();

		error = FT_New_Memory_Face(library,
			faceBuffer,    /* first byte in memory */
			len,      /* size in bytes        */
			0,         /* face_index           */
			&face);
		if (error)
		{
			//...
		}

		error = FT_Set_Char_Size(
			face,    /* handle to face object           */
			0,       /* char_width in 1/64th of points  */
			16 * 64,   /* char_height in 1/64th of points */
			300,     /* horizontal device resolution    */
			300);   /* vertical device resolution      */

		if (error)
		{
			//...
		}

		int pen_x = 0;
		int pen_y = 0;
		std::string text = "Trololo";
		
		FT_GlyphSlot slot = face->glyph;

		for (int n = 0; n < text.size(); n++)
		{
			FT_UInt  glyph_index;


			/* retrieve glyph index from character code */
			glyph_index = FT_Get_Char_Index(face, text[n]);

			/* load glyph image into the slot (erase previous one) */
			error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
			if (error)
				continue;  /* ignore errors */

						   /* convert to an anti-aliased bitmap */
			error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
			if (error)
				continue;
			
			/* now, draw to our target surface */
			/*
			my_draw_bitmap(&slot->bitmap,
				pen_x + slot->bitmap_left,
				pen_y - slot->bitmap_top);
			*/
			/* increment pen position */
			pen_x += slot->advance.x >> 6;
			pen_y += slot->advance.y >> 6; /* not useful for now */
		}

		FT_Done_Face(face);
		core::Mem::deallocate(faceBuffer);

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
