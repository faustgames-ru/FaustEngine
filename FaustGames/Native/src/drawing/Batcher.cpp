#include "Batcher.h"

namespace drawing
{
	Batcher::Batcher() : _batchBufferIndex(0), _format(graphics::VertexFormats::positionTextureColor())
	{
		_buffer = new RenderBuffer();
		_localBuffer = (TVertex *)malloc(graphics::GraphicsConstants::LocalBufferSize * sizeof(TVertex));
		//_backBuffer = new RenderBuffer();
		_graphicsDevice = &graphics::GraphicsDevice::Default;
		_x = 0;
		_y = 0;
		_w = 1;
		_h = 1;

	}
	
	Batcher::~Batcher()
	{
		delete _buffer;
		free(_localBuffer);
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
	
	void Batcher::drawMesh(graphics::EffectBase *effect, graphics::BlendState::e blend, llge::ITexture * texture, uint lightmapId, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount)
	{
		graphics::Texture * textureInstance = (graphics::Texture *)texture->getTextureInstance();
		_x = textureInstance->X;
		_y = textureInstance->Y;
		_w = textureInstance->W;
		_h = textureInstance->H;
		drawMesh(effect, blend, textureInstance->getHandle(), lightmapId, vertices, verticesCount, indices, indicesCount);
		_x = 0;
		_y = 0;
		_w = 1;
		_h = 1;
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

		if ((textureId != _textureId) || (lightmapId != _lightmapId) || (effect != _effect) || /*(blend != _blend) ||*/ needNewEntry)
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
		currentBuffer->_x = _x;
		currentBuffer->_y = _y;
		currentBuffer->_w = _w;
		currentBuffer->_h = _h;
		currentBuffer->addMesh(vertices, verticesCount, indices, indicesCount, blend == graphics::BlendState::Additive);
	}

	void Batcher::drawSpineMesh(const BatcherSpineMesh &mesh)
	{
		BatchBuffer * currentBuffer = _buffer->Buffers[_batchBufferIndex];
		bool needNewEntry = false;
		if (!currentBuffer->canAdd(mesh.VerticesCount, mesh.IndicesCount))
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

		if ((mesh.State.TextureId != _textureId) || (mesh.State.LightmapId != _lightmapId) || (mesh.State.Effect != _effect) || /*(mesh.State.Blend != _blend) ||*/ needNewEntry)
		{
			if (_currentEntry.IndicesCount != 0)
				_buffer->Entries.push_back(_currentEntry);
			_currentEntry.IndicesStart = currentBuffer->getCurrentIndices();
			_currentEntry.IndicesCount = 0;
			_currentEntry.BatchBufferIndex = _batchBufferIndex;
			_currentEntry.TextureId = _textureId = mesh.State.TextureId;
			_currentEntry.LightmapId = _lightmapId = mesh.State.LightmapId;
			_currentEntry.Blend = _blend = mesh.State.Blend;
			_currentEntry.Effect = _effect = mesh.State.Effect;
			_currentEntry.TransformIndex = _buffer->Transforms.size() - 1;
		}

		_textureId = mesh.State.TextureId;
		_currentEntry.IndicesCount += mesh.IndicesCount;
		currentBuffer->_x = _x;
		currentBuffer->_y = _y;
		currentBuffer->_w = _w;
		currentBuffer->_h = _h;
		currentBuffer->addMesh(mesh.Color, mesh.Z, mesh.Vertices, mesh.Uvs, mesh.VerticesCount, mesh.Indices, mesh.IndicesCount, mesh.State.Blend == graphics::BlendState::Additive);
	}


	void Batcher::executeRenderCommands(bool usePostProcess)
	{
		//RenderBuffer *swap = _buffer;
		//_buffer = _backBuffer;
		//_backBuffer = swap;
		RenderBuffer * _backBuffer = _buffer;
		graphics::TextureRenderTarget2d *target;
		if (usePostProcess)
		{
			_bloom.beginRender(_tonemapId);
		}
		_graphicsDevice->clear();
		for (TBatchEntries::iterator i = _backBuffer->Entries.begin(); i != _backBuffer->Entries.end(); i++)
		{
			BatchBuffer * currentBuffer = _backBuffer->Buffers[i->BatchBufferIndex];
			graphics::UniformValues::texture()->setValue(i->TextureId);
			graphics::UniformValues::lightmap()->setValue(i->LightmapId);
			graphics::UniformValues::projection()->setValue(_backBuffer->Transforms[i->TransformIndex]);
			//_graphicsDevice->renderState.setBlend(i->Blend);
			_graphicsDevice->renderState.setBlend(graphics::BlendState::Alpha);
			_graphicsDevice->renderState.setEffect(i->Effect);
			//_graphicsDevice->renderState.setEffect(graphics::Effects::textureColor());
			//_graphicsDevice->drawPrimitives(_format, currentBuffer->getVertices(), i->IndicesStart, i->IndicesCount / 3);
			
			memcpy(_localBuffer, currentBuffer->getVertices(), currentBuffer->getVerticesCount() * sizeof(TVertex));
			_graphicsDevice->drawPrimitives(_format, _localBuffer, i->IndicesStart, i->IndicesCount / 3);
		}
		if (usePostProcess)
		{
			_bloom.finishRender();
		}
	}

}