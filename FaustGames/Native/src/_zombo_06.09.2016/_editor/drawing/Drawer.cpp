#include "Drawer.h"

namespace zombo
{
	ZomboDrawer ZomboDrawer::Default(&ZomboEditorRenderService::Default);
	ZomboDrawer ZomboDrawer::Gui(&ZomboEditorRenderService::Gui);


	ZomboDrawer::ZomboDrawer(ZomboEditorRenderService* renderService): _renderService(renderService), _smoothR(0), _scaleToPixels(0)
	{
	}

	void ZomboDrawer::setSmoothR(float smoothR)
	{
		_smoothR = smoothR;
	}

	void ZomboDrawer::setScaleToPixels(float scale)
	{
		_scaleToPixels = scale;
	}

	void ZomboDrawer::vignetting(uint color, const core::Vector3& p, const core::Vector2& outSize, const core::Vector2& inSize)
	{
		uint count = 64;
		float a = 0;
		float da = 2.0f * core::Math::Pi / count;
		

		float ow = outSize.getX() * 0.5f;
		float oh = outSize.getY() * 0.5f;
		float iw = inSize.getX() * 0.5f;
		float ih = inSize.getY() * 0.5f;

		_colorVertices.clear();
		_indices.clear();
		for (uint i = 0; i < count; i++)
		{
			core::Vector3 cp0 = p + core::Vector2(ow * core::Math::cos(a), oh * core::Math::sin(a));
			core::Vector3 cp1 = p + core::Vector2(iw * core::Math::cos(a), ih * core::Math::sin(a));
			a += da;
			_colorVertices.push_back(ColorVertex(cp0, color));
			_colorVertices.push_back(ColorVertex(cp1, 0));

			uint i0 = i * 2 + 0;
			uint i1 = i * 2 + 1;
			uint i2 = ((i + 1) % count) * 2 + 1;
			uint i3 = ((i + 1) % count) * 2 + 0;

			_indices.push_back(i0);
			_indices.push_back(i1);
			_indices.push_back(i2);
			_indices.push_back(i0);
			_indices.push_back(i2);
			_indices.push_back(i3);
		}

		_renderService->drawTriangles(_colorVertices.data(), _colorVertices.size(), _indices.data(), _indices.size() / 3);

	}
	/*
	{
		_colorVertices.clear();
		_indices.clear();

		float ow = outSize.getX() * 0.5f;
		float oh = outSize.getY() * 0.5f;
		float iw = inSize.getX() * 0.5f;
		float ih = inSize.getY() * 0.5f;

		_colorVertices.push_back(ColorVertex(p + core::Vector2(-ow, -oh), color));
		_colorVertices.push_back(ColorVertex(p + core::Vector2(ow, -oh), color));
		_colorVertices.push_back(ColorVertex(p + core::Vector2(ow, oh), color));
		_colorVertices.push_back(ColorVertex(p + core::Vector2(-ow, oh), color));

		_colorVertices.push_back(ColorVertex(p + core::Vector2(-iw, -ih), 0));
		_colorVertices.push_back(ColorVertex(p + core::Vector2(iw, -ih), 0));
		_colorVertices.push_back(ColorVertex(p + core::Vector2(iw, ih), 0));
		_colorVertices.push_back(ColorVertex(p + core::Vector2(-iw, ih), 0));

		_indices.push_back(0);
		_indices.push_back(1);
		_indices.push_back(5);
		_indices.push_back(0);
		_indices.push_back(5);
		_indices.push_back(4);

		_indices.push_back(1);
		_indices.push_back(2);
		_indices.push_back(6);
		_indices.push_back(1);
		_indices.push_back(6);
		_indices.push_back(5);

		_indices.push_back(2);
		_indices.push_back(3);
		_indices.push_back(7);
		_indices.push_back(2);
		_indices.push_back(7);
		_indices.push_back(6);

		_indices.push_back(3);
		_indices.push_back(0);
		_indices.push_back(4);
		_indices.push_back(3);
		_indices.push_back(4);
		_indices.push_back(7);

		_renderService->drawTriangles(_colorVertices.data(), _colorVertices.size(), _indices.data(), _indices.size() / 3);
	}
	*/
	void ZomboDrawer::fillCircle(uint color, const core::Vector3 &p, float r)
	{
		float len = 2.0f * core::Math::Pi * r;
		float lenPx = len * _scaleToPixels;
		uint count = core::Math::round(lenPx / ZomboConstants::circlesDetail);
		if (count > 64)
		{
			count = 64;
		}
		if (count < 6)
		{
			count = 6;
		}
		float a = 0;
		float da = 2.0f * core::Math::Pi / count;
		_colorVertices.clear();
		_indices.clear();
		_colorVertices.push_back(ColorVertex(p, color));
		for (uint i = 0; i < count; i++)
		{
			core::Vector3 cp0 = p + core::Vector2(r * core::Math::cos(a), r * core::Math::sin(a));;
			a += da;
			_colorVertices.push_back(ColorVertex(cp0, color));
			
			_indices.push_back(0);
			_indices.push_back(i + 1);
			_indices.push_back(1 + (i + 1) % count);
		}
		uint startIndex = _colorVertices.size();
		a = da*0.5f;
		color = graphics::Color::mulA(color, 0.0f);
		float sr = r + _smoothR / _scaleToPixels;
		for (uint i = 0; i < count; i++)
		{
			core::Vector3 cp1 = p + core::Vector2(sr * core::Math::cos(a), sr * core::Math::sin(a));
			a += da;
			_colorVertices.push_back(ColorVertex(cp1, color));

			_indices.push_back(i + 1);
			_indices.push_back(1 + (i + 1) % count);
			_indices.push_back(startIndex + i);
			
			_indices.push_back(1 + (i + 1) % count);
			_indices.push_back(startIndex + i);
			_indices.push_back(startIndex + (i + 1) % count);
			
		}
		_renderService->drawTriangles(_colorVertices.data(), _colorVertices.size(), _indices.data(), _indices.size() / 3);
	}

