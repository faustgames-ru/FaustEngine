#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "graphics_classes.h"

namespace graphics
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();
		void create();
		void cleanup();
		void setData(void * data, int bytesCount);
		inline GLuint getHandle(){ return _handler; }
	private:
		GLuint _handler;
	};
}

#endif /*VERTEX_BUFFER_H*/