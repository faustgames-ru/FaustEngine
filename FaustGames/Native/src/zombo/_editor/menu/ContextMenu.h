#ifndef ZOMBO_CONTEXT_MENU_H
#define ZOMBO_CONTEXT_MENU_H

#include "../../zombo_classes.h"
#include "../curves/CurvesStateSelect.h"

namespace zombo
{
	class ContextMenu
	{
	public:
		static ContextMenu Default;
		ContextMenu();
		bool isVisible() const;
		void show(core::Vector2 guiPosition);
		void hide();
		void update();
	private:
		bool _visible;
		SFloat _alpha;
		SFloat _vignettingAlpha;
		SFloat _r;
		core::Vector2 _position;
	};
}

#endif /*ZOMBO_CONTEXT_MENU_H*/