	void ZomboDrawer::fillRing(uint color, const core::Vector3& p, float r, float d)
	{
		float len = 2.0f * core::Math::Pi * r;
		float lenPx = len * _scaleToPixels;
		uint count = core::Math::round(lenPx / ZomboConstants::circlesDetail);
		if (count > 64)
		{
			count = 64;
		}
		if (count < 3)
		{
			count = 3;
		}
		float a = 0;
		float da = 2.0f * core::Math::Pi / count;
		d *= 0.5f;
		_colorVertices.clear();
		_indices.clear();
		for (uint i = 0; i < count; i++)
		{
			core::Vector3 cp0 = p + core::Vector2((r + d) * core::Math::cos(a), (r + d) * core::Math::sin(a));
			core::Vector3 cp1 = p + core::Vector2((r - d) * core::Math::cos(a), (r - d) * core::Math::sin(a));
			a += da;
			_colorVertices.push_back(ColorVertex(cp0, color));
			_colorVertices.push_back(ColorVertex(cp1, color));

			uint i0 = i * 2 + 0;
			uint i1 = i * 2 + 1;
			uint i2 = ((i + 1) % count) * 2 + 1;
			uint i3 = ((i + 1) % count) * 2 + 0;

			_indices.push_back(i0);
			_indices.push_back(i1);
			_indices.push_back(i2);
			_indices.push_back(i0);
			_indices.push_back(i2);
			_indices.push_back(i3);
		}

		float sr = _smoothR / _scaleToPixels;
		uint tcolor = graphics::Color::mulA(color, 0);
		uint startIndex = _colorVertices.size();
		for (uint i = 0; i < count; i++)
		{
			core::Vector3 cp0 = p + core::Vector2((r + d+ sr) * core::Math::cos(a), (r + d+ sr) * core::Math::sin(a));
			core::Vector3 cp1 = p + core::Vector2((r + d) * core::Math::cos(a), (r + d) * core::Math::sin(a));
			a += da;
			_colorVertices.push_back(ColorVertex(cp0, tcolor));
			_colorVertices.push_back(ColorVertex(cp1, color));

			uint i0 = startIndex + i * 2 + 0;
			uint i1 = startIndex + i * 2 + 1;
			uint i2 = startIndex + ((i + 1) % count) * 2 + 1;
			uint i3 = startIndex + ((i + 1) % count) * 2 + 0;

			_indices.push_back(i0);
			_indices.push_back(i1);
			_indices.push_back(i2);
			_indices.push_back(i0);
			_indices.push_back(i2);
			_indices.push_back(i3);
		}

		startIndex = _colorVertices.size();
		for (uint i = 0; i < count; i++)
		{
			core::Vector3 cp0 = p + core::Vector2((r - d) * core::Math::cos(a), (r - d) * core::Math::sin(a));
			core::Vector3 cp1 = p + core::Vector2((r - d - sr) * core::Math::cos(a), (r - d - sr) * core::Math::sin(a));
			a += da;
			_colorVertices.push_back(ColorVertex(cp0, color));
			_colorVertices.push_back(ColorVertex(cp1, tcolor));

			uint i0 = startIndex + i * 2 + 0;
			uint i1 = startIndex + i * 2 + 1;
			uint i2 = startIndex + ((i + 1) % count) * 2 + 1;
			uint i3 = startIndex + ((i + 1) % count) * 2 + 0;

			_indices.push_back(i0);
			_indices.push_back(i1);
			_indices.push_back(i2);
			_indices.push_back(i0);
			_indices.push_back(i2);
			_indices.push_back(i3);
		}
		_renderService->drawTriangles(_colorVertices.data(), _colorVertices.size(), _indices.data(), _indices.size() / 3);

	}

