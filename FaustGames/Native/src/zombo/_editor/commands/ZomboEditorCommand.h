#ifndef ZOMBO_EDITOR_COMMAND_H
#define ZOMBO_EDITOR_COMMAND_H

#include "../../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"

namespace zombo
{
	struct CommandExecutonStatus
	{
		enum e
		{
			Created,
			CommandExecutionNotAvaliable,
			CommandExecuted,
			CommandUndone
		};
	};

	class ZomboEditorCommand : public IBaseObject
	{
	public:
		CommandExecutonStatus::e status;
		ZomboEditorCommand();
		virtual bool isExecutionAvaliable();
		virtual bool isUndoAvaliable();
		virtual void execute() = 0;
		virtual void undo() = 0;
	};
}

#endif /* ZOMBO_EDITOR_COMMAND_H */