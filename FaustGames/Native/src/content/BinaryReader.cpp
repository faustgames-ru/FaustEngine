#include "BinaryReader.h"

namespace content
{
	BinaryReader::BinaryReader(const char* data): _position(0), _data(data)
	{
	}

	float BinaryReader::readFloat()
	{
		float result = *static_cast<const float *>(data());
		_position += 4;
		return result;
	}

	int BinaryReader::readInt()
	{
		int result = *static_cast<const int *>(data());
		_position += 4;
		return result;
	}

	ushort BinaryReader::readUshort()
	{
		ushort result = *static_cast<const ushort *>(data());
		_position += 2;
		return result;
	}

	char BinaryReader::readChar()
	{
		char result = *static_cast<const char *>(data());
		_position += 1;
		return result;
	}

	core::Vector3 BinaryReader::readVector()
	{
		core::Vector3 v;
		v.setX(readFloat());
		v.setY(readFloat());
		v.setZ(readFloat());
		return v;
	}

	std::string BinaryReader::readString()
	{
		int size = readInt();
		std::string result;
		result.resize(size);
		for (int i = 0; i < size; i++)
			result[i] = readChar();
		return result;
	}

	const void* BinaryReader::data() const
	{
		return static_cast<const void *>(_data + _position);
	}
}

