#include "ZomboEditorRenderService.h"

namespace zombo
{
	ZomboEditorRenderService ZomboEditorRenderService::Default;
	ZomboEditorFontRenderer ZomboEditorFontRenderer::Default;
	ZomboEditorBitmapFontRenderer ZomboEditorBitmapFontRenderer::Default;

	void ZomboEditorBitmapFontRenderer::drawQuad(uint color, graphics::Texture* texture, const fonts::BitmapFontVertex& min, const fonts::BitmapFontVertex& max, float z, bool rotate)
	{
		ushort t0 = 1;
		ushort t1 = 65535;

		if(rotate)
		{			
			vertices[0].xyz = core::Vector3(min.xy.getX(), min.xy.getY(), z);
			vertices[0].color = color;
			vertices[0].uv.u = max.u;
			vertices[0].uv.v = min.v;

			vertices[1].xyz = core::Vector3(min.xy.getX(), max.xy.getY(), z);;
			vertices[1].color = color;
			vertices[1].uv.u = min.u;
			vertices[1].uv.v = min.v;

			vertices[2].xyz = core::Vector3(max.xy.getX(), max.xy.getY(), z);;
			vertices[2].color = color;
			vertices[2].uv.u = min.u;
			vertices[2].uv.v = max.v;

			vertices[3].xyz = core::Vector3(max.xy.getX(), min.xy.getY(), z);;
			vertices[3].color = color;
			vertices[3].uv.u = max.u;
			vertices[3].uv.v = max.v;
		}
		else
		{			
			vertices[0].xyz = core::Vector3(min.xy.getX(), min.xy.getY(), z);
			vertices[0].color = color;
			vertices[0].uv.u = min.u;
			vertices[0].uv.v = min.v;

			vertices[1].xyz = core::Vector3(min.xy.getX(), max.xy.getY(), z);;
			vertices[1].color = color;
			vertices[1].uv.u = min.u;
			vertices[1].uv.v = max.v;

			vertices[2].xyz = core::Vector3(max.xy.getX(), max.xy.getY(), z);;
			vertices[2].color = color;
			vertices[2].uv.u = max.u;
			vertices[2].uv.v = max.v;

			vertices[3].xyz = core::Vector3(max.xy.getX(), min.xy.getY(), z);;
			vertices[3].color = color;
			vertices[3].uv.u = max.u;
			vertices[3].uv.v = min.v;
		}


		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 3;
		ZomboEditorRenderService::Default.drawTrianglesTextured(texture, vertices, 4, indices, 2);
	}

	void ZomboEditorFontRenderer::drawEdge(uint color, const core::Vector3 a, const core::Vector3 b)
	{
		vertices[0].xyz = a;
		vertices[0].color = color;
		vertices[1].xyz = b;
		vertices[1].color = color;

		indices[0] = 0;
		indices[1] = 1;
		ZomboEditorRenderService::Default.drawLines(vertices, 2, indices, 1);
	}

	void ZomboEditorFontRenderer::drawTriangle(uint color, core::Vector3 a, core::Vector3 b, core::Vector3 c)
	{
		vertices[0].xyz = a;
		vertices[0].color = color;
		vertices[1].xyz = b;
		vertices[1].color = color;
		vertices[2].xyz = c;
		vertices[2].color = color;

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		ZomboEditorRenderService::Default.drawTriangles(vertices, 3, indices, 1);
	}

	void ZomboEditorFontRenderer::drawTriangle(uint ca, uint cb, uint cc, core::Vector3 a, core::Vector3 b, core::Vector3 c)
	{
		vertices[0].xyz = a;
		vertices[0].color = ca;
		vertices[1].xyz = b;
		vertices[1].color = cb;
		vertices[2].xyz = c;
		vertices[2].color = cc;

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		ZomboEditorRenderService::Default.drawTriangles(vertices, 4, indices, 1);
	}

	void ZomboEditorFontRenderer::drawQuad(uint ca, uint cb, uint cc, uint cd, core::Vector3 a, core::Vector3 b, core::Vector3 c, core::Vector3 d)
	{
		vertices[0].xyz = a;
		vertices[0].color = ca;
		vertices[1].xyz = b;
		vertices[1].color = cb;
		vertices[2].xyz = c;
		vertices[2].color = cc;
		vertices[3].xyz = d;
		vertices[3].color = cd;

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 3;
		ZomboEditorRenderService::Default.drawTriangles(vertices, 4, indices, 2);
	}

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

	void ZomboEditorRenderService::draw(ZomboEditorPrimitivesType::e type, uint actualTexture, graphics::EffectBase* effect, void* vertices, int vertexSize, int verticesCount, ushort* indices, int primitivesCount)
	{
		if (_entries.size() <= _entriesCount)
			_entries.resize(_entriesCount + 1);

		int primitiveSize = 0;
		switch (type)
		{
		case ZomboEditorPrimitivesType::None: break;
		case ZomboEditorPrimitivesType::Edges:
			primitiveSize = 2;
			break;
		case ZomboEditorPrimitivesType::Triangles:
		case ZomboEditorPrimitivesType::TrianglesTextured:
			primitiveSize = 3;
			break;
		default: 
			primitiveSize = 0;
			break;
		}

		uint indicesCount = primitivesCount * primitiveSize;
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
			if (_entries[entriesHigh].texture != actualTexture)
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
		_entries[entriesHigh].texture = actualTexture;
	}

	void ZomboEditorRenderService::drawLines(ColorVertex* vertices, int verticesCount, ushort* indices, int primitivesCount)
	{
		draw(ZomboEditorPrimitivesType::Edges, 0, graphics::EffectsBasic::solidColor(), vertices, sizeof(ColorVertex), verticesCount, indices, primitivesCount);
	}

	void ZomboEditorRenderService::drawTriangles(ColorVertex* vertices, int verticesCount, ushort* indices, int primitivesCount)
	{
		draw(ZomboEditorPrimitivesType::Triangles, 0, graphics::EffectsBasic::solidColor(), vertices, sizeof(ColorVertex), verticesCount, indices, primitivesCount);
	}

	void ZomboEditorRenderService::drawTrianglesTextured(graphics::Texture* texture, RenderVertex* vertices, int verticesCount, ushort* indices, int primitivesCount)
	{
		draw(ZomboEditorPrimitivesType::TrianglesTextured, texture->getHandle(), graphics::EffectsBasic::textureColor(), vertices, sizeof(RenderVertex), verticesCount, indices, primitivesCount);
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
				graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormatsBasic::positionColor(), _entries[i].buffer->getBuffer(), _entries[i].indices, _entries[i].indicesCount / 3);
				break;
			case ZomboEditorPrimitivesType::TrianglesTextured:
				graphics::UniformValues::texture()->setValue(_entries[i].texture);
				graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormatsBasic::positionColorTexture(), _entries[i].buffer->getBuffer(), _entries[i].indices, _entries[i].indicesCount / 3);
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

	ZomboEditorRenderEntry::ZomboEditorRenderEntry(): type(ZomboEditorPrimitivesType::None), effect(nullptr), buffer(nullptr), indices(nullptr), indicesCount(0), texture(0)
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
		if (_items.size() <= _count) return;
		for (uint i = 0; i <= _count; i++)
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
