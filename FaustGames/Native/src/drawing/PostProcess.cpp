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

	//float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	void TonemapFilter::execute(graphics::TextureRenderTarget2d *source, graphics::TextureRenderTarget2d *target, uint tonemapId)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);		
		graphics::UniformValues::texture()->setValue(source->getHandle());
		graphics::UniformValues::lightmap()->setValue(tonemapId);
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessToneMapping());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}


	void BloomFilter::execute(graphics::TextureRenderTarget2d *source, graphics::TextureRenderTarget2d *target)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::UniformValues::texture()->setValue(source->getHandle());
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessBloomFilter());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}

	void FilterVBlur::execute(graphics::TextureRenderTarget2d *source, graphics::TextureRenderTarget2d *target)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::UniformValues::texture()->setValue(source->getHandle());
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessVBlurFilter());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}

	void FilterHBlur::execute(graphics::TextureRenderTarget2d *source, graphics::TextureRenderTarget2d *target)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::UniformValues::texture()->setValue(source->getHandle());
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessHBlurFilter());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}


	void FilterAdd::execute(graphics::TextureRenderTarget2d *value0, graphics::TextureRenderTarget2d *value1, graphics::IRenderTarget *target)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::UniformValues::texture()->setValue(value0->getHandle());
		graphics::UniformValues::lightmap()->setValue(value1->getHandle());
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessBloomAddFilter());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}


	void PostProcessBloom::beginRender(uint tonemapId)
	{
		_tonemapId = tonemapId;
		_source = graphics::GraphicsDevice::Default.PostProcessTargets.pop();
		_beginTarget = graphics::GraphicsDevice::Default.actualRenderTarget;
		graphics::GraphicsDevice::Default.setRenderTarget(_source);
	}
	
	void PostProcessBloom::finishRender()
	{
		/*
		if (_tonemapId != 0)
		{
			_tonemap.execute(_source, 0, _tonemapId);
			graphics::GraphicsDevice::Default.PostProcessTargets.push(_source);
		}
		return;
		*/
		
		if (_tonemapId != 0)
		{
			graphics::TextureRenderTarget2d * tonemaped = graphics::GraphicsDevice::Default.PostProcessTargets.pop();
			_tonemap.execute(_source, tonemaped, _tonemapId);
			graphics::GraphicsDevice::Default.PostProcessTargets.push(_source);
			_source = tonemaped;
		}
		
		graphics::TextureRenderTarget2d *blur0 = graphics::GraphicsDevice::Default.PostProcessScaledTargets.pop();
		graphics::TextureRenderTarget2d *blur1 = graphics::GraphicsDevice::Default.PostProcessScaledTargets.pop();
		_filter.execute(_source, blur0);
		graphics::UniformValues::pixelSize()->setValue(core::Vector2(
			1.0f / (float)blur0->getWidth(),
			1.0f / (float)blur0->getHeight()));

		_vBlur.execute(blur0, blur1);
		_hBlur.execute(blur1, blur0);
		_add.execute(_source, blur0, _beginTarget);

		graphics::GraphicsDevice::Default.PostProcessScaledTargets.push(blur0);
		graphics::GraphicsDevice::Default.PostProcessScaledTargets.push(blur1);
		graphics::GraphicsDevice::Default.PostProcessTargets.push(_source);

		/*
		_vBlur.execute(graphics::GraphicsDevice::Default.PostProcessRenderTargets[2], graphics::GraphicsDevice::Default.PostProcessRenderTargets[3]);
		_hBlur.execute(graphics::GraphicsDevice::Default.PostProcessRenderTargets[3], graphics::GraphicsDevice::Default.PostProcessRenderTargets[2]);
		_add.execute(graphics::GraphicsDevice::Default.PostProcessRenderTargets[1], graphics::GraphicsDevice::Default.PostProcessRenderTargets[2], 0);
		*/

	}
/*
	void PostProcessBloom::execute(graphics::TextureRenderTarget2d *source, uint tonemapId)
	{
		
		if (tonemapId != 0)
		{
			_tonemap.execute(source, graphics::GraphicsDevice::Default.PostProcessRenderTargets[1], tonemapId);
			
		}
		//_filter.execute(graphics::GraphicsDevice::Default.PostProcessRenderTargets[1], 0, tonemapId);		
		_filter.execute(graphics::GraphicsDevice::Default.PostProcessRenderTargets[1], graphics::GraphicsDevice::Default.PostProcessRenderTargets[2], tonemapId);
		graphics::UniformValues::pixelSize()->setValue(core::Vector2(
			1.0f / (float)graphics::GraphicsDevice::Default.PostProcessRenderTargets[2]->getWidth(),
			1.0f / (float)graphics::GraphicsDevice::Default.PostProcessRenderTargets[2]->getHeight()));
		_vBlur.execute(graphics::GraphicsDevice::Default.PostProcessRenderTargets[2], graphics::GraphicsDevice::Default.PostProcessRenderTargets[3]);
		_hBlur.execute(graphics::GraphicsDevice::Default.PostProcessRenderTargets[3], graphics::GraphicsDevice::Default.PostProcessRenderTargets[2]);
		_add.execute(graphics::GraphicsDevice::Default.PostProcessRenderTargets[1], graphics::GraphicsDevice::Default.PostProcessRenderTargets[2], 0);
		
	}*/
}