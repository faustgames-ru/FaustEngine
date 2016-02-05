#ifndef ZOMBO_EDITOR_DRAWING_DRAWER_H
#define ZOMBO_EDITOR_DRAWING_DRAWER_H

#include "../../zombo_classes.h"
#include "../ZomboEditorRenderService.h"

namespace zombo
{
	class ZomboDrawer
	{
	public:
		static ZomboDrawer Default;
		static ZomboDrawer Gui;
		ZomboDrawer(ZomboEditorRenderService *renderService);
		void setSmoothR(float smoothR);
		void setScaleToPixels(float scale);
		void fillCircle(uint color, const core::Vector3 &p, float r);
		void fillRing(uint color, const core::Vector3 &p, float r, float d);
		void fillRect(uint color, const core::Vector3 &p, float r, float angle);
		void fillEdge(uint color, const core::Vector3 &p0, const core::Vector3 &p1, float r);
		void fillArrow(uint color, const core::Vector3 &p0, const core::Vector3 &p1, float r);
		void drawEdge(uint color, const core::Vector3 &p0, const core::Vector3 &p1);
	private:
		ZomboEditorRenderService *_renderService;
		float _smoothR;
		float _scaleToPixels;
		std::vector<ColorVertex> _colorVertices;
		std::vector<ushort> _indices;
	};
}

#endif /*ZOMBO_EDITOR_DRAWING_DRAWER_H*/