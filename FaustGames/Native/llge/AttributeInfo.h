#ifndef ATTRIBUTE_INFO_H
#define ATTRIBUTE_INFO_H

#include "graphics.h"

namespace graphics
{
	class AttributeInfo
	{
	private:	
		static GLuint _locationCounter;
		std::string _name;
		AttributeType::e _type;
		GLenum  _elementType;
		GLint _elementsCount;
		int _elementSize;
		int _elementsSize;
		GLboolean _normalized;
		GLuint _location;
		unsigned int _locationMask;
	public:
		AttributeInfo(const char *name, AttributeType::e type);
		const char *getName();
		const GLuint getLocation();
		const unsigned int getMask();
		inline const GLenum  getElementType(){ return _elementType; }
		inline const GLint getElementsCount(){ return _elementsCount; }
		inline const GLboolean getNormalized(){ return _normalized; }
		inline const int getElementsSize(){ return _elementsSize; }
	};
}

#endif /*ATTRIBUTE_INFO_H*/
