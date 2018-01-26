#ifndef LOADING_BUFFER_H
#define LOADING_BUFFER_H

#include "resources_classes.h"

namespace resources
{
	class LoadingBuffer
	{
	public:
		LoadingBuffer(void *buffer, int capacity);
		~LoadingBuffer();
		void skip(int bytesCount);
		uint readInt();
		CompressedTextureHeader readCompressedHeader();
	private:
		byte* _buffer;
		int _capacity;
		int _position;
	};
}

#endif /*LOADING_BUFFER_H*/