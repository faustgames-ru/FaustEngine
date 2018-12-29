#include "Batcher.h"

namespace drawing
{
	BatchBuffer::BatchBuffer()
	{
		_verticesCount = 0;
		_indicesCount = 0;
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

	void BatchBuffer::addMesh(uint color, float z, float* vertices, float* uvs, int verticesCount, ushort* indices, int indicesCount, graphics::BlendState::e blend, byte colorScale)
	{
		for (int i = 0; i < indicesCount; i++)
		{
			_indices[_indicesCount + i] = static_cast<unsigned short>(_verticesCount + indices[i]);
		}

		TVertex* target = _vertices + _verticesCount;
		float* source = vertices;
		float* uvsource = uvs;
		//uint targetColor = graphics::Color::premul(color, colorScale, additive);
		for (int i = 0; i < verticesCount; i++, target++)
		{
			target->x = *source; ++source;
			target->y = *source; ++source;
			target->z = z;
			target->color = lightMap->applyLight(target->x, target->y, color, colorScale, blend);
			float u = *uvsource; ++uvsource;
			float v = *uvsource; ++uvsource;
			target->u = textureTransform.transformU(u, v);
			target->v = textureTransform.transformV(u, v);
		}

		_verticesCount += verticesCount;
		_indicesCount += indicesCount;
	}
	
	void BatchBuffer::addMesh(uint color, float z, float* vertices, float* uvs, int verticesCount, ushort* indices, int indicesCount, graphics::BlendState::e blend, core::Matrix viewTransform, byte colorScale)
	{
		for (int i = 0; i < indicesCount; i++)
		{
			_indices[_indicesCount + i] = static_cast<unsigned short>(_verticesCount + indices[i]);
		}

		TVertex* target = _vertices + _verticesCount;
		float* source = vertices;
		float* uvsource = uvs;
		core::Vector3 uv;
		//uint targetColor = graphics::Color::premul(color, colorScale, additive);
		for (int i = 0; i < verticesCount; i++, target++)
		{
			target->x = *source; ++source;
			target->y = *source; ++source;
			target->z = z;
			target->color = lightMap->applyLight(target->x, target->y, color, colorScale, blend);
			float u = *uvsource; ++uvsource;
			float v = *uvsource; ++uvsource;
			target->u = textureTransform.transformU(u, v);
			target->v = textureTransform.transformV(u, v);

			//uv = core::Matrix::transform(viewTransform, core::Vector3(target->x, target->y, target->z));
			//uv = (uv + core::Vector3(1.0f, 1.0f, 0.0f))*0.5f;
			//target->u = uv.getX();
			//target->v = uv.getY();
		}

		_verticesCount += verticesCount;
		_indicesCount += indicesCount;
	}


	void BatchBuffer::addMesh(TVertex* vertices, int verticesCount, ushort* indices, int indicesCount, graphics::BlendState::e blend, unsigned char colorScale)
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
			target->u = textureTransform.transformU(source->u, source->v);
			target->v = textureTransform.transformV(source->u, source->v);
			target->color = lightMap->applyLight(target->x, target->y, source->color, colorScale, blend);
			//target->color = graphics::Color::premul(source->color, colorScale, additive);
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

	EntriesGroup::EntriesGroup(int index)
	{
		transformIndex = index;
		Entries.reserve(128);
	}

	RenderBuffer::RenderBuffer()
	{
		for (int i = 0; i < 3; i++)
		{
			EntriesGroups.push_back(new EntriesGroup(i));
		}
		Transforms.reserve(128);
		Buffers.push_back(new BatchBuffer());
	}

	RenderBuffer::~RenderBuffer()
	{
		for (TBatchEntriesGroup::iterator i = EntriesGroups.begin(); i != EntriesGroups.end(); ++i)
		{
			EntriesGroup *group = *i;
			delete group;
		}
		for (TBatchBuffers::iterator i = Buffers.begin(); i != Buffers.end(); ++i)
		{
			BatchBuffer *buffer = *i;
			delete buffer;
		}
	}

	void RenderBuffer::addEntry(const BatchEntry& entry)
	{
		int transformIndex = entry.TransformIndex;
		while (transformIndex >= EntriesGroups.size())
		{
			EntriesGroups.push_back(new EntriesGroup(EntriesGroups.size()));
		}
		EntriesGroups[transformIndex]->Entries.push_back(entry);
	}

	void RenderBuffer::clearEntries()
	{
		for (int i = 0; i < EntriesGroups.size(); i++)
		{
			EntriesGroups[i]->Entries.clear();
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

	BatcherRenderArgs::BatcherRenderArgs() : usePostProcess(false), renderBuffer(nullptr)
	{
	}

	StencilVerticesBuffer::StencilVerticesBuffer()
	{
		_verticesCount = 0;
		_indicesCount = 0;
		_vertices = static_cast<core::Vector3 *>(malloc(VerticesLimit * sizeof(core::Vector3)));
		_indices = static_cast<ushort *>(malloc(IndicesLimit * sizeof(unsigned short)));
	}

	StencilVerticesBuffer::~StencilVerticesBuffer()
	{
		free(_vertices);
		free(_indices);
	}

	void StencilVerticesBuffer::reset()
	{
		_indicesCount = 0;
		_verticesCount = 0;
	}

	bool StencilVerticesBuffer::canAdd(int verticesCount, int indicesCount) const
	{
		return _indicesCount + indicesCount < IndicesLimit && _verticesCount + verticesCount < VerticesLimit;
	}

	void StencilVerticesBuffer::add(core::Vector3* vertices, int verticesCount, ushort* indices, int indicesCount)
	{
		for (int i = 0; i < indicesCount; i++)
		{
			_indices[_indicesCount + i] = static_cast<unsigned short>(_verticesCount + indices[i]);
		}

		core::Vector3* target = _vertices + _verticesCount;
		core::Vector3* source = vertices;
		for (int i = 0; i < verticesCount; i++)
		{
			target[i] = source[i]; 
		}

		_verticesCount += verticesCount;
		_indicesCount += indicesCount;
	}

	core::Vector3* StencilVerticesBuffer::getVertices()
	{
		return _vertices;
	}

	int StencilVerticesBuffer::getVerticesCount() const
	{
		return _verticesCount;
	}

	ushort* StencilVerticesBuffer::getCurrentIndices() const
	{
		return  _indices + _indicesCount;
	}

	int StencilVerticesBuffer::getIndicesCount()
	{
		return _indicesCount;
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
		_lightingMode = llge::BLMDynamicCpu;
		_lightingModes[llge::BLMNone] = &_lightMapEmpty;
		_lightingModes[llge::BLMDynamicCpu] = &_lightMap;
		core::DebugDraw::Default.Render = this;
		_buffer = new RenderBuffer();
		//_localBuffer = static_cast<TVertex *>(malloc(graphics::GraphicsConstants::LocalBufferSize * sizeof(TVertex)));
		_graphicsDevice = &graphics::GraphicsDevice::Default;
		_textureTransform = graphics::TextureTransform();
		_zButcher = new ZBatcher();
		_emptyColorTransformContainer = core::Matrix3Container(core::Matrix3::identity);
		_controller = new BatcherRenderController(this);
		_controller->setState(_controller->Default);
	}
	
	Batcher::~Batcher()
	{
		core::DebugDraw::Default.Render = &core::DebugRenderDummy::Default;
		delete _buffer;
		delete _zButcher;
		delete _controller;
		//free(_localBuffer);
	}
	
	void Batcher::start()
	{
		_mask = 0;
		_verticesCounter = 0;
		_buffer->clearEntries();
		_buffer->Transforms.clear();
		_buffer->ColorTransforms.clear();
		_batchBufferIndex = 0;
		for (uint i = 0; i < CONFIG_MAX_SIZE; i++)
			_config[i] = 0;
		_buffer->Buffers[_batchBufferIndex]->reset();
		_currentEntry.IndicesCount = 0;
		_blend = graphics::BlendState::None;
		_effect = nullptr;
		_zButcher->reset(&_lightMap);
		_stecilMaskGeometry.reset();
		_stecilMaskEntries.clear();
	}
	
	void Batcher::finish()
	{
		applyEntry();
		_mask = 0;
	}

	void Batcher::drawMaskMesh(byte mask, core::Vector3* vertices, int verticesCount, ushort* indices, int indicesCount)
	{
		if (!_stecilMaskGeometry.canAdd(verticesCount, indicesCount))
		{
			// todo: log stencil geometru overflow
			return;
		}

		StencilEntry entry;
		entry.mask = mask;
		entry.indices = _stecilMaskGeometry.getCurrentIndices();
		entry.indicesCount = indicesCount;
		entry.transformIndex = _buffer->Transforms.size() - 1;
		_stecilMaskEntries.push_back(entry);
		_stecilMaskGeometry.add(vertices, verticesCount, indices, indicesCount);
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

		if (effect != _effect || needNewEntry || !effect->isConfigEqual(_config, &_lightingConfig) || blend == graphics::BlendState::Multiplicative)
		{
			if (_currentEntry.IndicesCount != 0)
				_buffer->addEntry(_currentEntry);
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
			_currentEntry.StencilMask = _mask;
			_currentEntry.ColorTransformIndex = _buffer->ColorTransforms.size() - 1;
			_currentEntry.RenderTargetIndex = _buffer->RenderTargets.size() - 1;
		}
		
		//_textureId = textureId;
		_currentEntry.IndicesCount += indicesCount;
		currentBuffer->textureTransform = _textureTransform;
		currentBuffer->lightMap = _lightingModes[_lightingMode];
		currentBuffer->addMesh(vertices, verticesCount, indices, indicesCount, blend, colorScale);
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
				_buffer->addEntry(_currentEntry);
			_currentEntry.IndicesStart = currentBuffer->getCurrentIndices();
			_currentEntry.IndicesCount = 0;
			_currentEntry.BatchBufferIndex = _batchBufferIndex;
			effect->configCopy(_currentEntry.Config, config);
			effect->configCopy(_config, config);
			_currentEntry.Blend = _blend = blend;
			_currentEntry.Effect = _effect = effect;
			_currentEntry.TransformIndex = _buffer->Transforms.size() - 1;
			_currentEntry.StencilMask = _mask;
			_currentEntry.ColorTransformIndex = _buffer->ColorTransforms.size() - 1;
			_currentEntry.RenderTargetIndex = _buffer->RenderTargets.size() - 1;
		}

		_currentEntry.IndicesCount += indicesCount;
		currentBuffer->textureTransform = _textureTransform;
		currentBuffer->lightMap = _lightingModes[_lightingMode];
		currentBuffer->addMesh(vertices, verticesCount, indices, indicesCount, blend, colorScale);
	}

	void Batcher::drawSpineMesh(const BatcherSpineMesh &mesh, byte colorScale)
	{		
		if (mesh.texture != nullptr)
		{
			setupUVTransform(mesh.texture);
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
				_buffer->addEntry(_currentEntry);
			_currentEntry.IndicesStart = currentBuffer->getCurrentIndices();
			_currentEntry.IndicesCount = 0;
			_currentEntry.BatchBufferIndex = _batchBufferIndex;
			mesh.State.Effect->configCopy(_currentEntry.Config, mesh.State.config);
			mesh.State.Effect->configCopy(_config, mesh.State.config);
			
			_currentEntry.Blend = _blend = mesh.State.Blend;
			_currentEntry.Effect = _effect = mesh.State.Effect;
			_currentEntry.TransformIndex = _buffer->Transforms.size() - 1;
			_currentEntry.StencilMask = _mask;
			_currentEntry.ColorTransformIndex = _buffer->ColorTransforms.size() - 1;
			_currentEntry.RenderTargetIndex = _buffer->RenderTargets.size() - 1;
		}

		_currentEntry.IndicesCount += mesh.IndicesCount;
		currentBuffer->textureTransform = _textureTransform;
		currentBuffer->lightMap = _lightingModes[_lightingMode];
		currentBuffer->addMesh(mesh.Color, mesh.Z, mesh.Vertices, mesh.Uvs, mesh.VerticesCount, mesh.Indices, mesh.IndicesCount, mesh.State.Blend, colorScale);
	}

	void Batcher::setupUVTransform(llge::ITexture* texture)
	{
		graphics::Texture * textureInstance = static_cast<graphics::Texture *>(texture->getTextureInstance());
		_textureTransform = textureInstance->transform;
	}

	void Batcher::cleanupUVTransform()
	{
		_textureTransform = graphics::TextureTransform();
	}

	void Batcher::executeRenderCommands(bool usePostProcess)
	{
		BatcherRenderArgs e;
		e.renderBuffer = _buffer;
		e.usePostProcess = usePostProcess;
		internalExecuteRenderCommands(e);
	}

	void Batcher::internalExecuteRenderCommands(BatcherRenderArgs e)
	{
		_usedPostProcess = e.usePostProcess;
		_primitivesCount = 0;
		_verticesCount = 0;

		_controller->update(e);
		
		_verticesCount = _verticesCounter; 
		_verticesCounter = 0;
		
		if (_buffer->Transforms.size() > 0)
		{
			_debugRender.apply(_graphicsDevice, _buffer->Transforms[0]);
		}

		_graphicsDevice->renderState.setDepth(graphics::DepthState::None);
	}

	void Batcher::addColorTransform(const core::Matrix3& value)
	{
		_colorTransform.setValue(value);
		_buffer->ColorTransforms.push_back(_colorTransform);
		applyEntry();
	}

	void Batcher::renderStencil(BatcherRenderArgs e)
	{
		if (_stecilMaskEntries.empty())
		{
			return;
		}
		_graphicsDevice->clearStencil();		
		for(int i = 0; i < _stecilMaskEntries.size(); i++)
		{
			StencilEntry entry = _stecilMaskEntries[i];
			_graphicsDevice->startStencilMask(entry.mask);
			graphics::UniformValues::projection()->setValue(_buffer->Transforms[entry.transformIndex]);
			_graphicsDevice->renderState.setEffect(graphics::Effects::solid());
			_graphicsDevice->drawPrimitives(graphics::VertexFormats::position(), 
				_stecilMaskGeometry.getVertices(), 
				entry.indices, 
				entry.indicesCount / 3);
		}
		_graphicsDevice->finishStencil();
	}

	void Batcher::mainRender(BatcherRenderArgs e)
	{
		renderStencil(e);

		if (_usedPostProcess && _bloom.isAvaliable())
		{
			_bloom.beginRender(_tonemapId);
		}
		else
		{
			_graphicsDevice->clear();
		}

		if (_buffer->Transforms.size() > 0)
		{
			_zButcher->configure(graphics::BlendState::Alpha, _converter.getEffect(llge::EffectTextureColor), _buffer->Transforms[0]);
			_zButcher->applyRender();
		}
		renderTranformGroup(0, e);

		_graphicsDevice->renderState.setDepth(graphics::DepthState::None);
		if (_usedPostProcess && _bloom.isAvaliable())
		{
			_graphicsDevice->clearDepth();
			_bloom.finishRender();
		}
	}

	void Batcher::guiRender(BatcherRenderArgs e)
	{
		_graphicsDevice->clearDepth();
		for (int i = 1; i < _buffer->EntriesGroups.size(); i++)
		{
			renderTranformGroup(i, e);
		}
	}

	void Batcher::renderTranformGroup(int groupIndex, BatcherRenderArgs e)
	{
		if (groupIndex >= e.renderBuffer->EntriesGroups.size()) return;
		EntriesGroup* group = e.renderBuffer->EntriesGroups[groupIndex];
		int primitivesCount = 0;
		int verticesCount = 0;

		RenderBuffer * _backBuffer = e.renderBuffer;
		graphics::EffectBase *effect = nullptr;
		for (TBatchEntries::iterator i = group->Entries.begin(); i != group->Entries.end(); ++i)
		{	

			BatchBuffer * currentBuffer = _backBuffer->Buffers[i->BatchBufferIndex];
			effect = i->Effect;
			if (i->Blend == graphics::BlendState::Multiplicative)
			{
				effect = graphics::Effects::textureColor();
			}
			else if (groupIndex != 0)
			{
				effect = graphics::Effects::unFog(effect);
			}
			effect->configApply(i->Config);

			graphics::UniformValues::projection()->setValue(_backBuffer->Transforms[i->TransformIndex]);
			if (i->ColorTransformIndex >= 0)
			{
				graphics::UniformValues::colorTransform()->setValue(_backBuffer->ColorTransforms[i->ColorTransformIndex]);

			}
			_graphicsDevice->renderState.setEffect(effect);

			primitivesCount = i->IndicesCount / 3;
			verticesCount = currentBuffer->getVerticesCount();

			_primitivesCount += primitivesCount;
			_verticesCount += verticesCount;
			if (i->StencilMask > 0)
			{
				_graphicsDevice->startStencilTest(i->StencilMask);
			}

			if (i->Blend == graphics::BlendState::Multiplicative)
			{
				_graphicsDevice->renderState.setBlend(graphics::BlendState::Multiplicative);
				// disable fog
			}
			else
			{
				_graphicsDevice->renderState.setBlend(graphics::BlendState::Alpha);
			}
			_graphicsDevice->renderState.setDepth(graphics::DepthState::Read);
			_graphicsDevice->renderState.setDepthfunc(graphics::DepthFunc::LessEqual);
			_graphicsDevice->drawPrimitives(_format, currentBuffer->getVertices(), i->IndicesStart, primitivesCount);
			if (i->StencilMask > 0)
			{
				_graphicsDevice->finishStencil();
			}
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
			_buffer->addEntry(_currentEntry);
			//if (_batchBufferIndex < _buffer->Buffers.size())
			//{
			//	_currentEntry.IndicesStart = _buffer->Buffers[_batchBufferIndex]->getCurrentIndices();
			//	_currentEntry.BatchBufferIndex = _batchBufferIndex;
			//}
			_currentEntry.IndicesCount = 0;
			_effect = nullptr;
		}
	}

	void Batcher::setLightingMode(llge::BatcherLightingMode mode)
	{
		_lightingMode = mode;
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

	void Batcher::setupLighting(IntPtr lightingConfig)
	{
		if (lightingConfig == nullptr)
		{
			_lightMap.clear();
			_lightMap.build(0, 0, 1, 1, 0xffffffff);
		}
		else
		{
			llge::Lighting2dConfig* config = static_cast<llge::Lighting2dConfig*>(lightingConfig);
			_lightMap.clear();
			llge::Light2d* lights = static_cast<llge::Light2d*>(config->LightsPtr);
			for (int i = 0; i < config->LightsCount; i++)
			{
				llge::Light2d* light = lights + i;
				_lightMap.addLight(light->x, light->y, light->r, light->color, light->i);
			}
			_lightMap.build(config->x, config->y, config->w, config->h, config->ambient);
		}
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
		llge::GraphicsEffects effect = static_cast<llge::GraphicsEffects>(bc->effect);
		bool useAlpha = false;
		if (effect == llge::GraphicsEffects::EffectTextureColor)
		{
			llge::EffectConfig* config = static_cast<llge::EffectConfig*>(texturesConfig);
			useAlpha = config->alpha != 0;
		}
		drawMesh(
			_converter.getEffect(static_cast<llge::GraphicsEffects>(effect), useAlpha),
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

			_zButcher->textureTransform = textureInstance->transform;
			_zButcher->drawMesh(z, texture, lightmapId, static_cast<TVertex *>(vertices), verticesCount, static_cast<ushort *>(indices), indicesCount, colorScale);
		}
		else
		{
			setupUVTransform(texture);
			drawMesh(_converter.getEffect(llge::GraphicsEffects::EffectTextureColor), graphics::BlendState::Alpha, texture, lightmapId, static_cast<TVertex *>(vertices), verticesCount, static_cast<ushort *>(indices), indicesCount, colorScale);
			cleanupUVTransform();
		}
	}

	void Batcher::drawMask(byte mask, IntPtr vertices, int verticesCount, IntPtr indices, int indicesCount)
	{
		drawMaskMesh(mask, static_cast<core::Vector3*>(vertices), verticesCount, static_cast<ushort*>(indices), indicesCount);
	}

	void Batcher::setMask(byte mask)
	{
		if (_mask == mask) return;
		_mask = mask;
		applyEntry();
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

	void Batcher::setBatcherMode(llge::BatcherMode mode)
	{
		_controller->Mode = mode;
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

	void ZBlock::reconstruct(int zLevel, ILightMap* lightMap)
	{
		z = zLevel;
		Buffer.setupLightMap(lightMap);
		Buffer.reset();
		Entries.clear();
	}

	void ZBlock::addMesh(llge::ITexture* texture, uint lightmapId, TVertex* vertices, int verticesCount, ushort* indices, int indicesCount, byte colorScale)
	{
		ZBatchEntry e;
		Buffer.textureTransform = textureTransform;
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
		_lightMap = nullptr;
	}

	ZBatchBuffer::~ZBatchBuffer()
	{
		for (uint i = 0; i < _vertices.size(); i++)
		{
			delete[] _vertices[i];
		}
	}

	void ZBatchBuffer::setupLightMap(ILightMap* lightMap)
	{
		_lightMap = lightMap;
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
			target->color = graphics::Color::premul(source->color, colorScale, graphics::BlendState::Alpha);
			//target->color = _lightMap->applyLight(source->x, source->y, source->color, colorScale, false);
			target->u = textureTransform.transformU(source->u, source->v);
			target->v = textureTransform.transformV(source->u, source->v);
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

	void ZBatcher::reset(ILightMap* lightmap)
	{
		_lightMap = lightmap;
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
		graphicsDevice->renderState.setDepthfunc(graphics::DepthFunc::Less);
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
		block->textureTransform = textureTransform;
		block->addMesh(texture, lightmapId, vertices, verticesCount, indices, indicesCount, colorScale);
	}

	ZBlock* ZBatcher::queryBlock(int z)
	{
		BlocksMap::iterator it = _blocks.find(z);
		if (it == _blocks.end())
		{
			ZBlock* newBlock = _blocksPool.queryBlock();
			newBlock->reconstruct(z, _lightMap);
			_blocks[z] = newBlock;
			return newBlock;
		}
		return it->second;
	}

	BatcherRenderArgs* BatcherRenderController::args()
	{
		return &_args;
	}

	Batcher* BatcherRenderController::batcher()
	{
		return _batcher;
	}

	void BatcherRenderController::setState(BatcherRenderState* state)
	{
		if (_state != nullptr)
		{
			_state->deactivated();
		}
		_state = state;
		if (_state != nullptr)
		{
			_state->activated();
		}
	}

	void BatcherRenderController::update(BatcherRenderArgs e)
	{
		_args = e;
		if (_state != nullptr)
		{
			_state->update();
		}
	}

	void BatcherRenderController::drawSnapShot(graphics::IPostProcessTarget *snapShot)
	{
		graphics::UniformValues::texture()->setValue(snapShot->getColor()->getHandle());
		graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessEmpty());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), quadVertices(), quadIndices(), 2);
	}

	Mesh2dVertex _quadColorVertices[4] =
	{
		Mesh2dVertex(-1.0f, -1.0f, 0.5f, 0x0, 0.0f, 0.0f),
		Mesh2dVertex(-1.0f, +1.0f, 0.5f, 0x0, 0.0f, 1.0f),
		Mesh2dVertex(+1.0f, +1.0f, 0.5f, 0x0, 1.0f, 1.0f),
		Mesh2dVertex(+1.0f, -1.0f, 0.5f, 0x0, 1.0f, 0.0f),
	};

	void BatcherRenderController::drawSnapShot(graphics::IPostProcessTarget* snapShot, int color)
	{
		
		for (int i = 0; i < 4; i++)
		{
			_quadColorVertices[i].color = color;
		}
		
		identity.setValue(core::Matrix::identity);
		graphics::UniformValues::texture()->setValue(snapShot->getColor()->getHandle());
		graphics::UniformValues::projection()->setValue(identity);
		graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::Alpha);
		
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::textureColor());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTextureColor(), _quadColorVertices, quadIndices(), 2);
	}

