#ifndef BINARY_READER_H
#define BINARY_READER_H

#include "content_classes.h"

namespace content
{
	class BinaryReader
	{
	public:
		BinaryReader(const char* data);
		float readFloat();
		int readInt();
		ushort readUshort();
		char readChar();
		core::Vector3 readVector();
		std::string readString();
	private:
		int _position;
		const void* data() const;
		const char* _data;
	};
}

#endif /*BINARY_READER_H*/