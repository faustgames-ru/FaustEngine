#ifndef VERTEX_FORMATS_BASIC_H
#define VERTEX_FORMATS_BASIC_H

#include "graphics_classes.h"
#include "VertexFormat.h"
#include "VertexFormatPosition.h"
#include "VertexFormatPositionColor.h"
#include "VertexFormatPositionTextureColor.h"

namespace graphics
{
	class VertexFormatsBasic
	{
	public:
		static void create();
		static VertexFormat *positionColor();
		static VertexFormat *positionColorTexture();
	private:
		static VertexFormatPositionColor _positionColor;
		static VertexFormatPositionColorTexturePacked _positionColorTexture;
	};
}

#endif /*VERTEX_FORMATS_BASIC_H*/