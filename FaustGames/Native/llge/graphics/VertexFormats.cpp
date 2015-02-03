#include "VertexFormats.h"
#include "Attributes.h"

namespace graphics
{		
	VertexFormatPosition VertexFormats::_position;
	VertexFormatPositionColor VertexFormats::_positionColor;
	VertexFormatPositionTextureColor VertexFormats::_positionTextureColor;

	void VertexFormats::create()
	{
		_position.create();
		_positionColor.create();
		_positionTextureColor.create();
	}


	VertexFormat *VertexFormats::position()
	{
		return &_position;
	}

	VertexFormat *VertexFormats::positionColor()
	{
		return &_positionColor;
	}

	VertexFormat *VertexFormats::positionTextureColor()
	{
		return &_positionTextureColor;
	}

}