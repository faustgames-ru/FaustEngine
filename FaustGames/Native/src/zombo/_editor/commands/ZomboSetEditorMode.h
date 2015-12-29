#ifndef ZOMBO_SET_EDITOR_MODE_H
#define ZOMBO_SET_EDITOR_MODE_H

#include "../../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "ZomboEditorCommand.h"

namespace zombo
{
	class ZomboSetEditorMode : public ZomboEditorCommand
	{
	public:
		std::string originMode;
		std::string targetMode;
		ZomboSetEditorMode(const std::string &mode);
		virtual bool isExecutionAvaliable() OVERRIDE;
		virtual bool isUndoAvaliable() OVERRIDE;
		virtual void execute() OVERRIDE;
		virtual void undo() OVERRIDE;
	};
}

#endif /* ZOMBO_SET_EDITOR_MODE_H */