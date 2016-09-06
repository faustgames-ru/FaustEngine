#ifndef ZOMBO_EDITOR_STATISTICS_DISPLAYER_H
#define ZOMBO_EDITOR_STATISTICS_DISPLAYER_H

#include "../content/ZomboBitmapFont.h"

namespace zombo
{
	class ZomboEditorStatisticsDisplayer
	{
	public:
		static ZomboEditorStatisticsDisplayer Default;
		ZomboEditorStatisticsDisplayer();
		void load();
		void update() const;
	private:
		ZomboBitmapFont *_font;
	};
}

#endif /* ZOMBO_EDITOR_STATISTICS_DISPLAYER_H */