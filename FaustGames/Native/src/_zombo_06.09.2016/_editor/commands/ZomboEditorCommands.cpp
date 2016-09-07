#include "ZomboEditorCommands.h"
#include "../ZomboEditorInput.h"
#include "../menu/ContextMenu.h"

namespace zombo
{
	ZomboEditorCommands ZomboEditorCommands::Default;
	ZomboEditorCommands ZomboEditorCommands::Camera;

	ZomboEditorCommands* ZomboEditorCommands::camera()
	{
		//return &Camera;
		return &Default;
	}

	CommandExecutonStatus::e ZomboEditorCommands::doCommand(ZomboEditorCommand* command)
	{
		if (!command->isExecutionAvaliable())
		{
			command->status = CommandExecutonStatus::CommandExecutionNotAvaliable;
			return command->status;
		}
		ContextMenu::Default.hide();
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
		ContextMenu::Default.hide();
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
		if (_undoList.empty() || ZomboEditorInput::Default.mouse()->buttons != 0)
			return false;
		return _undoList.top()->isUndoAvaliable();
	}

	bool ZomboEditorCommands::isRedoAvaliable() const
	{
		if (_redoList.empty() || ZomboEditorInput::Default.mouse()->buttons != 0)
			return false;
		return _redoList.top()->isExecutionAvaliable();
	}
}