	void ZomboDrawer::drawRect(uint color, const core::Vector3& p, const core::Vector2& size)
	{
		_colorVertices.clear();
		_indices.clear();
		_colorVertices.push_back(ColorVertex(p, color));
		_colorVertices.push_back(ColorVertex(p.getX() + size.getX(), p.getY(), p.getZ(), color));
		_colorVertices.push_back(ColorVertex(p.getX() + size.getX(), p.getY() + size.getY(), p.getZ(), color));
		_colorVertices.push_back(ColorVertex(p.getX(), p.getY() + size.getY(), p.getZ(), color));
		_indices.push_back(0);
		_indices.push_back(1);
		_indices.push_back(1);
		_indices.push_back(2);
		_indices.push_back(2);
		_indices.push_back(3);
		_indices.push_back(3);
		_indices.push_back(0);
		_renderService->drawLines(_colorVertices.data(), _colorVertices.size(), _indices.data(), _indices.size() / 2);
	}
	void ZomboDrawer::fillRect(uint color, const core::Vector3& p, const core::Vector2& size)
	{
		_colorVertices.clear();
		_indices.clear();
		_colorVertices.push_back(ColorVertex(p, color));
		_colorVertices.push_back(ColorVertex(p.getX() + size.getX(), p.getY(), p.getZ(), color));
		_colorVertices.push_back(ColorVertex(p.getX() + size.getX(), p.getY() + size.getY(), p.getZ(), color));
		_colorVertices.push_back(ColorVertex(p.getX(), p.getY() + size.getY(), p.getZ(), color));
		_indices.push_back(0);
		_indices.push_back(1);
		_indices.push_back(2);
		_indices.push_back(0);
		_indices.push_back(2);
		_indices.push_back(3);
		_renderService->drawTriangles(_colorVertices.data(), _colorVertices.size(), _indices.data(), _indices.size() / 3);

	}
	void ZomboDrawer::fillQuad(uint color, const core::Vector3& p, float r, float angle)
	{
		uint count = 4;
		float a = angle;
		float da = 2.0f * core::Math::Pi / count;
		_colorVertices.clear();
		_indices.clear();
		_colorVertices.push_back(ColorVertex(p, color));
		for (uint i = 0; i < count; i++)
		{
			core::Vector3 cp0 = p + core::Vector2(r * core::Math::cos(a), r * core::Math::sin(a));;
			a += da;
			_colorVertices.push_back(ColorVertex(cp0, color));

			_indices.push_back(0);
			_indices.push_back(i + 1);
			_indices.push_back(1 + (i + 1) % count);
		}
		uint startIndex = _colorVertices.size();
		a = angle;
		color = graphics::Color::mulA(color, 0.0f);
		float sr = r + _smoothR / _scaleToPixels;
		for (uint i = 0; i < count; i++)
		{
			core::Vector3 cp1 = p + core::Vector2(sr * core::Math::cos(a), sr * core::Math::sin(a));
			a += da;
			_colorVertices.push_back(ColorVertex(cp1, color));

			_indices.push_back(i + 1);
			_indices.push_back(1 + (i + 1) % count);
			_indices.push_back(startIndex + i);

			_indices.push_back(1 + (i + 1) % count);
			_indices.push_back(startIndex + i);
			_indices.push_back(startIndex + (i + 1) % count);

		}
		_renderService->drawTriangles(_colorVertices.data(), _colorVertices.size(), _indices.data(), _indices.size() / 3);
	}

