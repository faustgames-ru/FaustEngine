#ifndef VERTEX_FORMATS_H
#define VERTEX_FORMATS_H

#include "graphics_classes.h"
#include "VertexFormat.h"
#include "VertexFormatPosition.h"
#include "VertexFormatPositionColor.h"
#include "VertexFormatPositionTextureColor.h"

namespace graphics
{
	class VertexFormats
	{
	public:
		static void create();
		static VertexFormat *position();
		static VertexFormat *positionColor();
		static VertexFormat *positionTextureColor();
		static VertexFormat *positionNormal();
		static VertexFormat *positionTexture();
	private:
		static VertexFormatPosition _position;
		static VertexFormatPositionColor _positionColor;
		static VertexFormatPositionTextureColor _positionTextureColor;
		static VertexFormatPositionNormal _positionNormal;
		static VertexFormatPositionTexture _positionTexture;
	};
}

#endif /*VERTEX_FORMATS_H*/