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
	public:
		VertexFormat();
		~VertexFormat();
		VertexFormat *addAttribute(AttributeInfo *info);
		AttributeFormat *getFormat(GLuint location);
		inline int getStride(){ return _stride; }
		void create();
	private:
		AttributeFormat _attributes[32];
		core::StaticArray<AttributeInfo *, 32> _attributesInfos;
		int _stride;
	};
}

#endif /*VERTEX_FORMAT_H*/