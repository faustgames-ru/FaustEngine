#ifndef VERTEX_FORMAT_POSITION_COLOR_H
#define VERTEX_FORMAT_POSITION_COLOR_H

#include "graphics_classes.h"
#include "Attributes.h"
#include "VertexFormat.h"

namespace graphics
{
	class VertexFormatPositionColor : public VertexFormat
	{
	public:
		VertexFormatPositionColor()
		{
			addAttribute(Attributes::position());
			addAttribute(Attributes::color());
		}
	};
}

#endif /*VERTEX_FORMAT_POSITION_COLOR_H*/
