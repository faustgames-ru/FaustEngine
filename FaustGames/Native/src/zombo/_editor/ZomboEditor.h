#ifndef ZOMBO_EDITOR_H
#define ZOMBO_EDITOR_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"

namespace zombo
{
	class ZomboEditor : public IZomboEditor
	{
	public:
		virtual void API_CALL updateMouse(int w, int h, int x, int y, uint buttons) OVERRIDE;
		virtual void API_CALL render(int w, int h) OVERRIDE;
		virtual void API_CALL release() OVERRIDE;
	private:
	};
}

#endif /* ZOMBO_EDITOR_H */