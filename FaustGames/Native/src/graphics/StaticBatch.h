#ifndef STATIC_BATCH_H
#define STATIC_BATCH_H

#include "graphics_classes.h"
#include "VertexBuffer.h"

namespace graphics
{
	template<typename T>
	struct DrawData
	{
		// vertices to push in vbo
		T *Vertices;
		int VerticesCount;

		// return indices
		unsigned short *Indices;
		int IndicesCount;
	};

	template<typename T>
	class BatchBlock
	{
	public:
		BatchBlock() : 
			_size(0), 
			_buffer(0)
		{
		}
		~BatchBlock()
		{
			if (_buffer)
				free(_buffer);
		}
		void create()
		{
			_vertexBuffer.create();
		}

		void addDrawData(const DrawData<T> &data)
		{
			if (!_buffer)
				_buffer = (T *)malloc(GraphicsConstants::StaticBatchBufferSize * sizeof(T));
			for (int i = 0; i < data.IndicesCount; i++)
			{
				data.Indices[i] += _size;
			}
			memcpy(_buffer + _size, data.Vertices, data.VerticesCount * sizeof(T));
			_size += data.VerticesCount;
		}
		void applyToVertexBuffer()
		{
			if (!_buffer) return;
			_vertexBuffer.setData(_buffer, _size * sizeof(T));
		}
		void reset()
		{
			if (!_buffer) return;
			free(_buffer);
			_buffer = 0;
			_size = 0;
		}
		void cleanup()
		{
			_vertexBuffer.cleanup();
		}
		inline int getSize(){ return _size; }
	private:
		VertexBuffer _vertexBuffer;
		T *_buffer;
		int _size;
	};

	template<typename T>
	class StaticBatch
	{
	public:
		StaticBatch()
		{
			_blocks[0] = _current = new BatchBlock<T>();
			_count = 1;
		}
		~StaticBatch()		
		{
			for (int i = 0; i < _count; i++)
			{
				delete _blocks[i];
			}
		}
		int addDrawData(const DrawData<T> &data)
		{
			int newSize = _current->getSize() + data.VerticesCount;
			if (newSize > GraphicsConstants::StaticBatchBufferSize)
			{
				_current = new BatchBlock<T>();
				_blocks[_count] = _current;
				++_count;
			}
			_current->addDrawData(data);
			return _count - 1;
		}
		void create()
		{
			for (int i = 0; i < GraphicsConstants::StaticBatchBlocksLimit; i++)
			{
				_blocks[i]->create();
			}
		}
		void applyToVertexBuffers()
		{
			for (int i = 0; i < _count; i++)
			{
				_blocks[i]->applyToVertexBuffers();
			}
		}

		void reset()
		{
			for (int i = 0; i < _count; i++)
			{
				_blocks[i]->reset();
			}
		}

		void cleanup()
		{
			for (int i = 0; i < GraphicsConstants::StaticBatchBlocksLimit; i++)
			{
				_blocks[i]->cleanup();
			}
		}
	private:
		BatchBlock<T> * _blocks[GraphicsConstants::StaticBatchBlocksLimit];
		int _count;
		BatchBlock<T> *_current;
	};
}

#endif /*STATIC_BATCH_H*/