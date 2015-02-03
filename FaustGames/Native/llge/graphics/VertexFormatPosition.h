#ifndef VERTEX_FORMAT_POSITION_H
#define VERTEX_FORMAT_POSITION_H

#include "graphics_classes.h"
#include "Attributes.h"
#include "VertexFormat.h"

namespace graphics
{
	class VertexFormatPosition : public VertexFormat
	{
	public:
		VertexFormatPosition()
		{
			addAttribute(Attributes::position());
		}
	};
}

#endif /*VERTEX_FORMAT_POSITION_H*/
