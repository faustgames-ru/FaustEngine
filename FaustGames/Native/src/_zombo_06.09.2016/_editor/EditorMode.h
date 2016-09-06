#ifndef ZOMBO_EDITOR_MODE_H
#define ZOMBO_EDITOR_MODE_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"

namespace zombo
{
	class IEditorMode : public IBaseObject
	{
	public:
		virtual void API_CALL activated() = 0;
		virtual void API_CALL deactivated() = 0;
		virtual void API_CALL update() = 0;
	};
}

#endif /*ZOMBO_EDITOR_MODE_H*/