	void ZomboDrawer::fillEdge(uint color, const core::Vector3& p0, const core::Vector3& p1, float r)
	{
		_colorVertices.clear();
		_indices.clear();
		core::Vector2 n = (p1 - p0).toVector2().rotate90cw().normalize()*r;		

		_colorVertices.push_back(ColorVertex(p0 + n, color));
		_colorVertices.push_back(ColorVertex(p1 + n, color));
		_colorVertices.push_back(ColorVertex(p1 - n, color));
		_colorVertices.push_back(ColorVertex(p0 - n, color));

		color = graphics::Color::mulA(color, 0.0f);
		n = (p1 - p0).toVector2().rotate90cw().normalize()*(r + _smoothR / _scaleToPixels);

		_colorVertices.push_back(ColorVertex(p0 + n, color));
		_colorVertices.push_back(ColorVertex(p1 + n, color));
		_colorVertices.push_back(ColorVertex(p1 - n, color));
		_colorVertices.push_back(ColorVertex(p0 - n, color));

		_indices.push_back(0);
		_indices.push_back(1);
		_indices.push_back(2);
		_indices.push_back(0);
		_indices.push_back(2);
		_indices.push_back(3);

		_indices.push_back(0);
		_indices.push_back(4);
		_indices.push_back(1);
		_indices.push_back(1);
		_indices.push_back(5);
		_indices.push_back(4);

		_indices.push_back(2);
		_indices.push_back(6);
		_indices.push_back(3);
		_indices.push_back(3);
		_indices.push_back(7);
		_indices.push_back(6);

		_renderService->drawTriangles(_colorVertices.data(), _colorVertices.size(), _indices.data(), _indices.size() / 3);
	}

	void ZomboDrawer::fillArrow(uint color, const core::Vector3& p0, const core::Vector3& p1, float r)
	{
		_colorVertices.clear();
		_indices.clear();
		core::Vector2 n = (p1 - p0).toVector2().rotate90cw().normalize()*r;

		_colorVertices.push_back(ColorVertex(p0 + n, color));//0
		_colorVertices.push_back(ColorVertex(p1, color));//1
		_colorVertices.push_back(ColorVertex(p0 - n, color));//2

		color = graphics::Color::mulA(color, 0.0f);
		n = (p1 - p0).toVector2().rotate90cw().normalize()*(r + _smoothR / _scaleToPixels);

		_colorVertices.push_back(ColorVertex(p0 + n, color));//3
		_colorVertices.push_back(ColorVertex(p0 - n, color));//4

		_indices.push_back(0);
		_indices.push_back(1);
		_indices.push_back(2);

		_indices.push_back(0);
		_indices.push_back(3);
		_indices.push_back(1);
		_indices.push_back(2);
		_indices.push_back(1);
		_indices.push_back(4);

		_renderService->drawTriangles(_colorVertices.data(), _colorVertices.size(), _indices.data(), _indices.size() / 3);
	}

	void ZomboDrawer::drawEdge(uint color, const core::Vector3& p0, const core::Vector3& p1)
	{
		_colorVertices.clear();
		_indices.clear();

		_colorVertices.push_back(ColorVertex(p0, color));//0
		_colorVertices.push_back(ColorVertex(p1, color));//1


		_indices.push_back(0);
		_indices.push_back(1);
		_renderService->drawLines(_colorVertices.data(), _colorVertices.size(), _indices.data(), _indices.size() / 2);
	}

	void ZomboDrawer::drawSprite(uint color, float rotaion, float scale, const core::Vector3& p, ZomboContentImage* image)
	{
		_renderVertices.clear();
		_indices.clear();

		float c = core::Math::cos(rotaion);
		float s = core::Math::sin(rotaion);
		for (uint i = 0; i < image->vertices.size(); i++)
		{
			ZomboImageVertex v = image->vertices[i];			
			_renderVertices.push_back(RenderVertex(p + v.xy.rotate(c, s)*scale, color, v.u, v.v));
		}
		_renderService->drawTrianglesTextured(image->texture, _renderVertices.data(), _renderVertices.size(), image->indices.data(), image->indices.size() / 3);
	}
}
