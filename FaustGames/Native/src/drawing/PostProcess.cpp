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
	
	void TonemapFilter::execute(graphics::Texture* source, graphics::IRenderTarget *target, uint tonemapId)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);		
		graphics::GraphicsDevice::Default.clear();
		graphics::UniformValues::texture()->setValue(source->getHandle());
		graphics::UniformValues::lightmap()->setValue(tonemapId);
		graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessToneMapping());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}

	void ColorTransformFilter::execute(graphics::Texture* source, const core::Vector3 &offset, const core::Matrix3 &colorTransform, graphics::IRenderTarget* target)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::GraphicsDevice::Default.clear();
		graphics::UniformValues::texture()->setValue(source->getHandle());
		_colorTransform.setValue(colorTransform);
		graphics::UniformValues::colorTransform()->setValue(_colorTransform);
		graphics::UniformValues::colorOffset()->setValue(offset);
		
		graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessRgbTransform());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}

	void BloomFilter::execute(graphics::Texture* source, graphics::IRenderTarget *target)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::GraphicsDevice::Default.clear();
		graphics::UniformValues::texture()->setValue(source->getHandle());
		graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessBloomFilter());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}

	void EmptyProcess::execute(graphics::Texture* source, graphics::IRenderTarget* target)
	{		
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::GraphicsDevice::Default.clear();
		graphics::UniformValues::texture()->setValue(source->getHandle());
		graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessEmpty());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
		
	}

	void VignettingProcess::execute(graphics::Texture* source)
	{
		_vertices.clear();
		_indices.clear();
		int detail = 4;
		core::Vector2 v;
		v.setY(1);
		for (int i = 0; i < detail; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(detail);
			float x = core::Math::lerp(-1.0f, 1.0f, u);
			v.setX(x);
			addVertex(v);
		}

		v.setX(1);
		for (int i = 0; i < detail; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(detail);
			float y = core::Math::lerp(1.0f, -1.0f, u);
			v.setY(y);
			addVertex(v);
		}

		v.setY(-1);
		for (int i = 0; i < detail; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(detail);
			float x = core::Math::lerp(1.0f, -1.0f, u);
			v.setX(x);
			addVertex(v);
		}

		v.setX(-1);
		for (int i = 0; i < detail; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(detail);
			float y = core::Math::lerp(-1.0f, 1.0f, u);
			v.setY(y);
			addVertex(v);
		}

		int quads = _vertices.size()/3 - 1;

		int pi = 0;
		for (int i = 0; i < quads; i++)
		{
			pi = i*3;

			_indices.push_back(pi + 0);
			_indices.push_back(pi + 1);
			_indices.push_back(pi + 3);

			_indices.push_back(pi + 1);
			_indices.push_back(pi + 4);
			_indices.push_back(pi + 3);

			_indices.push_back(pi + 1);
			_indices.push_back(pi + 2);
			_indices.push_back(pi + 4);

			_indices.push_back(pi + 2);
			_indices.push_back(pi + 5);
			_indices.push_back(pi + 4);
		}

		_indices.push_back(_vertices.size() - 3);
		_indices.push_back(_vertices.size() - 2);
		_indices.push_back(0);

		_indices.push_back(_vertices.size() - 2);
		_indices.push_back(1);
		_indices.push_back(0);

		_indices.push_back(_vertices.size() - 2);
		_indices.push_back(_vertices.size() - 1);
		_indices.push_back(1);

		_indices.push_back(_vertices.size() - 1);
		_indices.push_back(2);
		_indices.push_back(1);

		graphics::UniformValues::texture()->setValue(source->getHandle());
		graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::Normal);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessEmptyColor());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTextureColor(), 
			_vertices.data(), _indices.data(), _indices.size() / 3);
	}

	Mesh2dVertex createVertex(core::Vector2 v, uint color)
	{
		Mesh2dVertex vp;
		vp.x = v.getX();
		vp.y = v.getY();
		vp.z = 0;
		vp.u = (vp.x + 1.0f) *0.5f;
		vp.v = (vp.y + 1.0f) *0.5f;
		vp.color = color;
		return vp;
	}

	void VignettingProcess::addVertex(core::Vector2 v)
	{
		

		float d0 = graphics::GraphicsDevice::Default.config.vignettingR0;// 0.9f;
		float d1 = graphics::GraphicsDevice::Default.config.vignettingR1;// 1.2f;
		float d2 = graphics::GraphicsDevice::Default.config.vignettingR2;// 1.5f;

		float l = v.length();
		core::Vector2 v0 = v.normalize()*d0;
		core::Vector2 v1 = l > d1? v.normalize()*d1:v;
		core::Vector2 v2 = v;

		float u1 = core::Math::clamp((v1.length() - d0) / (d1 - d0), 0.0f, 1.0f);
		float u2 = core::Math::clamp((v2.length() - d1) / (d2 - d1), 0.0f, 1.0f);
		
		uint color0 = graphics::GraphicsDevice::Default.config.vignettingColor0;
		uint color1 = graphics::Color::lerp(color0, graphics::GraphicsDevice::Default.config.vignettingColor1, u1);
		uint color2 = graphics::Color::lerp(color1, graphics::GraphicsDevice::Default.config.vignettingColor2, u2);
		_vertices.push_back(createVertex(v0, color0));
		_vertices.push_back(createVertex(v1, color1));
		_vertices.push_back(createVertex(v2, color2));
	}

	void FilterVBlur::execute(graphics::Texture* source, graphics::IRenderTarget *target)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::GraphicsDevice::Default.clear();
		graphics::UniformValues::texture()->setValue(source->getHandle());
		graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessVBlurFilter());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}

	void FilterHBlur::execute(graphics::Texture* source, graphics::IRenderTarget *target)
	{
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::GraphicsDevice::Default.clear();
		graphics::UniformValues::texture()->setValue(source->getHandle());
		graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessHBlurFilter());
		graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
	}

	void FilterAdd::execute(graphics::Texture* value0, graphics::Texture* value1, graphics::IRenderTarget *target)
	{
		if (graphics::GraphicsDevice::Default.config.refraction)
		{
			refractor.update();
		}
		graphics::GraphicsDevice::Default.setRenderTarget(target);
		graphics::GraphicsDevice::Default.clear();
		graphics::UniformValues::texture()->setValue(value0->getHandle());
		graphics::UniformValues::lightmap()->setValue(value1->getHandle());
		graphics::GraphicsDevice::Default.renderState.setDepth(graphics::DepthState::None);
		graphics::GraphicsDevice::Default.renderState.setBlend(graphics::BlendState::None);
		graphics::GraphicsDevice::Default.renderState.setEffect(graphics::Effects::postProcessBloomAddFilter());
		
		if (graphics::GraphicsDevice::Default.config.refraction)
		{
			graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(),
				refractor._refractVertices.data(),
				refractor._refractIndices.data(),
				refractor._refractIndices.size() / 3);
		}
		else
		{
			graphics::GraphicsDevice::Default.drawPrimitives(graphics::VertexFormats::positionTexture(), _quadVertices, _quadIndices, 2);
		}
	}

	PostProcessBloom::PostProcessBloom() :
		_tonemapId(0),
		_source(nullptr),
		_beginTarget(nullptr),
		_blurMap(nullptr)
	{
	}

	float calcDx(float d)
	{
		return d;// / core::Math::sqrt(4 + 2 * d*d);
	}

	Refractor::Refractor()
	{
		_offset = core::Vector2(8, 8);
		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				core::Vector2 xy = core::Vector2(
					core::Math::lerp(-1.0f, 1.0f, static_cast<float>(y) / static_cast<float>(size - 1)), 
					core::Math::lerp(-1.0f, 1.0f, static_cast<float>(x) / static_cast<float>(size - 1)));
				
				float wave0 = core::Math::sin(xy.getX()* core::Math::Pi*4.0f);
				float wave1 = core::Math::sin(xy.getY()* core::Math::Pi*4.0f);

				float h = wave0 + wave1;

				core::Vector2 uv = core::Vector2(
					core::Math::lerp(0.0f, 1.0f, static_cast<float>(y) / static_cast<float>(size - 1)),
					core::Math::lerp(0.0f, 1.0f, static_cast<float>(x) / static_cast<float>(size - 1)));
				_vertices[y][x].X = xy.getX();
				_vertices[y][x].Y = xy.getY();
				_vertices[y][x].Z = h;
				_vertices[y][x].U = uv.getX();
				_vertices[y][x].V = uv.getY();
				_normals[y][x] = core::Vector2::empty;
			}
		}
		float dz00 = 0;
		float dz01 = 0;
		float dz11 = 0;
		float dz10 = 0;

		float d00 = 0;
		float d01 = 0;
		float d11 = 0;
		float d10 = 0;
		for (int y = 1; y < size - 1; y++)
		{
			for (int x = 1; x < size - 1; x++)
			{
				dz00 = _vertices[y][x].Z - _vertices[y - 1][x - 1].Z;
				dz01 = _vertices[y][x].Z - _vertices[y - 1][x + 1].Z;
				dz11 = _vertices[y][x].Z - _vertices[y + 1][x + 1].Z;
				dz10 = _vertices[y][x].Z - _vertices[y-+ 1][x - 1].Z;

				d00 = calcDx(dz00);
				d01 = calcDx(dz01);
				d11 = calcDx(dz11);
				d10 = calcDx(dz10);
				core::Vector2 n =
					core::Vector2(-d00, -d00) +
					core::Vector2(+d01, -d01) +
					core::Vector2(+d11, +d11) +
					core::Vector2(-d10, +d10);
				_normals[y][x] = n;
				/*
				_vertices[y][x].U += n.getX();
				_vertices[y][x].V += n.getY();
				*/
			}
		}
	}

	void Refractor::update()
	{		
		_refractVertices.clear();
		_refractIndices.clear();
		_velocity = core::Vector2(graphics::GraphicsDevice::Default.config.refractionVelocityX, graphics::GraphicsDevice::Default.config.refractionVelocityY);
		_offset = _velocity * graphics::GraphicsDevice::Default.config.ellapsedTime;
		float s = 0.001f*graphics::GraphicsDevice::Default.config.refractionScale;
		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				PostProcessVertex v = _vertices[y][x];
				core::Vector2 n = getNormal(x + _offset.getX(), y + _offset.getY())*s;
				v.U = v.U + n.getX();
				v.V = v.V + n.getY();
				_refractVertices.push_back(v);
			}
		}
		for (uint i = 0; i < _refractVertices.size(); i++)
		{
			_refractVertices[i].Z = 0;
		}

		for (uint y = 0; y < size-1; y++)
		{
			for (uint x = 0; x < size-1; x++)
			{
				ushort lt = y * size + x;
				ushort rt = y * size + x + 1;
				ushort lb = (y + 1) * size + x;
				ushort rb = (y + 1) * size + x + 1;
				_refractIndices.push_back(lt);
				_refractIndices.push_back(rt);
				_refractIndices.push_back(rb);
				_refractIndices.push_back(lt);
				_refractIndices.push_back(rb);
				_refractIndices.push_back(lb);
			}
		}
	}

	core::Vector2 Refractor::getNormal(float u, float v)
	{
		int iu0 = static_cast<int>(core::Math::trunc(u));
		int iv0 = static_cast<int>(core::Math::trunc(v));
		int iu1 = (iu0 + 1) % size;
		int iv1 = (iv0 + 1) % size;

		float u0 = u - static_cast<float>(iu0);
		float v0 = v - static_cast<float>(iv0);

		iu0 = iu0 % size;
		iv0 = iv0 % size;

		core::Vector2 n00 = _normals[iv0][iu0];
		core::Vector2 n01 = _normals[iv0][iu1];
		core::Vector2 n10 = _normals[iv1][iu0];
		core::Vector2 n11 = _normals[iv1][iu1];

		core::Vector2 n0 = core::Vector2::lerp(n00, n01, u0);
		core::Vector2 n1 = core::Vector2::lerp(n10, n11, u0);
		return core::Vector2::lerp(n0, n1, v0);
	}

	void PostProcessBloom::beginRender(uint tonemapId)
	{
		_tonemapId = tonemapId;
		_source = graphics::GraphicsDevice::Default.PostProcessTargets.pop();
		_beginTarget = graphics::GraphicsDevice::Default.actualRenderTarget;
		graphics::GraphicsDevice::Default.setRenderTarget(_source->getRenderTarget());
		graphics::GraphicsDevice::Default.clear();
	}
	
	void PostProcessBloom::finishRender()
	{	
		if (_tonemapId != 0)
		{
			graphics::IPostProcessTarget * tonemaped = graphics::GraphicsDevice::Default.PostProcessTargets.pop();
			_tonemap.execute(_source->getColor(), tonemaped->getRenderTarget(), _tonemapId);
			graphics::GraphicsDevice::Default.PostProcessTargets.push(_source);
			_source = tonemaped;
		}

		if (graphics::GraphicsDevice::Default.config.useColorCorrection)
		{
			graphics::IPostProcessTarget * corrected = graphics::GraphicsDevice::Default.PostProcessTargets.pop();
			
			core::Vector3 n = core::Vector3(1.0f, 1.0f, 1.0f).normalize();

			core::Matrix correctionMatrix = core::Matrix::CreateRotationWithScale(
				n,
				core::Vector3(
					graphics::GraphicsDevice::Default.config.colorCorrectionScaleX, 
					graphics::GraphicsDevice::Default.config.colorCorrectionScaleX, 
					graphics::GraphicsDevice::Default.config.colorCorrectionScaleY), 
					graphics::GraphicsDevice::Default.config.colorCorrectionRotation);

			core::Matrix3 correctionMatrix3 = core::Matrix3(correctionMatrix);
			_colorFilter.execute(_source->getColor(),  
				n*graphics::GraphicsDevice::Default.config.colorCorrectionOffsetY, 
				correctionMatrix3, corrected->getRenderTarget());
			graphics::GraphicsDevice::Default.PostProcessTargets.push(_source);
			_source = corrected;
		}
		graphics::IPostProcessTarget *downSample1(nullptr);
		graphics::IPostProcessTarget *downSample2(nullptr);
		if(graphics::GraphicsDevice::Default.config.bloomDownsample > 0)
		{
			downSample1 = graphics::GraphicsDevice::Default.PostProcessScaledTargets1.pop();
			downSample2 = graphics::GraphicsDevice::Default.PostProcessScaledTargets2.pop();
		}
		if (_blurMap == nullptr)
			_blurMap = graphics::GraphicsDevice::Default.PostProcessScaledTargets3.pop();
		graphics::IPostProcessTarget *bloom = graphics::GraphicsDevice::Default.PostProcessScaledTargets3.pop();

		if (graphics::GraphicsDevice::Default.config.bloomDownsample > 0)
		{
			_empty.execute(_source->getColor(), downSample1->getRenderTarget());
			_empty.execute(downSample1->getColor(), downSample2->getRenderTarget());
			_empty.execute(downSample2->getColor(), _blurMap->getRenderTarget());
		}
		else
		{
			_empty.execute(_source->getColor(), _blurMap->getRenderTarget());
		}
			
		graphics::UniformValues::pixelSize()->setValue(core::Vector2(
			1.0f / static_cast<float>(bloom->getWidth()),
			1.0f / static_cast<float>(bloom->getHeight())));

		_hBlur.execute(_blurMap->getColor(), bloom->getRenderTarget());
		_vBlur.execute(bloom->getColor(), _blurMap->getRenderTarget());
		_filter.execute(_blurMap->getColor(), bloom->getRenderTarget());
		_add.execute(_source->getColor(), bloom->getColor(), _beginTarget);

		if (graphics::GraphicsDevice::Default.config.vignetting)
		{
			_vignetting.execute(_blurMap->getColor());
		}
		
		//_empty.execute(_source->getDepth(), _beginTarget);
		
		if (graphics::GraphicsDevice::Default.config.bloomDownsample > 0)
		{
			graphics::GraphicsDevice::Default.PostProcessScaledTargets1.push(downSample1);
			graphics::GraphicsDevice::Default.PostProcessScaledTargets2.push(downSample2);
		}
		graphics::GraphicsDevice::Default.PostProcessScaledTargets3.push(bloom);
		
		graphics::GraphicsDevice::Default.PostProcessTargets.push(_source);		
	}

	bool PostProcessBloom::isAvaliable()
	{
		bool test0 = graphics::GraphicsDevice::Default.PostProcessTargets.isAvaliable();
		bool test1 = graphics::GraphicsDevice::Default.PostProcessScaledTargets1.isAvaliable();
		bool test2 = graphics::GraphicsDevice::Default.PostProcessScaledTargets2.isAvaliable();
		bool test3 = graphics::GraphicsDevice::Default.PostProcessScaledTargets3.isAvaliable();
		return test0 && test1 && test2 && test3;
		//return graphics::GraphicsDevice::Default.PostProcessScaledTargets.isAvaliable() && graphics::GraphicsDevice::Default.PostProcessTargets.isAvaliable();
	}

	graphics::Texture* PostProcessBloom::getBlurMap()
	{
		if (_blurMap == nullptr)
			return nullptr;
		return _blurMap->getColor();
	}

	PostProcessVertex* quadVertices()
	{
		return _quadVertices;
	}

	ushort* quadIndices()
	{
		return _quadIndices;
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