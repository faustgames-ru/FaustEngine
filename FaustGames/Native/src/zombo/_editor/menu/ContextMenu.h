#ifndef ZOMBO_CONTEXT_MENU_H
#define ZOMBO_CONTEXT_MENU_H

#include "../../zombo_classes.h"
#include "../curves/CurvesStateSelect.h"

namespace zombo
{
	class ZomboContentBlock;

	class ContextMenu
	{
	public:
		static ContextMenu Default;
		ContextMenu();
		bool isVisible() const;
		static void queryResources();
		void load();
		void show(core::Vector2 guiPosition);
		void hide();
		void update();
	private:
		bool _visible;
		SFloat _alpha;
		SFloat _vignettingAlpha;
		SFloat _r;
		core::Vector2 _position;
		ZomboContentImage* _ringMenuImage;
	};

	class ContextMenuButton
	{
	public:
		ContextMenuButton(float angle);
		void update();
	private:
		float _angle;
		RenderVertex _vertices[3];		
		RenderVertex _verticesOrigin[3];
	};
}

#endif /*ZOMBO_CONTEXT_MENU_H*/
