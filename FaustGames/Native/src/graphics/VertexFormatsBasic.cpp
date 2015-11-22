#include "VertexFormatsBasic.h"
#include "Attributes.h"

namespace graphics
{		
	VertexFormatPositionColor VertexFormatsBasic::_positionColor;
	VertexFormatPositionColorTexturePacked VertexFormatsBasic::_positionColorTexture;


	void VertexFormatsBasic::create()
	{
		_positionColor.create();
		_positionColorTexture.create();
	}



	VertexFormat * VertexFormatsBasic::positionColor()
	{
		return &_positionColor;
	}

	VertexFormat * VertexFormatsBasic::positionColorTexture()
	{
		return &_positionColorTexture;
	}
}
