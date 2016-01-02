#ifndef ZOMBO_EDITOR_VIEWPORT_H
#define ZOMBO_EDITOR_VIEWPORT_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "../common/ZomboCamera.h"

namespace zombo
{
	class ZomboEditorCamera
	{
	public:
		ZomboCamera camera;
	};

	class ZomboEditorViewport : public IZomboEditorViewport
	{
	public:
		static ZomboEditorViewport Default;
		int w;
		int h;
		virtual void API_CALL update(int width, int height) OVERRIDE;
	};
}

#endif /* ZOMBO_EDITOR_VIEWPORT_H */