#ifndef ZOMBO_EDITOR_LOG_DISPLAYER_H
#define ZOMBO_EDITOR_LOG_DISPLAYER_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "../common/ZomboCamera.h"
#include "../content/ZomboBitmapFont.h"
#include "../common/ZomboInterpolatedValue.h"

namespace zombo
{
	class ZomboEditorLogDisplayer
	{
	public:
		ZomboEditorLogDisplayer();
		static ZomboEditorLogDisplayer Default;
		void load();
		void update();
	private:
		ZomboBitmapFont *_font;
		ZomboInterpolatedValue _yoffset;
		ZomboInterpolatedValue _xSize;
		ZomboInterpolatedValue _alpha;
	};
}

#endif /* ZOMBO_EDITOR_LOG_DISPLAYER_H */