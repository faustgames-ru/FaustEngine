#include "VertexFormats.h"
#include "Attributes.h"

namespace graphics
{		
	VertexFormatPosition VertexFormats::_position;
	VertexFormatPositionColor VertexFormats::_positionColor;
	VertexFormatPositionTextureColor VertexFormats::_positionTextureColor;
	VertexFormatPositionColorTexture VertexFormats::_positionColorTexture;
	VertexFormatPositionNormal VertexFormats::_positionNormal;
	VertexFormatPositionTexture VertexFormats::_positionTexture;


	void VertexFormats::create()
	{
		_position.create();
		_positionColor.create();
		_positionTextureColor.create();
		_positionColorTexture.create();
		_positionNormal.create();
		_positionTexture.create();
	}


	VertexFormat * VertexFormats::position()
	{
		return &_position;
	}

	VertexFormat * VertexFormats::positionColor()
	{
		return &_positionColor;
	}

	VertexFormat * VertexFormats::positionTextureColor()
	{
		return &_positionTextureColor;
	}

	VertexFormat * VertexFormats::positionColorTexture()
	{
		return &_positionColorTexture;
	}


	VertexFormat * VertexFormats::positionNormal()
	{
		return &_positionNormal;
	}

	VertexFormat * VertexFormats::positionTexture()
	{
		return &_positionTexture;
	}


}