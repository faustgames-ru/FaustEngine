#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include "graphics.h"
#include "AttributeInfo.h"

namespace graphics
{
	class Attributes
	{
	private:
		static AttributeInfo _position;
		static AttributeInfo _textureCoords;
		static AttributeInfo _color;
	public:
		static AttributeInfo *position();
		static AttributeInfo *textureCoords();
		static AttributeInfo *color();
	};
}

#endif /*ATTRIBUTES_H*/