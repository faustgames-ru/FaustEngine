#ifndef ZOMBO_EDITOR_H
#define ZOMBO_EDITOR_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "EditorMode.h"
#include "../../fonts/OutlineVectorFont.h"
#include "../../fonts/BitmapFont.h"
#include "../content/ZomboContentBlock.h"

namespace zombo
{
	class ZomboEditorConstants
	{
	public:
		static std::string LogDisplayerFont;
	};

	class ZomboEditor : public IZomboEditor
	{
	public:
		ZomboEditor();
		static ZomboEditor Default;
		String getEditorModeInternal() const;
		void setEditorModeInternal(String mode);
		ZomboContentBlock internalContent;

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
		void renderGui();		
		void internalUpdate();
		void contentLoaded();

		std::string _actualModeName;
		IEditorMode *_mode;
		std::string _rootPath;
		core::MatrixContainer _guiMatrix;
		bool _needCallContetnLoaded;

	};
}
#endif /* ZOMBO_EDITOR_H */