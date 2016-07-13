#ifndef GRPAHICS_PIPELINES_RENDER_PIPELINE_H
#define GRPAHICS_PIPELINES_RENDER_PIPELINE_H

#include "../graphics_classes.h"
#include "RenderVertex.h"
#include "UniformsConfig.h"

namespace graphics
{
	class RenderPipelineBuffer
	{
	public:
		static const int VerticesBuffesSize = 655360;
		RenderPipelineBuffer();
		byte data[VerticesBuffesSize];
		uint currentPosition;
		void reset();
	};

	struct RenderPipelineBufferPushResult
	{
		void* originPtr;
		void* ptr;
	};

	class RenderPipelineBuffers
	{
	public:
		RenderPipelineBuffers();
		RenderPipelineBufferPushResult push(byte *data, int size);
		void reset();
	private:
		std::vector<RenderPipelineBuffer*> _buffers;
		uint _currentPosition;
	};

	struct RenderPipelineBlock
	{
		VertexFormat* format;
		EffectBase* effect;
		void* verticesData;
		int indicesStart;
		int indicesCount;
		IUniformConfig* uniformsConfig;
		byte* uniformsData;
		int uniformsSize;
		template <typename TUniformConfig>
		bool uniformsEquals(const TUniformConfig& uniforms);
	};

	template<typename T, typename TUniformConfig>
	struct RenderPipelineInput
	{
		EffectBase* effect;
		T* verticesData;
		int verticesCount;
		ushort* indicesData;
		int indicesCount;
		TUniformConfig uniforms;
	};

	class RenderPipeline
	{
	public:
		RenderPipeline();
		template<typename T, typename TUniformConfig>
		void draw(const RenderPipelineInput<T, TUniformConfig>& data);
		//void draw(const RenderPipelineAbstractInput& data);
		void load();
		void apply();
	private:
		void startPipeline();
		template <typename TUniformConfig>
		RenderPipelineBlock& newBlock(VertexFormat* format, EffectBase* effect, void* verticesData, int indicesSize, TUniformConfig config);
		std::vector<RenderPipelineBlock> _blocks;
		RenderPipelineBuffers _vertices;
		RenderPipelineBuffers _uniforms;
		std::vector<ushort> _indices;
		int _verticesCount;
	};


	template <typename TUniformConfig>
	bool RenderPipelineBlock::uniformsEquals(const TUniformConfig& uniforms)
	{
		if (sizeof(TUniformConfig) != uniformsSize)
			return false;
		return memcmp(&uniforms, uniformsData, uniformsSize) == 0;
	}

	template <typename T, typename TUniformConfig>
	void RenderPipeline::draw(const RenderPipelineInput<T, TUniformConfig>& data)
	{
		VertexFormat *format = T::format();
		int verticesSize = data.verticesCount * sizeof(T);
		RenderPipelineBufferPushResult verticesPtr = _vertices.push(static_cast<byte *>(static_cast<void*>(data.verticesData)), verticesSize);
		int addVertives = data.verticesCount;

		if (_blocks.size() == 0)
		{
			newBlock(format, data.effect, verticesPtr.ptr, data.indicesCount, data.uniforms);
			_verticesCount = 0;
		}
		else
		{
			RenderPipelineBlock& block = _blocks.back();
			if (verticesPtr.originPtr != block.verticesData)
			{
				newBlock(format, data.effect, verticesPtr.ptr, data.indicesCount, data.uniforms);
				_verticesCount = 0;
			}
			else if (block.effect != data.effect || block.format != format || _verticesCount + addVertives >= 65536 || !block.uniformsEquals(data.uniforms))
			{
				newBlock(format, data.effect, verticesPtr.ptr, data.indicesCount, data.uniforms);
				_verticesCount = 0;
			}
			else if (!true/*uniformsEquals()*/)
			{
				newBlock(format, data.effect, verticesPtr.ptr, data.indicesCount, data.uniforms);
				_verticesCount = 0;
			}
			else
			{
				block.indicesCount += data.indicesCount;
			}
		}
		for (uint i = 0; i < data.indicesCount; i++)
		{
			_indices.push_back(_verticesCount + data.indicesData[i]);
		}
		_verticesCount += addVertives;
		/*
		RenderPipelineAbstractInput abstractData;
		abstractData.effect = data.effect;
		abstractData.format = T::format();
		abstractData.verticesData = data.verticesData;
		abstractData.verticesSize = data.verticesCount * sizeof(T);
		abstractData.indicesData = data.indicesData;
		abstractData.indicesCount = data.indicesCount;
		draw(abstractData);
		*/
	}

	template <typename TUniformConfig>
	RenderPipelineBlock& RenderPipeline::newBlock(VertexFormat* format, EffectBase* effect, void* verticesData, int indicesSize, TUniformConfig config)
	{
		RenderPipelineBufferPushResult u = _uniforms.push(static_cast<byte *>(static_cast<void *>(&config)), sizeof(TUniformConfig));
		TUniformConfig* configPtr = static_cast<TUniformConfig*>(u.ptr);
		RenderPipelineBlock block;
		block.format = format;
		block.effect = effect;
		block.verticesData = verticesData;
		block.indicesStart = _indices.size();
		block.indicesCount = indicesSize;
		block.uniformsConfig = configPtr;
		block.uniformsData = static_cast<byte*>(u.ptr);
		block.uniformsSize = sizeof(TUniformConfig);
		_blocks.push_back(block);
		return _blocks.back();
	}
}

#endif