	BatcherRenderController::BatcherRenderController(Batcher* batcher): _state(nullptr), _snapShot(nullptr), _blurShot(nullptr), lighting(0.6f), Mode(llge::BatcherModeDefault)
	{
		_batcher = batcher;
		Default = CreateState<BatcherStateDefault>();
		BlurSnapShot = CreateState<BatcherStateBlurSnapShot>();
		Blur = CreateState<BatcherStateBlur>();
		Hide = CreateState<BatcherStateBlurHide>();
	}


	graphics::IPostProcessTarget* BatcherRenderController::popSnapShot()
	{
		if (_snapShot == nullptr)
		{
			_snapShot = graphics::GraphicsDevice::Default.PostProcessScaledTargets1.pop();
		}
		return _snapShot;		
	}

	graphics::IPostProcessTarget* BatcherRenderController::popBlurShot()
	{
		if (_blurShot == nullptr)
		{
			_blurShot = graphics::GraphicsDevice::Default.PostProcessScaledTargets1.pop();
		}
		return _blurShot;
	}

	void BatcherRenderController::swapShots()
	{
		graphics::IPostProcessTarget* swap = _snapShot;
		_snapShot = _blurShot;
		_blurShot = swap;
	}

	void BatcherRenderController::pushSnapShot()
	{
		if (_snapShot != nullptr)
		{
			graphics::GraphicsDevice::Default.PostProcessScaledTargets1.push(_snapShot);
		}
		_snapShot = nullptr;
	}

