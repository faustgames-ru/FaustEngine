#include "VertexFormat.h"
#include "Errors.h"

namespace graphics
{
	VertexFormat::VertexFormat() : _stride(0)
	{
	}
	VertexFormat::~VertexFormat()
	{
	}

	VertexFormat *VertexFormat::addAttribute(AttributeInfo *info)
	{
		// todo: add attribute
		
		int i = info->getLocation();
		_attributes[i].ElementsCount = info->getElementsCount();
		_attributes[i].ElementType = info->getElementType();
		_attributes[i].Normalized = info->getNormalized();		
		_attributes[i].Offset = _stride;
		_stride += info->getElementsSize();
		return this;
	}

	AttributeFormat *VertexFormat::getFormat(GLuint location)
	{
		return &(_attributes[location]);
	}

}