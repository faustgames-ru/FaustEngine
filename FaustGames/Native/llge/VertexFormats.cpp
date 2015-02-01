#include "VertexFormats.h"
#include "Attributes.h"

namespace graphics
{		
	VertexFormats::VertexFormats()
	{
		_position.addAttribute(Attributes::position());

		_positionColor.addAttribute(Attributes::position());
		_positionColor.addAttribute(Attributes::color());

		_positionTextureColor.addAttribute(Attributes::position());
		_positionTextureColor.addAttribute(Attributes::textureCoords());
		_positionTextureColor.addAttribute(Attributes::color());
	}

	VertexFormats::~VertexFormats()
	{
	}
	
	VertexFormats * VertexFormats::instanciate()
	{
		return new VertexFormats();
	}
	void VertexFormats::deinstanciate(VertexFormats *value)
	{
		delete value;
	}

	core::Lazy<VertexFormats> VertexFormats::_instance(VertexFormats::instanciate, VertexFormats::deinstanciate);

	VertexFormats * VertexFormats::instance()
	{
		return _instance.value();
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