	void BatcherRenderController::pushBlurShot()
	{
		if (_blurShot != nullptr)
		{
			graphics::GraphicsDevice::Default.PostProcessScaledTargets1.push(_blurShot);
		}
		_blurShot = nullptr;
	}


	void BatcherRenderController::constructState(BatcherRenderState* state)
	{
		state->construct(this);
	}

	BatcherRenderState::BatcherRenderState() : _controller(nullptr)
	{
	}

	BatcherRenderState::~BatcherRenderState()
	{
	}

	BatcherRenderController* BatcherRenderState::controller()
	{
		return _controller;
	}

	BatcherRenderArgs* BatcherRenderState::args()
	{
		return _controller->args();
	}

	Batcher* BatcherRenderState::batcher()
	{
		return _controller->batcher();
	}

	void BatcherRenderState::setState(BatcherRenderState* state)
	{
		_controller->setState(state);
	}

	void BatcherRenderState::update()
	{
	}

	void BatcherRenderState::activated()
	{
	}

	void BatcherRenderState::deactivated()
	{
	}

	void BatcherRenderState::construct(BatcherRenderController* controller)
	{
		_controller = controller;
	}

	void BatcherStateDefault::activated()
	{
		_frameCounter = 60 * 10;
	}

	void BatcherStateDefault::update()
	{
		batcher()->mainRender(*args());
		batcher()->guiRender(*args());
		if (controller()->Mode == llge::BatcherModeBlur)
		{
			setState(controller()->BlurSnapShot);
		}
	}

