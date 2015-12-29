#ifndef ZOMBO_EDITOR_SELECTION_MODE_H
#define ZOMBO_EDITOR_SELECTION_MODE_H

#include "../../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "../EditorMode.h"

namespace zombo
{
	class ZomboEditorSelectionMode: public IEditorMode
	{
	public:
		virtual void API_CALL activated() OVERRIDE;
		virtual void API_CALL deactivated() OVERRIDE;
		virtual void API_CALL update() OVERRIDE;
	};
}

#endif /*ZOMBO_EDITOR_SELECTION_MODE_H*/
