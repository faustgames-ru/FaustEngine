#include "PostProcess.h"

namespace drawing
{
	PostProcessVertex _quadVertices[4] =
	{
		PostProcessVertex(-1.0f, -1.0f, 0.5f, 0.0f, 0.0f),
		PostProcessVertex(-1.0f, +1.0f, 0.5f, 0.0f, 1.0f),
		PostProcessVertex(+1.0f, +1.0f, 0.5f, 1.0f, 1.0f),
		PostProcessVertex(+1.0f, -1.0f, 0.5f, 1.0f, 0.0f),
	};

	ushort _quadIndices[6] =
	{
		0, 1, 2,
		0, 2, 3
	};

	void BloomFilter::execute(graphics::TextureRenderTarget2d *source, graphics::TextureRenderTarget2d *target)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::UniformValues::texture()->setValue(source->getHandle());
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::Alpha);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessBloomFilter());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}

	void FilterVBlur::execute(graphics::TextureRenderTarget2d *source, graphics::TextureRenderTarget2d *target)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::UniformValues::texture()->setValue(source->getHandle());
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::Alpha);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessVBlurFilter());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}

	void FilterHBlur::execute(graphics::TextureRenderTarget2d *source, graphics::TextureRenderTarget2d *target)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::UniformValues::texture()->setValue(source->getHandle());
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::Alpha);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessHBlurFilter());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}


	void FilterAdd::execute(graphics::TextureRenderTarget2d *value0, graphics::TextureRenderTarget2d *value1, graphics::TextureRenderTarget2d *target)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::UniformValues::texture()->setValue(value0->getHandle());
		graphics::UniformValues::lightmap()->setValue(value1->getHandle());
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::Alpha);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessBloomAddFilter());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}


	void PostProcessBloom::execute(graphics::TextureRenderTarget2d *source)
	{
		_filter.execute(source, graphics::GraphicsDevice::Default.PostProcessRenderTargets[1]);
		graphics::UniformValues::pixelSize()->setValue(core::Vector2(
			1.0f / (float)graphics::GraphicsDevice::Default.PostProcessRenderTargets[1]->getWidth(),
			1.0f / (float)graphics::GraphicsDevice::Default.PostProcessRenderTargets[1]->getHeight()));
		_vBlur.execute(graphics::GraphicsDevice::Default.PostProcessRenderTargets[1], graphics::GraphicsDevice::Default.PostProcessRenderTargets[2]);
		_hBlur.execute(graphics::GraphicsDevice::Default.PostProcessRenderTargets[2], graphics::GraphicsDevice::Default.PostProcessRenderTargets[1]);
		_add.execute(source, graphics::GraphicsDevice::Default.PostProcessRenderTargets[1], 0);
	}
}