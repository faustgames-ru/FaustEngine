#ifndef ZOMBO_EDITOR_COMMANDS_H
#define ZOMBO_EDITOR_COMMANDS_H

#include "../../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "ZomboEditorCommand.h"

namespace zombo
{
	class ZomboEditorCommands
	{
	public:
		static ZomboEditorCommands Default;
		//static ZomboEditorCommands Camera;
		static ZomboEditorCommands* camera();
		CommandExecutonStatus::e doCommand(ZomboEditorCommand *command);
		void undo();
		void redo();
		bool isUndoAvaliable() const;
		bool isRedoAvaliable() const;
	private:
		std::stack<ZomboEditorCommand *> _undoList;
		std::stack<ZomboEditorCommand *> _redoList;
	};
}

#endif /* ZOMBO_EDITOR_COMMANDS_H */