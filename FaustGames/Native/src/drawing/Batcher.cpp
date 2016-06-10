#include "Batcher.h"

namespace drawing
{
	BatchBuffer::BatchBuffer()
	{
		_verticesCount = 0;
		_indicesCount = 0;
		_x = 0.0f;
		_y = 0.0f;
		_w = 1.0f;
		_h = 1.0f;
		_vertices = static_cast<TVertex *>(malloc(VerticesLimit * sizeof(TVertex)));
		_indices = static_cast<ushort *>(malloc(IndicesLimit * sizeof(unsigned short)));
	}

	BatchBuffer::~BatchBuffer()
	{
		free(_vertices);
		free(_indices);
	}

	void BatchBuffer::reset()
	{
		_indicesCount = 0;
		_verticesCount = 0;
	}

	bool BatchBuffer::canAdd(int verticesCount, int indicesCount) const
	{
		return _indicesCount + indicesCount < IndicesLimit && _verticesCount + verticesCount < VerticesLimit;
	}

	void BatchBuffer::addMesh(uint color, float z, float* vertices, float* uvs, int verticesCount, int* indices, int indicesCount, bool additive)
	{
		for (int i = 0; i < indicesCount; i++)
		{
			_indices[_indicesCount + i] = static_cast<unsigned short>(_verticesCount + indices[i]);
		}

		TVertex* target = _vertices + _verticesCount;
		float* source = vertices;
		float* uvsource = uvs;
		for (int i = 0; i < verticesCount; i++, target++)
		{
			target->x = *source; ++source;
			target->y = *source; ++source;
			target->z = z;
			target->color = graphics::Color::premul(color, additive);
			target->u = _x + *uvsource *_w; ++uvsource;
			target->v = _y + *uvsource *_h; ++uvsource;
		}

		_verticesCount += verticesCount;
		_indicesCount += indicesCount;
	}

	void BatchBuffer::addMesh(TVertex* vertices, int verticesCount, ushort* indices, int indicesCount, bool additive)
	{
		for (int i = 0; i < indicesCount; i++)
		{
			_indices[_indicesCount + i] = static_cast<unsigned short>(_verticesCount + indices[i]);
		}

		TVertex* target = _vertices + _verticesCount;
		TVertex* source = vertices;
		for (int i = 0; i < verticesCount; i++, target++, source++)
		{
			target->x = source->x;
			target->y = source->y;
			target->z = source->z;
			target->u = _x + source->u *_w;
			target->v = _y + source->v *_h;
			target->color = graphics::Color::premul(source->color, additive);
		}

		_verticesCount += verticesCount;
		_indicesCount += indicesCount;
	}

	TVertex* BatchBuffer::getVertices()
	{
		return _vertices;
	}

	int BatchBuffer::getVerticesCount() const
	{
		return _verticesCount;
	}

	ushort* BatchBuffer::getCurrentIndices() const
	{
		return _indices + _indicesCount;
	}

	int BatchBuffer::getIndicesCount()
	{
		return _indicesCount;
	}

	RenderBuffer::RenderBuffer()
	{
		Entries.reserve(128);
		Transforms.reserve(128);
		Buffers.push_back(new BatchBuffer());
	}

	RenderBuffer::~RenderBuffer()
	{
		for (TBatchBuffers::iterator i = Buffers.begin(); i != Buffers.end(); ++i)
		{
			BatchBuffer *buffer = *i;
			delete buffer;
		}
	}

	bool BatcherState::isConfigEqual(void* otherConfig) const
	{
		return Effect->isConfigEqual(config, otherConfig);
	}

	BatcherSpineMesh::BatcherSpineMesh()
	{
		Color = 0;
		Vertices = nullptr;
		Uvs = nullptr;
		Z = 0;
		VerticesCount = 0;
		Indices = nullptr;
		IndicesCount = 0;
	}

	BatcherSpineMesh::BatcherSpineMesh(int verticesLimit)
	{
		Color = 0;
		Vertices = nullptr;
		Uvs = nullptr;
		Z = 0;
		VerticesCount = 0;
		Vertices = static_cast<float *>(core::Mem::allocate(verticesLimit * sizeof(float) * 2));
		Indices = nullptr;
		IndicesCount = 0;
	}

	void BatcherDebugRender::drawEdge(uint color, const core::Vector3& a, const core::Vector3& b)
	{
		_edges.push_back(SolidVertex(a, color));
		_edges.push_back(SolidVertex(b, color));
	}

