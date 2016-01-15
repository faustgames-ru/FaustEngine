#ifndef ZOMBO_EDITOR_H
#define ZOMBO_EDITOR_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "EditorMode.h"
#include "../../fonts/OutlineVectorFont.h"
#include "../../fonts/BitmapFont.h"

namespace zombo
{
	class ZomboEditor : public IZomboEditor
	{
	public:
		ZomboEditor();
		static ZomboEditor Default;
		String getEditorModeInternal() const;
		void setEditorModeInternal(String mode);

		virtual void API_CALL setRootPath(String rootPath) OVERRIDE;
		virtual IntPtr API_CALL getMode() OVERRIDE;
		virtual void API_CALL setMode(String modeName) OVERRIDE;
		virtual bool API_CALL isUndoAvaliable() OVERRIDE;
		virtual bool API_CALL isRedoAvaliable() OVERRIDE;
		virtual void API_CALL undo() OVERRIDE;
		virtual void API_CALL redo() OVERRIDE;
		virtual void API_CALL init() OVERRIDE;
		virtual void API_CALL update(float ellapsedTime) OVERRIDE;
		virtual void API_CALL render() OVERRIDE;
		virtual void API_CALL release() OVERRIDE;
	private:
		std::string _actualModeName;
		IEditorMode *_mode;
		std::string _rootPath;
		fonts::OutlineVectorFont *_font;
		fonts::BitmapFont *_bitmapFont;
		graphics::TextureImage2d * _textureFiltered;
		graphics::TextureImage2d * _texture;

	};
}

#endif /* ZOMBO_EDITOR_H */