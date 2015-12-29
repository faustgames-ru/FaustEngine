#include "ZomboEditorCommand.h"

namespace zombo
{
	ZomboEditorCommand::ZomboEditorCommand() : status(CommandExecutonStatus::Created)
	{
	}

	bool ZomboEditorCommand::isExecutionAvaliable()
	{
		return true;
	}

	bool ZomboEditorCommand::isUndoAvaliable()
	{
		return true;
	}
}
