#include "ZomboSetEditorMode.h"
#include "../ZomboEditor.h"

namespace zombo
{
	ZomboSetEditorMode::ZomboSetEditorMode(const std::string& mode)
	{
		targetMode = mode;
	}

	bool ZomboSetEditorMode::isExecutionAvaliable()
	{
		return ZomboEditor::Default.getEditorModeInternal() != targetMode;
	}

	bool ZomboSetEditorMode::isUndoAvaliable()
	{
		return ZomboEditor::Default.getEditorModeInternal() != originMode;
	}

	void ZomboSetEditorMode::execute()
	{
		originMode = ZomboEditor::Default.getEditorModeInternal();
		ZomboEditor::Default.setEditorModeInternal(targetMode.c_str());
	}

	void ZomboSetEditorMode::undo()
	{
		ZomboEditor::Default.setEditorModeInternal(originMode.c_str());
		originMode = "";
	}
}
