#include "AttributeInfo.h"

namespace graphics
{
	GLuint AttributeInfo::_locationCounter(0);
	
	AttributeInfo::AttributeInfo(const char *name, AttributeType::e type) : _name(name), _type(type)
	{
		switch (_type)
		{
		case AttributeType::Float:
			_elementType = GL_FLOAT;
			_elementSize = 4;
			_elementsCount = 1;
			_normalized = false;
			break;
		case AttributeType::TextureCoordPacked:
			_elementType = GL_UNSIGNED_SHORT;
			_elementSize = 2;
			_elementsCount = 2;
			_normalized = true;
			break;
		case AttributeType::TextureCoord:
		case AttributeType::Vector2:
			_elementType = GL_FLOAT;
			_elementSize = 4;
			_elementsCount = 2;
			_normalized = false;
			break;
		case AttributeType::Vector3:
			_elementType = GL_FLOAT;
			_elementSize = 4;
			_elementsCount = 3;
			_normalized = false;
			break;
		case AttributeType::Vector4:
			_elementType = GL_FLOAT;
			_elementSize = 4;
			_elementsCount = 4;
			_normalized = false;
			break;
		case AttributeType::Color:
			_elementType = GL_UNSIGNED_BYTE;
			_elementSize = 1;
			_elementsCount = 4;
			_normalized = true;
			break;
		case AttributeType::Normal:
			_elementType = GL_BYTE;
			_elementSize = 1;
			_elementsCount = 4;
			_normalized = true;
			break;
			/*
		case AttributeType::TextureCoord:
			_elementType = GL_UNSIGNED_SHORT;
			_elementSize = 2;
			_elementsCount = 2;
			_normalized = true;
			break;
			*/
		default:
			break;
		}
		_elementsSize = _elementSize * _elementsCount;
		_location = _locationCounter;
		++_locationCounter;
		_locationMask = 0x1 << _location;
	}

	const char *AttributeInfo::getName()
	{
		return _name.c_str();
	}

	const GLuint AttributeInfo::getLocation()
	{
		return _location;
	}

	const unsigned int AttributeInfo::getMask()
	{
		return _locationMask;
	}
}