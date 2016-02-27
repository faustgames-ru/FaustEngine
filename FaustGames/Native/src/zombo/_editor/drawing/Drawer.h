#ifndef ZOMBO_EDITOR_DRAWING_DRAWER_H
#define ZOMBO_EDITOR_DRAWING_DRAWER_H

#include "../../zombo_classes.h"
#include "../ZomboEditorRenderService.h"
#include "../../content/ZomboAtlas.h"

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
		void drawRect(uint color, const core::Vector3& p, const core::Vector2& size);
		void fillRect(uint color, const core::Vector3& p, const core::Vector2& size);
		void fillQuad(uint color, const core::Vector3& p, float r, float angle);
		void fillEdge(uint color, const core::Vector3 &p0, const core::Vector3 &p1, float r);
		void fillArrow(uint color, const core::Vector3 &p0, const core::Vector3 &p1, float r);
		void drawEdge(uint color, const core::Vector3 &p0, const core::Vector3 &p1);
		void drawSprite(uint color, float rotaion, float scale, const core::Vector3 &position, ZomboContentImage* image);
	private:
		ZomboEditorRenderService *_renderService;
		float _smoothR;
		float _scaleToPixels;
		std::vector<ColorVertex> _colorVertices;
		std::vector<ushort> _indices;
		std::vector<RenderVertex> _renderVertices;
	};
}

#endif /*ZOMBO_EDITOR_DRAWING_DRAWER_H*/