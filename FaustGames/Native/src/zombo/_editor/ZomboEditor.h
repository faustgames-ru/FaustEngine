#ifndef ZOMBO_EDITOR_H
#define ZOMBO_EDITOR_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"

namespace zombo
{
	class ZomboEditor : public IZomboEditor
	{
	public:
		static ZomboEditor Default;
		String getEditorModeInternal() const;
		void setEditorModeInternal(String mode);

		virtual IntPtr API_CALL getMode() OVERRIDE;
		virtual void API_CALL setMode(String modeName) OVERRIDE;
		virtual bool API_CALL isUndoAvaliable() OVERRIDE;
		virtual bool API_CALL isRedoAvaliable() OVERRIDE;
		virtual void API_CALL undo() OVERRIDE;
		virtual void API_CALL redo() OVERRIDE;
		virtual void API_CALL update() OVERRIDE;
		virtual void API_CALL render() OVERRIDE;
		virtual void API_CALL release() OVERRIDE;
	private:
		std::string _actualModeName;
	};
}

#endif /* ZOMBO_EDITOR_H */