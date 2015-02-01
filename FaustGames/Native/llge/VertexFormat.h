#ifndef VERTEX_FORMAT_H
#define VERTEX_FORMAT_H

#include "graphics.h"
#include "AttributeInfo.h"
#include "StaticArray.h"

namespace graphics
{
	struct AttributeFormat
	{
		GLenum  ElementType;
		GLint ElementsCount;
		GLboolean Normalized;
		GLuint Offset;
	};

	class VertexFormat
	{
	private:
		AttributeFormat _attributes[32];
		int _stride;
	public:
		VertexFormat();
		~VertexFormat();
		VertexFormat *addAttribute(AttributeInfo *info);
		AttributeFormat *getFormat(GLuint location);
		inline int getStride(){ return _stride; }
	};
}

#endif /*VERTEX_FORMAT_H*/