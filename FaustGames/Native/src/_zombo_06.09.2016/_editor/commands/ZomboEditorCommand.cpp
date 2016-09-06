#include "ZomboEditorCommand.h"

namespace zombo
{
	ZomboEditorCommand::ZomboEditorCommand() : status(CommandExecutonStatus::Created)
	{
	}

	ZomboEditorCommand::~ZomboEditorCommand()
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
