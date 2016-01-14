#ifndef FONTS_I_FONT_RENDERER_H
#define FONTS_I_FONT_RENDERER_H

#include "fonts_classes.h"

namespace fonts
{
	class IFontRenderer: IBaseObject
	{
	public:
		virtual void drawEdge(uint color, const core::Vector3 a, const core::Vector3 b) = 0;
		virtual void drawTriangle(uint color, core::Vector3 a, core::Vector3 b, core::Vector3 c) = 0;
		virtual void drawTriangle(uint ca, uint cb, uint cc, core::Vector3 a, core::Vector3 b, core::Vector3 c) = 0;
		virtual void drawQuad(uint ca, uint cb, uint cc, uint cd, core::Vector3 a, core::Vector3 b, core::Vector3 c, core::Vector3 d) = 0;
	};
}

#endif /*FONTS_I_FONT_RENDERER_H*/

