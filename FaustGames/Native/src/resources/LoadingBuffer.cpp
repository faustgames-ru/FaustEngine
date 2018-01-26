#include "LoadingBuffer.h"

namespace resources
{
	LoadingBuffer::LoadingBuffer(void* buffer, int capacity)
	{
		_buffer = static_cast<byte*>(buffer);
		_capacity = capacity;
		_position = 0;
	}

	LoadingBuffer::~LoadingBuffer()
	{
	}

	void LoadingBuffer::skip(int bytesCount)
	{
		_position += bytesCount;
	}

	uint LoadingBuffer::readInt()
	{
		int newPosition = _position + sizeof(int);
		if (newPosition >= _capacity)
		{
			_position = _capacity;
			return 0;
		}
		uint result = *reinterpret_cast<uint *>(_buffer + _position);
		_position = newPosition;
		return result;
	}

	CompressedTextureHeader LoadingBuffer::readCompressedHeader()
	{
		CompressedTextureHeader result;
		auto pos = _position;
		uint formatAttributes = readInt();
		result.attributes = static_cast<ushort>(static_cast<uint>(formatAttributes) & 0xffff000000 >> 16);
		result.Format = static_cast<ushort>(static_cast<uint>(formatAttributes) & 0x0000ffff);
		result.CompressionPercent = readInt();
		int skipBytes = readInt();
		skip(skipBytes);
		result.OriginWidth = readInt();
		result.OriginHeight = readInt();
		result.BorderX = readInt();
		result.BorderY = readInt();
		result.RawDataOffset = (_position - pos) / 4;
		return result;
	}
}