	void BatcherDebugRender::apply(graphics::GraphicsDevice *graphicsDevice, core::MatrixContainer transform)
	{
		if (_edges.size() == 0) return;
		graphics::UniformValues::projection()->setValue(transform);
		graphicsDevice->renderState.setBlend(graphics::BlendState::Alpha);
		graphicsDevice->renderState.setDepth(graphics::DepthState::None);
		graphicsDevice->renderState.setEffect(graphics::Effects::solidColor());
		graphicsDevice->drawEdges(graphics::VertexFormats::positionColor(), _edges.data(), _edges.size() / 2);
		_edges.clear();
	}

	Batcher::Batcher() : 
		_tonemapId(0), 
		_batchBufferIndex(0), 
		_blend(graphics::BlendState::e::Alpha), 
		_effect(nullptr), 
		_format(graphics::VertexFormats::positionTextureColor())
	{
		core::DebugDraw::Default.Render = this;
		_buffer = new RenderBuffer();
		_localBuffer = static_cast<TVertex *>(malloc(graphics::GraphicsConstants::LocalBufferSize * sizeof(TVertex)));
		_graphicsDevice = &graphics::GraphicsDevice::Default;
		_x = 0;
		_y = 0;
		_w = 1;
		_h = 1;

	}
	
	Batcher::~Batcher()
	{
		core::DebugDraw::Default.Render = &core::DebugRenderDummy::Default;
		delete _buffer;
		free(_localBuffer);
	}
	
	void Batcher::start()
	{
		_buffer->Entries.clear();
		_buffer->Transforms.clear();
		_batchBufferIndex = 0;
		for (uint i = 0; i < CONFIG_MAX_SIZE; i++)
			_config[i] = 0;
		_buffer->Buffers[_batchBufferIndex]->reset();
		_currentEntry.IndicesCount = 0;
		_blend = graphics::BlendState::None;
		_effect = nullptr;
	}
	
	void Batcher::finish()
	{
		applyEntry();
	}
	
	void Batcher::drawMesh(graphics::EffectBase *effect, graphics::BlendState::e blend, llge::ITexture * texture, uint lightmapId, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount)
	{
		graphics::Texture * textureInstance = static_cast<graphics::Texture *>(texture->getTextureInstance());
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

		_lightingConfig.lightmap = lightmapId;
		_lightingConfig.texture = textureId;

		if (effect != _effect || needNewEntry || !effect->isConfigEqual(_config, &_lightingConfig))
		{
			if (_currentEntry.IndicesCount != 0)
				_buffer->Entries.push_back(_currentEntry);
			_currentEntry.IndicesStart = currentBuffer->getCurrentIndices();
			_currentEntry.IndicesCount = 0;
			_currentEntry.BatchBufferIndex = _batchBufferIndex;
			effect->configCopy(_currentEntry.Config, &_lightingConfig);
			effect->configCopy(_config, &_lightingConfig);
			//_currentEntry.TextureId = _textureId = textureId;
			//_currentEntry.LightmapId = _lightmapId = lightmapId;
			_currentEntry.Blend = _blend = blend;
			_currentEntry.Effect = _effect = effect;
			_currentEntry.TransformIndex = _buffer->Transforms.size() - 1;
			_currentEntry.RenderTargetIndex = _buffer->RenderTargets.size() - 1;
		}

		//_textureId = textureId;
		_currentEntry.IndicesCount += indicesCount;
		currentBuffer->_x = _x;
		currentBuffer->_y = _y;
		currentBuffer->_w = _w;
		currentBuffer->_h = _h;
		currentBuffer->addMesh(vertices, verticesCount, indices, indicesCount, blend == graphics::BlendState::Additive);
	}

	void Batcher::drawMesh(graphics::EffectBase* effect, graphics::BlendState::e blend, void* config, TVertex* vertices, int verticesCount, ushort* indices, int indicesCount)
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

		if (effect != _effect || needNewEntry || !effect->isConfigEqual(_config, config))
		{
			if (_currentEntry.IndicesCount != 0)
				_buffer->Entries.push_back(_currentEntry);
			_currentEntry.IndicesStart = currentBuffer->getCurrentIndices();
			_currentEntry.IndicesCount = 0;
			_currentEntry.BatchBufferIndex = _batchBufferIndex;
			effect->configCopy(_currentEntry.Config, config);
			effect->configCopy(_config, config);
			_currentEntry.Blend = _blend = blend;
			_currentEntry.Effect = _effect = effect;
			_currentEntry.TransformIndex = _buffer->Transforms.size() - 1;
			_currentEntry.RenderTargetIndex = _buffer->RenderTargets.size() - 1;
		}

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