	void BatcherStateBlurSnapShot::activated()
	{
	}

	void BatcherStateBlurSnapShot::update()
	{
		graphics::IRenderTarget *beginTarget = graphics::GraphicsDevice::Default.actualRenderTarget;
		graphics::IPostProcessTarget *snapShot = controller()->popSnapShot();
		graphics::GraphicsDevice::Default.setRenderTarget(snapShot->getRenderTarget());
		graphics::GraphicsDevice::Default.clear();
		batcher()->mainRender(*args());
		graphics::GraphicsDevice::Default.setRenderTarget(beginTarget);
		// todo: draw post process
		controller()->drawSnapShot(snapShot);
		graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);
		batcher()->guiRender(*args());
		setState(controller()->Blur);
	}

	core::Vector2 BatcherStateBlur::_pixelOffset[2] = 
	{
		core::Vector2(1, 0),
		core::Vector2(0, 1),
	};

	void BatcherStateBlur::activated()
	{
		_iterationsMax = 60;
		_iterationIndex = 0;
	}

	void BatcherStateBlur::update()
	{
		graphics::IPostProcessTarget *snapShot = controller()->popSnapShot();
		if (_iterationIndex < _iterationsMax)
		{
			graphics::IPostProcessTarget *blurShot = controller()->popBlurShot();

			graphics::IRenderTarget * beginTarget = graphics::GraphicsDevice::Default.actualRenderTarget;
			graphics::GraphicsDevice::Default.setRenderTarget(blurShot->getRenderTarget());
			graphics::GraphicsDevice::Default.clear();
			// todo: make iteration

			graphics::UniformValues::texture()->setValue(snapShot->getColor()->getHandle());
			
			core::Vector2 scale = core::Vector2(
				1.0f / static_cast<float>(snapShot->getWidth()),
				1.0f / static_cast<float>(snapShot->getHeight()));

			graphics::UniformValues::pixelOffset()->setValue(_pixelOffset[_iterationIndex % 2] * scale);
			graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);
			graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
			graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessBlurPassFilter());
			graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), quadVertices(), quadIndices(), 2);
			graphics::GraphicsDevice::Default.setRenderTarget(beginTarget);

			controller()->swapShots();
		}
		float u = core::Math::saturate(static_cast<float>(_iterationIndex) / static_cast<float>(_iterationsMax));
		float ch = core::Math::lerp(0.5f, controller()->lighting, u);
		controller()->drawSnapShot(snapShot, graphics::Color::fromRgba(ch, ch, ch, 1.0f));
		
		graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);
		batcher()->renderStencil(*args());
		batcher()->guiRender(*args());
			
		if (controller()->Mode == llge::BatcherModeDefault)
		{
			setState(controller()->Hide);
		}
		_iterationIndex++;

	}

	void BatcherStateBlurHide::activated()
	{
		_iterationsMax = 30;
		_frameCounter = _iterationsMax;
	}

	void BatcherStateBlurHide::update()
	{
		batcher()->mainRender(*args());

		graphics::IPostProcessTarget *snapShot = controller()->popSnapShot();
		float blend = static_cast<float>(_frameCounter) / static_cast<float>(_iterationsMax);
		float channel = controller()->lighting * blend;
		uint color = graphics::Color::fromRgba(channel, channel, channel, blend);
		controller()->drawSnapShot(snapShot, color);

		graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);

		batcher()->guiRender(*args());
		_frameCounter--;
		if (_frameCounter < 0)
		{
			setState(controller()->Default);
		}
	}
}