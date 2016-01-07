#include "ZomboEditorRenderService.h"

namespace zombo
{
	ZomboEditorRenderService ZomboEditorRenderService::Default;

	ZomboEditorRenderService::ZomboEditorRenderService(): _entriesCount(0)
	{
	}

	ZomboEditorRenderService::~ZomboEditorRenderService()
	{
		for (uint i = 0; i < _existingBuffers.size(); i++)
		{
			delete _existingBuffers[i];
		}
	}

	void ZomboEditorRenderService::draw(ZomboEditorPrimitivesType::e type, graphics::EffectBase* effect, void* vertices, int vertexSize, int verticesCount, ushort* indices, int primitivesCount)
	{
		if (_entries.size() <= _entriesCount)
			_entries.resize(_entriesCount + 1);
		uint indicesCount = primitivesCount * 2;
		ZomboEditorRenderBuffer* buffer = getBuffer(vertexSize, verticesCount, indicesCount);
		bool newEntry = _entriesCount == 0;
		int entriesHigh;
		if (_entriesCount > 0)
		{
			entriesHigh = _entriesCount - 1;
			if (_entries[entriesHigh].type != type)
				newEntry = true;
			if (_entries[entriesHigh].effect != effect)
				newEntry = true;
			if (_entries[entriesHigh].buffer != buffer)
				newEntry = true;
			if (_entries[entriesHigh].type != type)
				newEntry = true;
		}
		if (newEntry)
		{
			if (_entries.size() <= _entriesCount)
				_entries.resize(_entriesCount + 1);
			_entries[_entriesCount].indices = buffer->add(vertices, verticesCount, indices, indicesCount);
			_entries[_entriesCount].indicesCount = indicesCount;
			entriesHigh = _entriesCount;
			_entriesCount++;
		}
		else
		{
			buffer->add(vertices, verticesCount, indices, indicesCount);
			entriesHigh = _entriesCount - 1;
			_entries[entriesHigh].indicesCount += indicesCount;
		}
		_entries[entriesHigh].buffer = buffer;
		_entries[entriesHigh].effect = effect;
		_entries[entriesHigh].type = type;
	}

	void ZomboEditorRenderService::drawLines(ColorVertex* vertices, int verticesCount, ushort* indices, int primitivesCount)
	{
		draw(ZomboEditorPrimitivesType::Edges, graphics::EffectsBasic::solidColor(), vertices, sizeof(ColorVertex), verticesCount, indices, primitivesCount);
	}

	void ZomboEditorRenderService::applyRenderCommands()
	{
		for (uint i = 0; i < _entriesCount; i++)
		{
			graphics::GraphicsDevice::Default.renderState.setEffect(_entries[i].effect);
			switch (_entries[i].type)
			{
			case ZomboEditorPrimitivesType::None: break;
			case ZomboEditorPrimitivesType::Edges:
				graphics::GraphicsDevice::Default.drawEdgesPrimitives(graphics::VertexFormatsBasic::positionColor(), _entries[i].buffer->getBuffer(), _entries[i].indices, _entries[i].indicesCount / 2);
				break;
			case ZomboEditorPrimitivesType::Triangles: 
				graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormatsBasic::positionColorTexture(), _entries[i].buffer->getBuffer(), _entries[i].indices, _entries[i].indicesCount / 2);
				break;
			default: break;
			}
		}
		//graphics::GraphicsDevice::Default.drawEdgesPrimitives(graphics::VertexFormats::positionColor(), )
	}

	void ZomboEditorRenderService::resetBuffers()
	{
		for (uint i = 0; i < _existingBuffers.size(); i++)
			_existingBuffers[i]->reset();
		_entriesCount = 0;
	}

	ZomboEditorRenderBuffer* ZomboEditorRenderService::getBuffer(uint vertexSize, uint verticesCount, uint indicesCount)
	{
		ZomboEditorRenderBuffers* buffers = getBuffers(vertexSize);
		return buffers->getBuffer(verticesCount, indicesCount);
	}

	ZomboEditorRenderBuffers* ZomboEditorRenderService::getBuffers(uint vertexSize)
	{
		if (_buffers.size() < vertexSize)
			_buffers.resize(vertexSize + 1);
		ZomboEditorRenderBuffers*  buffers = _buffers[vertexSize];
		if (buffers == nullptr)
		{
			_buffers[vertexSize] = buffers = new ZomboEditorRenderBuffers(vertexSize);
			_existingBuffers.push_back(buffers);
		}
		return buffers;
	}

	ZomboEditorRenderEntry::ZomboEditorRenderEntry(): type(ZomboEditorPrimitivesType::None), effect(nullptr), buffer(nullptr), indices(nullptr), indicesCount(0)
	{
	}

	ZomboEditorRenderBuffers::ZomboEditorRenderBuffers(uint vertexSize) : _vertexSize(vertexSize), _count(0)
	{
	}

	ZomboEditorRenderBuffers::~ZomboEditorRenderBuffers()
	{
		for (uint i = 0; i < _items.size(); i++)
		{
			delete _items[i];
		}
	}

	ZomboEditorRenderBuffer* ZomboEditorRenderBuffers::getBuffer(uint verticesCount, uint indicesCount)
	{
		if (_items.size() <= _count)
		{
			_items.push_back(new ZomboEditorRenderBuffer(_vertexSize));
		}
		ZomboEditorRenderBuffer* buffer = _items[_count];
		if (buffer->canAdd(verticesCount, indicesCount))
		{
			return buffer;
		}
		_count++;
		if (_items.size() <= _count)
		{
			_items.push_back(new ZomboEditorRenderBuffer(_vertexSize));
		}
		return _items[_count];
	}

	void ZomboEditorRenderBuffers::reset()
	{
		for (uint i = 0; i < _count; i++)
		{
			_items[i]->reset();;
		}
		_count = 0;
	}

	uint ZomboEditorRenderBuffer::VerticesLimit = 32768;
	uint ZomboEditorRenderBuffer::IndicesLimit = 32768;


	ZomboEditorRenderBuffer::ZomboEditorRenderBuffer(uint vertexSize): _vertexSize(vertexSize), _verticesCount(0), _indicesCount(0)
	{
		_buffer = core::Mem::allocate(VerticesLimit * _vertexSize);
		_indices = static_cast<ushort *>(core::Mem::allocate(IndicesLimit*sizeof(ushort)));
	}

	ZomboEditorRenderBuffer::~ZomboEditorRenderBuffer()
	{
		core::Mem::deallocate(_buffer);
		core::Mem::deallocate(_indices);
	}

	bool ZomboEditorRenderBuffer::canAdd(uint verticesCount, uint indicesCount) const
	{
		return ((_verticesCount + verticesCount) < VerticesLimit) && ((_indicesCount + indicesCount) < IndicesLimit);
	}

	ushort* ZomboEditorRenderBuffer::add(void* vertices, uint verticesCount, ushort* indices, uint indicesCount)
	{
		ushort* result = _indices + _indicesCount;
		for (uint i = 0; i < indicesCount; i++)
		{
			_indices[_indicesCount + i] = _verticesCount + indices[i];
		}
		memcpy(static_cast<char *>(_buffer) + _verticesCount*_vertexSize, vertices, verticesCount*_vertexSize);
		_indicesCount += indicesCount;
		_verticesCount += verticesCount;
		return result;
	}

	void ZomboEditorRenderBuffer::reset()
	{
		_verticesCount = 0;
		_indicesCount = 0;
	}

	void* ZomboEditorRenderBuffer::getBuffer() const
	{
		return _buffer;
	}
}
