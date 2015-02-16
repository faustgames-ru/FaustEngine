#include "VertexBuffer.h"
#include "Errors.h"

namespace graphics
{
	VertexBuffer::VertexBuffer() :_handler(0)
	{
	}
	
	VertexBuffer::~VertexBuffer()
	{
	}
	
	void VertexBuffer::create()
	{
		glGenBuffers(1, &_handler);
		Errors::check(Errors::GenBuffers);
	}
	
	void VertexBuffer::cleanup()
	{
		glDeleteBuffers(1, &_handler);
		Errors::check(Errors::DeleteBuffers);
	}

	void VertexBuffer::setData(void * data, int bytesCount)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _handler);
		Errors::check(Errors::BindBuffer);
		glBufferData(GL_ARRAY_BUFFER, bytesCount, data, GL_STATIC_DRAW);
		Errors::check(Errors::BufferData);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Errors::check(Errors::BindBuffer);
	}
}