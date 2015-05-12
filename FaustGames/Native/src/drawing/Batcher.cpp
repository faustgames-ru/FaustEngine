#include "Batcher.h"

namespace drawing
{
	Batcher::Batcher() : _batchBufferIndex(0), _format(graphics::VertexFormats::positionTextureColor())
	{
		_buffer = new RenderBuffer();
		//_backBuffer = new RenderBuffer();
		_graphicsDevice = &graphics::GraphicsDevice::Default;
	}
	
	Batcher::~Batcher()
	{
		delete _buffer;
		//delete _backBuffer;
	}
	
	void Batcher::start()
	{
		_buffer->Entries.clear();
		_buffer->Transforms.clear();
		_batchBufferIndex = 0;
		_textureId = 0;
		_buffer->Buffers[_batchBufferIndex]->reset();
		_currentEntry.IndicesCount = 0;
		_blend = graphics::BlendState::None;
		_effect = 0;
	}
	
	void Batcher::finish()
	{
		applyEntry();
	}
	
	void Batcher::drawMesh(graphics::EffectBase *effect, graphics::BlendState::e blend, uint textureId, uint lightmapId, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount)
	{
		BatchBuffer * currentBuffer = _buffer->Buffers[_batchBufferIndex];
		bool needNewEntry = false;
		if (!currentBuffer->canAdd(verticesCount, indicesCount))
		{
			_batchBufferIndex++;
			if (_buffer->Buffers.size() <= _batchBufferIndex)
			{
				_buffer->Buffers.push_back(new BatchBuffer());
#ifdef __ANDROID__
				__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "new render buffer allocated");
#endif
			}

			currentBuffer = _buffer->Buffers[_batchBufferIndex];
			currentBuffer->reset();
			needNewEntry = true;
		}

		if ((textureId != _textureId) || (lightmapId != _lightmapId) || (effect != _effect) || (blend != _blend) || needNewEntry)
		{
			if (_currentEntry.IndicesCount != 0)
				_buffer->Entries.push_back(_currentEntry);
			_currentEntry.IndicesStart = currentBuffer->getCurrentIndices();
			_currentEntry.IndicesCount = 0;
			_currentEntry.BatchBufferIndex = _batchBufferIndex;
			_currentEntry.TextureId = _textureId = textureId;
			_currentEntry.LightmapId = _lightmapId = lightmapId;
			_currentEntry.Blend = _blend = blend;
			_currentEntry.Effect = _effect = effect;
			_currentEntry.TransformIndex = _buffer->Transforms.size() - 1;
		}

		_textureId = textureId;
		_currentEntry.IndicesCount += indicesCount;
		currentBuffer->addMesh(vertices, verticesCount, indices, indicesCount);
	}

	void Batcher::executeRenderCommands()
	{
		//RenderBuffer *swap = _buffer;
		//_buffer = _backBuffer;
		//_backBuffer = swap;
		RenderBuffer * _backBuffer = _buffer;
		for (TBatchEntries::iterator i = _backBuffer->Entries.begin(); i != _backBuffer->Entries.end(); i++)
		{
			BatchBuffer * currentBuffer = _backBuffer->Buffers[i->BatchBufferIndex];
			graphics::UniformValues::texture()->setValue(i->TextureId);
			graphics::UniformValues::lightmap()->setValue(i->LightmapId);
			graphics::UniformValues::projection()->setValue(_backBuffer->Transforms[i->TransformIndex]);
			_graphicsDevice->renderState.setBlend(i->Blend);
			_graphicsDevice->renderState.setEffect(i->Effect);
			_graphicsDevice->drawPrimitives(_format, currentBuffer->getVertices(), i->IndicesStart, i->IndicesCount / 3);
		}
	}

}