		if (mesh.State.Effect != _effect || needNewEntry || !mesh.State.isConfigEqual(_config))
		{
			if (_currentEntry.IndicesCount != 0)
				_buffer->Entries.push_back(_currentEntry);
			_currentEntry.IndicesStart = currentBuffer->getCurrentIndices();
			_currentEntry.IndicesCount = 0;
			_currentEntry.BatchBufferIndex = _batchBufferIndex;
			mesh.State.Effect->configCopy(_currentEntry.Config, mesh.State.config);
			mesh.State.Effect->configCopy(_config, mesh.State.config);
			
			_currentEntry.Blend = _blend = mesh.State.Blend;
			_currentEntry.Effect = _effect = mesh.State.Effect;
			_currentEntry.TransformIndex = _buffer->Transforms.size() - 1;
			_currentEntry.RenderTargetIndex = _buffer->RenderTargets.size() - 1;
		}

		_currentEntry.IndicesCount += mesh.IndicesCount;
		currentBuffer->_x = _x;
		currentBuffer->_y = _y;
		currentBuffer->_w = _w;
		currentBuffer->_h = _h;
		currentBuffer->addMesh(mesh.Color, mesh.Z, mesh.Vertices, mesh.Uvs, mesh.VerticesCount, mesh.Indices, mesh.IndicesCount, mesh.State.Blend == graphics::BlendState::Additive);
	}


	void Batcher::executeRenderCommands(bool usePostProcess)
	{
		RenderBuffer * _backBuffer = _buffer;
		
		if (usePostProcess)
		{
			_bloom.beginRender(_tonemapId);
		}
		
		_graphicsDevice->clear();
		
		for (TBatchEntries::iterator i = _backBuffer->Entries.begin(); i != _backBuffer->Entries.end(); ++i)
		{
			BatchBuffer * currentBuffer = _backBuffer->Buffers[i->BatchBufferIndex];
			i->Effect->configApply(i->Config);
			graphics::UniformValues::projection()->setValue(_backBuffer->Transforms[i->TransformIndex]);

			_graphicsDevice->renderState.setBlend(graphics::BlendState::Alpha);
			_graphicsDevice->renderState.setEffect(i->Effect);
			memcpy(_localBuffer, currentBuffer->getVertices(), currentBuffer->getVerticesCount() * sizeof(TVertex));
			_graphicsDevice->drawPrimitives(_format, _localBuffer, i->IndicesStart, i->IndicesCount / 3);
		}
		
		
		if (usePostProcess)
		{
			_bloom.finishRender();
		}
		
		if (_backBuffer->Transforms.size() > 0)
		{
			_debugRender.apply(_graphicsDevice, _backBuffer->Transforms[0]);
		}
	}

	IntPtr Batcher::getNativeInstance()
	{
		return this;
	}

	void Batcher::applyEntry()
	{
		if (_currentEntry.IndicesCount != 0)
		{
			_buffer->Entries.push_back(_currentEntry);
			_currentEntry.IndicesCount = 0;
		}
	}

	void Batcher::addProjection(void* floatMatrix)
	{
		_projection.setValue(static_cast<float *>(floatMatrix));
		_buffer->Transforms.push_back(_projection);
		applyEntry();
	}

	void Batcher::addRenderTarget(IntPtr renderTargetInstance)
	{
		_buffer->RenderTargets.push_back(static_cast<graphics::IRenderTarget *>(renderTargetInstance));
		applyEntry();
	}

	void Batcher::startBatch()
	{
		start();
	}

	void Batcher::finishBatch()
	{
		finish();
	}

	void Batcher::drawEx(llge::GraphicsEffects effect, llge::BlendMode blendMode, IntPtr config, void* vertices, int verticesCount, void* indices, int indicesCount)
	{
		drawMesh(_converter.getEffect(effect), _converter.getBlend(blendMode), config, static_cast<TVertex *>(vertices), verticesCount, static_cast<ushort *>(indices), indicesCount);
	}

	void Batcher::draw(llge::GraphicsEffects effect, llge::BlendMode blendMode, llge::ITexture* texture, uint lightmapId, void* vertices, int verticesCount, void* indices, int indicesCount)
	{
		drawMesh(_converter.getEffect(effect), _converter.getBlend(blendMode), texture, lightmapId, static_cast<TVertex *>(vertices), verticesCount, static_cast<ushort *>(indices), indicesCount);
	}

	void Batcher::execute(bool usePostProcess)
	{
		executeRenderCommands(usePostProcess);
	}

	void Batcher::setToneMap(uint tonemapId)
	{
		_tonemapId = tonemapId;
	}

	void Batcher::drawEdge(uint color, const core::Vector3& a, const core::Vector3& b)
	{
		_debugRender.drawEdge(color, a, b);
	}
}