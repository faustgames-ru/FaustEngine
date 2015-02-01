#ifndef VERTEX_FORMATS_H
#define VERTEX_FORMATS_H

#include "graphics.h"
#include "VertexFormat.h"
#include "Lazy.h"

namespace graphics
{
	class VertexFormats
	{
	private:
		static core::Lazy<VertexFormats> _instance;
		
		static VertexFormats *instanciate();
		static void deinstanciate(VertexFormats *value);
		
		VertexFormat _position;
		VertexFormat _positionColor;
		VertexFormat _positionTextureColor;

		VertexFormats();
		~VertexFormats();
	public:
		static VertexFormats * instance();
		
		VertexFormat *position();
		VertexFormat *positionColor();
		VertexFormat *positionTextureColor();
	};
}

#endif /*VERTEX_FORMATS_H*/