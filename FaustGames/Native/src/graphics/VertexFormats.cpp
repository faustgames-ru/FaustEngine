#include "VertexFormats.h"
#include "Attributes.h"

namespace graphics
{		
	VertexFormatPosition VertexFormats::_position;
	VertexFormatPositionColor VertexFormats::_positionColor;
	VertexFormatPositionTextureColor VertexFormats::_positionTextureColor;
	VertexFormatPositionNormal VertexFormats::_positionNormal;


	void VertexFormats::create()
	{
		_position.create();
		_positionColor.create();
		_positionTextureColor.create();
		_positionNormal.create();
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

	VertexFormat *VertexFormats::positionNormal()
	{
		return &_positionNormal;
	}

}