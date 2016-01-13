#ifndef FONTS_I_FONT_RENDERER_H
#define FONTS_I_FONT_RENDERER_H

#include "fonts_classes.h"

namespace fonts
{
	class IFontRenderer: IBaseObject
	{
	public:
		virtual void drawEdge(uint color, const core::Vector3 a, const core::Vector3 b) = 0;
	};
}

#endif /*FONTS_I_FONT_RENDERER_H*/

