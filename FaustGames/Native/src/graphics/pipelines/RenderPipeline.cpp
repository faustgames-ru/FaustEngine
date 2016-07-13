#include "RenderPipeline.h"

namespace graphics
{
	RenderPipelineBuffer::RenderPipelineBuffer(): currentPosition(0)
	{
	}

	void RenderPipelineBuffer::reset()
	{
		currentPosition = 0;
	}

	RenderPipelineBuffers::RenderPipelineBuffers(): _currentPosition(0)
	{
		_buffers.push_back(new RenderPipelineBuffer());
	}

	RenderPipelineBufferPushResult RenderPipelineBuffers::push(byte* data, int size)
	{
		RenderPipelineBuffer* buffer = _buffers[_currentPosition];
		if (buffer->currentPosition + size > RenderPipelineBuffer::VerticesBuffesSize)
		{
			_currentPosition++;
			if (_currentPosition >= _buffers.size())
			{
				_buffers.push_back(new RenderPipelineBuffer());
				buffer = _buffers[_currentPosition];
			}
			else
			{
				buffer = _buffers[_currentPosition];
				buffer->reset();
			}
		}
		RenderPipelineBufferPushResult res;
		res.originPtr = buffer->data;
		res.ptr = buffer->data + buffer->currentPosition;
		memmove(res.ptr, data, size);
		buffer->currentPosition += size;
		return res;
	}

	void RenderPipelineBuffers::reset()
	{
		_currentPosition = 0;
		RenderPipelineBuffer* buffer = _buffers[_currentPosition];
		buffer->reset();
	}

	RenderPipeline::RenderPipeline(): _verticesCount(0)
	{
	}
/*
	void RenderPipeline::draw(const RenderPipelineAbstractInput& data)
	{
		
		RenderPipelineBufferPushResult verticesPtr = _vertices.push(static_cast<byte *>(data.verticesData), data.verticesSize);
		int addVertives = data.verticesSize / data.format->getStride();

		if (_blocks.size() == 0)
		{
			newBlock(data.format, data.effect, verticesPtr.ptr, data.indicesCount);
		}
		else
		{
			RenderPipelineBlock& block = _blocks.back();
			if (verticesPtr.originPtr != block.verticesData)
			{
				newBlock(data.format, data.effect, verticesPtr.ptr, data.indicesCount);
				_verticesCount = 0;
			}
			else if (block.effect != data.effect || block.format != data.format || _verticesCount + addVertives >= 65536)
			{
				newBlock(data.format, data.effect, verticesPtr.ptr, data.indicesCount);
			}
			else if (!true
			//uniformsEquals()
			)
			{				
				newBlock(data.format, data.effect, verticesPtr.ptr, data.indicesCount);
			}
			else
			{
				block.indicesCount += data.indicesCount;
			}
		}
		for (int i = 0; i < data.indicesCount; i++)
		{
			_indices.push_back(_verticesCount + data.indicesData[i]);
		}
		_verticesCount += addVertives;
		
	}
*/
	void RenderPipeline::load()
	{
	}

	void RenderPipeline::apply()
	{
		for (uint i = 0; i < _blocks.size(); i++)
		{
			RenderPipelineBlock& block = _blocks[i];
			GraphicsDevice::Default.renderState.setEffect(block.effect);
			block.uniformsConfig->apply();
			GraphicsDevice::Default.drawPrimitives(
				block.format,
				block.verticesData,
				_indices.data() + block.indicesStart,
				block.indicesCount / 3);
		}
		startPipeline();
	}

	void RenderPipeline::startPipeline()
	{
		_blocks.clear();
		_indices.clear();
		_vertices.reset();
		_uniforms.reset();
		_verticesCount = 0;
	}
	/*
	RenderPipelineBlock& RenderPipeline::newBlock(VertexFormat* format, EffectBase* effect, void* verticesData, int indicesSize)
	{
		RenderPipelineBlock block;
		block.format = format;
		block.effect = effect;
		block.verticesData = verticesData;
		block.indicesStart = _indices.size();
		block.indicesCount = indicesSize;
		_blocks.push_back(block);
		return _blocks.back();
	}
	*/
}
