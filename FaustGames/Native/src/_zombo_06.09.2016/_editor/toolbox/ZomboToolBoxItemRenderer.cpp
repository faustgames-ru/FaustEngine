#include "ZomboToolBoxItemRenderer.h"
#include "../ZomboEditorRenderService.h"

namespace zombo
{
	ZomboToolBoxItemRendererEntry::ZomboToolBoxItemRendererEntry(): startVertex(0), verticesCount(0), startIndex(0), indicesCount(0), texture(nullptr)
	{
	}

	ZomboToolBoxItemRenderer::ZomboToolBoxItemRenderer(): _actualTexture(nullptr)
	{
	}

	ZomboToolBoxItemRenderer::~ZomboToolBoxItemRenderer()
	{
	}

	void ZomboToolBoxItemRenderer::clear()
	{
		_vertices.clear();
		_indices.clear();
		_entries.clear();
		_actualTexture = nullptr;
		_bounds = geometry::Aabb2d();
	}

	void ZomboToolBoxItemRenderer::drawTextured(graphics::Texture* texture, RenderVertex* vertices, int verticesCount, ushort* indices, int primitivesCount)
	{
		if (_actualTexture != texture)
		{
			_entries.push_back(ZomboToolBoxItemRendererEntry());
			_entries.back().texture = texture;
		}
		uint verticesStart = _vertices.size();
		for (uint i = 0; i < verticesCount; i++)
		{
			_vertices.push_back(vertices[i]);
			_bounds.expand(vertices[i].xyz.getX(), vertices[i].xyz.getY());
		}
		uint idicesSize = primitivesCount * 3;
		for (uint i = 0; i < idicesSize; i++)
		{
			_indices.push_back(indices[i]+ verticesStart);
		}
		_entries.back().indicesCount += idicesSize;
		_entries.back().verticesCount += verticesCount;
	}

	void ZomboToolBoxItemRenderer::apply(const core::Vector2& position, const core::Vector2& mirror, float scale, float alpha)
	{
		for (uint i = 0, count = _vertices.size(); i < count; i++)
		{
			_vertices[i].xyz *= scale;
			_vertices[i].xyz += position;
			_vertices[i].color = graphics::Color::mulA(_vertices[i].color, alpha);
		}
		for (uint i = 0, count = _entries.size(); i < count; i++)
		{
			ZomboEditorRenderService::Gui.drawTrianglesTextured(_entries[i].texture, 
				_vertices.data()+ _entries[i].startVertex, _entries[i].verticesCount,
				_indices.data() + _entries[i].startIndex, _entries[i].indicesCount / 3);
		}

		for (uint i = 0, count = _vertices.size(); i < count; i++)
		{
			_vertices[i].xyz -= position;
			float a = core::Math::saturate(1.0f - (_vertices[i].xyz.getY() / scale - _bounds.Min.getY()) / (_bounds.Max.getY() - _bounds.Min.getY()));
			a *= 0.5f;
			_vertices[i].color = graphics::Color::mulA(_vertices[i].color, a);
			_vertices[i].xyz.inverseY();
			_vertices[i].xyz += mirror;

		}

		for (uint i = 0, count = _entries.size(); i < count; i++)
		{
			ZomboEditorRenderService::Gui.drawTrianglesTextured(_entries[i].texture,
				_vertices.data() + _entries[i].startVertex, _entries[i].verticesCount,
				_indices.data() + _entries[i].startIndex, _entries[i].indicesCount / 3);
		}
	}
}
