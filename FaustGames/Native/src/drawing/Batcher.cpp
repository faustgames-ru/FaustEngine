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

	void BatchBuffer::addMesh(uint color, float z, float* vertices, float* uvs, int verticesCount, ushort* indices, int indicesCount, bool additive, byte colorScale)
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
			target->color = graphics::Color::premul(color, colorScale, additive);
			target->u = _x + *uvsource *_w; ++uvsource;
			target->v = _y + *uvsource *_h; ++uvsource;
		}

		_verticesCount += verticesCount;
		_indicesCount += indicesCount;
	}

	void BatchBuffer::addMeshNotPremul(uint color, float z, float* vertices, float* uvs, int verticesCount, ushort* indices, int indicesCount, bool additive, byte colorScale)
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
			target->color = color;
			target->u = _x + *uvsource *_w; ++uvsource;
			target->v = _y + *uvsource *_h; ++uvsource;
		}

		_verticesCount += verticesCount;
		_indicesCount += indicesCount;
	}

	void BatchBuffer::addMesh(uint color, float z, float* vertices, float* uvs, int verticesCount, ushort* indices, int indicesCount, bool additive, core::Matrix viewTransform, byte colorScale)
	{
		for (int i = 0; i < indicesCount; i++)
		{
			_indices[_indicesCount + i] = static_cast<unsigned short>(_verticesCount + indices[i]);
		}

		TVertex* target = _vertices + _verticesCount;
		float* source = vertices;
		float* uvsource = uvs;
		core::Vector3 uv;
		for (int i = 0; i < verticesCount; i++, target++)
		{
			target->x = *source; ++source;
			target->y = *source; ++source;
			target->z = z;
			target->color = graphics::Color::premul(color, colorScale, additive);
			target->u = _x + *uvsource *_w; ++uvsource;
			target->v = _y + *uvsource *_h; ++uvsource;
			//uv = core::Matrix::transform(viewTransform, core::Vector3(target->x, target->y, target->z));
			//uv = (uv + core::Vector3(1.0f, 1.0f, 0.0f))*0.5f;
			//target->u = uv.getX();
			//target->v = uv.getY();
		}

		_verticesCount += verticesCount;
		_indicesCount += indicesCount;
	}


	void BatchBuffer::addMesh(TVertex* vertices, int verticesCount, ushort* indices, int indicesCount, bool additive, unsigned char colorScale)
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
			target->color = graphics::Color::premul(source->color, colorScale, additive);
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
		texture = nullptr;
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
		texture = nullptr;
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
		_format(graphics::VertexFormats::positionTextureColor()),
		_verticesCounter(0),
		_usedPostProcess(false)
	{
		core::DebugDraw::Default.Render = this;
		_buffer = new RenderBuffer();
		//_localBuffer = static_cast<TVertex *>(malloc(graphics::GraphicsConstants::LocalBufferSize * sizeof(TVertex)));
		_graphicsDevice = &graphics::GraphicsDevice::Default;
		_x = 0;
		_y = 0;
		_w = 1;
		_h = 1;
		_zButcher = new ZBatcher();
	}
	
	Batcher::~Batcher()
	{
		core::DebugDraw::Default.Render = &core::DebugRenderDummy::Default;
		delete _buffer;
		delete _zButcher;
		//free(_localBuffer);
	}
	
	void Batcher::start()
	{
		_verticesCounter = 0;
		_buffer->Entries.clear();
		_buffer->Transforms.clear();
		_batchBufferIndex = 0;
		for (uint i = 0; i < CONFIG_MAX_SIZE; i++)
			_config[i] = 0;
		_buffer->Buffers[_batchBufferIndex]->reset();
		_currentEntry.IndicesCount = 0;
		_blend = graphics::BlendState::None;
		_effect = nullptr;
		_zButcher->reset();
	}
	
	void Batcher::finish()
	{
		applyEntry();
	}
	
	void Batcher::drawMesh(graphics::EffectBase *effect, graphics::BlendState::e blend, llge::ITexture * texture, uint lightmapId, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount, float colorScale)
	{
		_verticesCounter += verticesCount;
		setupUVTransform(texture);
		drawMesh(effect, blend, texture->getId(), lightmapId, vertices, verticesCount, indices, indicesCount, colorScale);
		cleanupUVTransform();
	}
	
	void Batcher::drawMesh(graphics::EffectBase *effect, graphics::BlendState::e blend, uint textureId, uint lightmapId, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount, unsigned char colorScale)
	{
		_verticesCounter += verticesCount;
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
		currentBuffer->addMesh(vertices, verticesCount, indices, indicesCount, blend == graphics::BlendState::Additive, colorScale);
	}

	void Batcher::drawMesh(graphics::EffectBase* effect, graphics::BlendState::e blend, void* config, TVertex* vertices, int verticesCount, ushort* indices, int indicesCount, unsigned char colorScale)
	{
		_verticesCounter += verticesCount;
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
		currentBuffer->addMesh(vertices, verticesCount, indices, indicesCount, blend == graphics::BlendState::Additive, colorScale);
	}

	void Batcher::drawSpineMesh(const BatcherSpineMesh &mesh, byte colorScale, bool pemul)
	{		
		if (mesh.texture != nullptr)
		{
			setupUVTransform(mesh.texture);
		}

		if (_bloom.getBlurMap() != nullptr)
		{
			graphics::EffectTextureBlurmap::blurmap = _bloom.getBlurMap()->getHandle();
		}


		_verticesCounter += mesh.VerticesCount;
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
		if (pemul)
		{
			currentBuffer->addMesh(mesh.Color, mesh.Z, mesh.Vertices, mesh.Uvs, mesh.VerticesCount, mesh.Indices, mesh.IndicesCount, mesh.State.Blend == graphics::BlendState::Additive, colorScale);
		}
		else
		{
			currentBuffer->addMeshNotPremul(mesh.Color, mesh.Z, mesh.Vertices, mesh.Uvs, mesh.VerticesCount, mesh.Indices, mesh.IndicesCount, mesh.State.Blend == graphics::BlendState::Additive, colorScale);
		}
	}

	void Batcher::setupUVTransform(llge::ITexture* texture)
	{
		graphics::Texture * textureInstance = static_cast<graphics::Texture *>(texture->getTextureInstance());
		_x = textureInstance->X;
		_y = textureInstance->Y;
		_w = textureInstance->W;
		_h = textureInstance->H;
	}

	void Batcher::cleanupUVTransform()
	{
		_x = 0.0f;
		_y = 0.0f;
		_w = 1.0f;
		_h = 1.0f;
	}

	void Batcher::executeRenderCommands(bool usePostProcess)
	{
		_usedPostProcess = usePostProcess;
		_primitivesCount = 0;
		_verticesCount = 0;
		int verticesCount;
		int primitivesCount;

		RenderBuffer * _backBuffer = _buffer;
		//graphics::Texture *blurmap = _bloom.getBlurMap();
		if (usePostProcess && _bloom.isAvaliable())
		{
			_bloom.beginRender(_tonemapId);
			/*
			if (blurmap != nullptr)
			{
				graphics::EffectTextureBlurmap::blurmap = blurmap->getHandle();
			}
			*/
		}
		else
		{			
			_graphicsDevice->clear();
		}

		if (_buffer->Transforms.size() > 0)
		{
			glDepthFunc(GL_LESS);
			_zButcher->configure(graphics::BlendState::Alpha, _converter.getEffect(llge::EffectTextureColor), _buffer->Transforms[0]);
			_zButcher->applyRender();
		}
		glDepthFunc(GL_LEQUAL);
		bool _postEffectFinished = false;
		for (TBatchEntries::iterator i = _backBuffer->Entries.begin(); i != _backBuffer->Entries.end(); ++i)
		{
				
			if (i->TransformIndex > 0)
			{
				_graphicsDevice->clearDepth();
				if (usePostProcess && !_postEffectFinished && _bloom.isAvaliable())
				{
					_bloom.finishRender();
					_postEffectFinished = true;
				}
			}
			
			BatchBuffer * currentBuffer = _backBuffer->Buffers[i->BatchBufferIndex];
			/*
			if (blurmap != nullptr && !_postEffectFinished)
			{
				i->Effect = graphics::Effects::textureBlurmap();
			}
			*/
			i->Effect->configApply(i->Config);

			graphics::UniformValues::projection()->setValue(_backBuffer->Transforms[i->TransformIndex]);
			//graphics::UniformValues::fogStart()->setValue(50.0f);
			//graphics::UniformValues::fogDensity()->setValue(0.00005f);
			//graphics::UniformValues::fogColor()->setValue(core::Vector3(0.1, 0.6, 0.7));

			_graphicsDevice->renderState.setEffect(i->Effect);
			primitivesCount = i->IndicesCount / 3;
			verticesCount = currentBuffer->getVerticesCount();
			
			_primitivesCount += primitivesCount;
			_verticesCount += verticesCount;
			_graphicsDevice->renderState.setBlend(graphics::BlendState::Alpha);
			_graphicsDevice->renderState.setDepth(graphics::DepthState::Read);
			_graphicsDevice->drawPrimitives(_format, currentBuffer->getVertices(), i->IndicesStart, primitivesCount);
		}
		_verticesCount = _verticesCounter;
		_verticesCounter = 0;
		
		if (usePostProcess && !_postEffectFinished && _bloom.isAvaliable())
		{
			_bloom.finishRender();
		}

		if (_backBuffer->Transforms.size() > 0)
		{
			_debugRender.apply(_graphicsDevice, _backBuffer->Transforms[0]);
		}

		_graphicsDevice->renderState.setDepth(graphics::DepthState::None);
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

	void Batcher::draw(IntPtr batcherConfig, IntPtr texturesConfig)
	{
		llge::BatcherConfig* bc = static_cast<llge::BatcherConfig*>(batcherConfig);
		setupUVTransform(static_cast<graphics::Texture*>(bc->texture));
		drawMesh(
			_converter.getEffect(static_cast<llge::GraphicsEffects>(bc->effect)),
			_converter.getBlend(static_cast<llge::BlendMode>(bc->blendMode)),
			texturesConfig,
			static_cast<TVertex *>(bc->vertices),
			bc->verticesCount,
			static_cast<ushort *>(bc->indices),
			bc->indicesCount,
			static_cast<byte>(bc->colorScale));
		cleanupUVTransform();
	}
	/*
	void Batcher::drawEx(llge::GraphicsEffects effect, llge::BlendMode blendMode, IntPtr config, void* vertices, int verticesCount, void* indices, int indicesCount, unsigned char colorScale)
	{
		drawMesh(_converter.getEffect(effect), _converter.getBlend(blendMode), config, static_cast<TVertex *>(vertices), verticesCount, static_cast<ushort *>(indices), indicesCount, colorScale);
	}
	*/
	void Batcher::drawSolid(int z, llge::ITexture* texture, uint lightmapId, void* vertices, int verticesCount, void* indices, int indicesCount, byte colorScale)
	{
		if (graphics::GraphicsDevice::Default.config.earlyDepthPath)
		{
			graphics::Texture * textureInstance = static_cast<graphics::Texture *>(texture->getTextureInstance());

			_zButcher->_x = textureInstance->X;
			_zButcher->_y = textureInstance->Y;
			_zButcher->_w = textureInstance->W;
			_zButcher->_h = textureInstance->H;

			_zButcher->drawMesh(z, texture, lightmapId, static_cast<TVertex *>(vertices), verticesCount, static_cast<ushort *>(indices), indicesCount, colorScale);
		}
		else
		{
			setupUVTransform(texture);
			drawMesh(_converter.getEffect(llge::GraphicsEffects::EffectTextureColor), graphics::BlendState::Alpha, texture, lightmapId, static_cast<TVertex *>(vertices), verticesCount, static_cast<ushort *>(indices), indicesCount, colorScale);
			cleanupUVTransform();
		}
	}

	void Batcher::execute(bool usePostProcess)
	{
		executeRenderCommands(usePostProcess);
	}

	void Batcher::setToneMap(uint tonemapId)
	{
		_tonemapId = tonemapId;
	}

	int Batcher::getRenderedVerticesCount()
	{
		return _verticesCount;
	}

	int Batcher::getRenderedPrimitivesCount()
	{
		return _primitivesCount;
	}

	void Batcher::drawEdge(uint color, const core::Vector3& a, const core::Vector3& b)
	{
		_debugRender.drawEdge(color, a, b);
	}

	bool Batcher::usedPostProcess()
	{
		return _usedPostProcess && _bloom.isAvaliable() && _bloom.getBlurMap() != nullptr;
	}

	graphics::Texture* Batcher::getBlurMap()
	{
		return _bloom.getBlurMap();
	}

	bool ZBatchEntry::equals(const ZBatchEntry& entry)
	{
		if (entry.originVertices != originVertices) return false;

		return graphics::Effects::textureColorFog()->isConfigEqual(&entry.Config, &Config);
	}

	void ZBlock::reconstruct(int zLevel)
	{
		z = zLevel;
		Buffer.reset();
		Entries.clear();
	}

	void ZBlock::addMesh(llge::ITexture* texture, uint lightmapId, TVertex* vertices, int verticesCount, ushort* indices, int indicesCount, byte colorScale)
	{
		ZBatchEntry e;
		Buffer._x = _x;
		Buffer._y = _y;
		Buffer._w = _w;
		Buffer._h = _h;
		Buffer.add(vertices, verticesCount, indices, indicesCount, colorScale, e);
		llge::LightingConfig* config = static_cast<llge::LightingConfig *>(static_cast<void *>(&e.Config));
		config->lightmap = lightmapId;
		config->texture = texture->getId();
		if(Entries.size() == 0)
		{
			Entries.push_back(e);
		}
		else if (Entries.back().equals(e))
		{
			Entries.back().indicesCount += indicesCount;
			Entries.back().verticesCount += verticesCount;
		}
		else
		{
			Entries.push_back(e);
		}
	}

	void ZBlock::applyRender(graphics::EffectBase *effect)
	{
		graphics::GraphicsDevice* graphicsDevice = &graphics::GraphicsDevice::Default;
		uint primitivesCount;
		for (std::vector<ZBatchEntry>::iterator i = Entries.begin(); i != Entries.end(); ++i)
		{
			effect->configApply(i->Config);
			primitivesCount = i->indicesCount / 3;
			graphicsDevice->drawPrimitives(graphics::VertexFormats::positionTextureColor(), i->originVertices, Buffer.allIndices() + i->indicesStart, primitivesCount);
		}
	}

	ZBlocksPool::ZBlocksPool(): _blockSize(128)
	{
		_blocks.push_back(new ZBlock[_blockSize]);
		_blocksIndex = 0;
		_blockIndex = 0;
	}

	ZBlocksPool::~ZBlocksPool()
	{
		for (uint i = 0; i < _blocks.size(); i++)
			delete [] _blocks[i];
	}

	void ZBlocksPool::reset()
	{
		_blocksIndex = 0;
		_blockIndex = 0;
	}

	ZBlock* ZBlocksPool::queryBlock()
	{
		if (_blockIndex > _blockSize)
		{
			_blocksIndex = _blocks.size();
			_blocks.push_back(new ZBlock[_blockSize]);
			_blockIndex = 0;
		}
		ZBlock* result = _blocks[_blocksIndex] + _blockIndex;
		_blockIndex++;
		return result;
	}

	ZBatchBuffer::ZBatchBuffer()
	{
		_blockSize = 32768;
		_verticesIndex = 0;
		_verticesBufferIndex = 0;
		_indices.reserve(_blockSize);
		_x = 0.0f;
		_y = 0.0f;
		_w = 1.0f;
		_h = 1.0f;
	}

	ZBatchBuffer::~ZBatchBuffer()
	{
		for (uint i = 0; i < _vertices.size(); i++)
			delete[] _vertices[i];
	}

	void ZBatchBuffer::reset()
	{
		_indices.clear();
		_verticesIndex = 0;
		_verticesBufferIndex = 0;
	}

	void ZBatchBuffer::add(TVertex* vertices, int verticesCount, ushort* indices, int indicesCount, byte colorScale, ZBatchEntry& result)
	{
		int newVSize = _verticesIndex + verticesCount;
		if (newVSize > _blockSize)
		{
			_verticesBufferIndex++;
			_verticesIndex = 0;
		}
		if (_verticesBufferIndex >= _vertices.size())
		{
			_vertices.push_back(new TVertex[_blockSize]);
		}

		TVertex* target = _vertices[_verticesBufferIndex] + _verticesIndex;
		TVertex* source = vertices;
		for (int i = 0; i < verticesCount; ++i, ++target, ++source)
		{
			target->x = source->x;
			target->y = source->y;
			target->z = source->z;
			target->color = graphics::Color::premul(source->color, colorScale, false);
			target->u = _x + source->u*_w;
			target->v = _y + source->v*_h;
		}

		result.originVertices = _vertices[_verticesBufferIndex];
		result.verticesStart = _verticesIndex;
		result.verticesCount = verticesCount;
		result.indicesCount = indicesCount;
		result.indicesStart = _indices.size();
		for (uint i = 0; i < indicesCount; i++)
		{
			_indices.push_back(static_cast<ushort>(_verticesIndex + static_cast<int>(indices[i])));
		}
		_verticesIndex += verticesCount;

	}

	ushort* ZBatchBuffer::allIndices()
	{
		return _indices.data();
	}

	void ZBatcher::configure(graphics::BlendState::e blend, graphics::EffectBase* effect, core::MatrixContainer transform)
	{
		_blend = blend;
		_effect = effect;
		_transform = transform;
	}

	void ZBatcher::reset()
	{
		_blocksPool.reset();
		_blocks.clear();
		_verticesCounter = 0;
		_indicesCounter = 0;
	}

	struct 
	{
		bool operator()(ZBlock* a, ZBlock* b)
		{
			return a->z < b->z;
		}
	} ZBlockComparer;

	void ZBatcher::applyRender()
	{
		_blocksList.clear();
		for (BlocksMap::iterator it = _blocks.begin(); it != _blocks.end(); ++it)
		{
			_blocksList.push_back(it->second);
		}
		std::sort(_blocksList.begin(), _blocksList.end(), ZBlockComparer);
		if (_blocksList.size() == 0) return;
		graphics::GraphicsDevice* graphicsDevice = &graphics::GraphicsDevice::Default;
		graphicsDevice->renderState.setDepth(graphics::DepthState::ReadWrite);
		graphicsDevice->renderState.setBlend(graphics::BlendState::Alpha);
		graphicsDevice->renderState.setEffect(_effect);
		graphics::UniformValues::projection()->setValue(_transform);
		for (uint i = 0; i < _blocksList.size(); i++)
		{
			_blocksList[i]->applyRender(_effect);
		}		
	}

	void ZBatcher::drawMesh(int z, llge::ITexture* texture, uint lightmapId, TVertex* vertices, int verticesCount, ushort* indices, int indicesCount, byte colorScale)
	{		
		_verticesCounter += verticesCount;
		_indicesCounter += indicesCount;
		
		ZBlock* block = queryBlock(z);								
		block->_x = _x;
		block->_y = _y;
		block->_w = _w;
		block->_h = _h;
		block->addMesh(texture, lightmapId, vertices, verticesCount, indices, indicesCount, colorScale);
	}

	ZBlock* ZBatcher::queryBlock(int z)
	{
		BlocksMap::iterator it = _blocks.find(z);
		if (it == _blocks.end())
		{
			ZBlock* newBlock = _blocksPool.queryBlock();
			newBlock->reconstruct(z);
			_blocks[z] = newBlock;
			return newBlock;
		}
		return it->second;
	}
}