#ifndef ZOMBO_EDITOR_LOADING_SCREEN_H
#define ZOMBO_EDITOR_LOADING_SCREEN_H

#include "../zombo_classes.h"
#include "curves/CurvesStateSelect.h"
#include "../content/ZomboBitmapFont.h"

namespace fonts{
	class BitmapFont;
}

namespace zombo
{
	class ZomboEditorLoadingScreen
	{
	public:
		static ZomboEditorLoadingScreen Default;
		ZomboBitmapFont* font;
		ZomboEditorLoadingScreen();
		void show();
		void showImmediately();
		void hide();
		void update();
		void load();
	private:
		std::string _loadingText;
		SFloat _alpha;
	};
}

#endif /*ZOMBO_EDITOR_LOADING_SCREEN_H*/
