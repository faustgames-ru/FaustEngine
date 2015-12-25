#ifndef ZOMBO_EDITOR_RENDER_SERVICE_H
#define ZOMBO_EDITOR_RENDER_SERVICE_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"

namespace zombo
{
	class ZomboEditorRenderService
	{
	public:
		static ZomboEditorRenderService Default;
		void applyRenderCommands();
	};
}

#endif /* ZOMBO_EDITOR_RENDER_SERVICE_H */