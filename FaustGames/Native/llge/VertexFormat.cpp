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
		_attributesInfos.add(info);
		return this;
	}

	AttributeFormat *VertexFormat::getFormat(GLuint location)
	{
		return &(_attributes[location]);
	}

	void VertexFormat::create()
	{
		_stride = 0;
		for (int j = 0; j < _attributesInfos.count; j++)
		{
			AttributeInfo *info = _attributesInfos.data[j];
			int i = info->getLocation();
			_attributes[i].ElementsCount = info->getElementsCount();
			_attributes[i].ElementType = info->getElementType();
			_attributes[i].Normalized = info->getNormalized();
			_attributes[i].Offset = _stride;
			_stride += info->getElementsSize();
		}
	}


}