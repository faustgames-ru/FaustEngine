#ifndef VERTEX_FORMAT_POSITION_TEXTURE_COLOR_H
#define VERTEX_FORMAT_POSITION_TEXTURE_COLOR_H

#include "graphics_classes.h"
#include "Attributes.h"
#include "VertexFormat.h"

namespace graphics
{
	class VertexFormatPositionTextureColor : public VertexFormat
	{
	public:
		VertexFormatPositionTextureColor()
		{
			addAttribute(Attributes::position());
			addAttribute(Attributes::textureCoords());
			addAttribute(Attributes::color());
		}
	};

	class VertexFormatPositionNormal : public VertexFormat
	{
	public:
		VertexFormatPositionNormal()
		{
			addAttribute(Attributes::position());
			addAttribute(Attributes::normal());
		}
	};
}

#endif /*VERTEX_FORMAT_POSITION_TEXTURE_COLOR_H*/
