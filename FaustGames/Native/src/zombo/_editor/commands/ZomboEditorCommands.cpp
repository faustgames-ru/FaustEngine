#include "ZomboEditorCommands.h"

namespace zombo
{
	ZomboEditorCommands ZomboEditorCommands::Default;

	CommandExecutonStatus::e ZomboEditorCommands::doCommand(ZomboEditorCommand* command)
	{
		if (!command->isExecutionAvaliable())
		{
			command->status = CommandExecutonStatus::CommandExecutionNotAvaliable;
			return command->status;
		}
		command->execute();
		command->status = CommandExecutonStatus::CommandExecuted;
		_undoList.push(command);
		while (!_redoList.empty())
		{
			delete _redoList.top();
			_redoList.pop();
		}
		return command->status;
	}

	void ZomboEditorCommands::undo()
	{
		if (!isUndoAvaliable()) return;
		ZomboEditorCommand* command = _undoList.top();
		_undoList.pop();
		command->undo();
		command->status = CommandExecutonStatus::CommandUndone;
		_redoList.push(command);
	}

	void ZomboEditorCommands::redo()
	{
		if (!isRedoAvaliable()) return;
		ZomboEditorCommand* command = _redoList.top();
		_redoList.pop();
		command->execute();
		command->status = CommandExecutonStatus::CommandExecuted;
		_undoList.push(command);
	}

	bool ZomboEditorCommands::isUndoAvaliable() const
	{
		if (_undoList.empty())
			return false;
		return _undoList.top()->isUndoAvaliable();
	}

	bool ZomboEditorCommands::isRedoAvaliable() const
	{
		if (_redoList.empty())
			return false;
		return _redoList.top()->isExecutionAvaliable();
	}
}