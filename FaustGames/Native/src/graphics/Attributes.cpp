#include "Attributes.h"

namespace graphics
{
	AttributeInfo Attributes::_position("position", AttributeType::Vector3);
	AttributeInfo Attributes::_normal("normal", AttributeType::Vector3);
	AttributeInfo Attributes::_textureCoords("textureCoords", AttributeType::TextureCoord);
	AttributeInfo Attributes::_textureCoordsPacked("textureCoords", AttributeType::TextureCoordPacked);
	AttributeInfo Attributes::_color("color", AttributeType::Color);

	AttributeInfo *Attributes::position()
	{
		return &_position;
	}

	AttributeInfo *Attributes::normal()
	{
		return &_normal;
	}

	AttributeInfo *Attributes::textureCoords()
	{
		return &_textureCoords;
	}

	AttributeInfo *Attributes::textureCoordsPacked()
	{
		return &_textureCoordsPacked;
	}

	AttributeInfo *Attributes::color()
	{
		return &_color;
	}

}