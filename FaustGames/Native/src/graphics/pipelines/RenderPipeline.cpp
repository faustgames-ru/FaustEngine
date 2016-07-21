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
}
