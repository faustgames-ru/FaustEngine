#ifndef ZOMBO_TOOL_BOX_ITEM_RENDERER_H
#define ZOMBO_TOOL_BOX_ITEM_RENDERER_H

#include "../../zombo_classes.h"

namespace zombo
{
	class ZomboToolBoxItemRendererEntry
	{
	public:
		uint startVertex;
		uint verticesCount;
		uint startIndex;
		uint indicesCount;
		graphics::Texture* texture;
		ZomboToolBoxItemRendererEntry();
	};

	class ZomboToolBoxItemRenderer : public IBaseObject
	{
	public:
		ZomboToolBoxItemRenderer();
		~ZomboToolBoxItemRenderer();
		void clear();
		void drawTextured(graphics::Texture* texture, RenderVertex* vertices, int verticesCount, ushort* indices, int primitivesCount);
		void apply(const core::Vector2 &position, const core::Vector2 &mirror, float scale, float alpha);
	private:
		std::vector<RenderVertex> _vertices;
		std::vector<ushort> _indices;
		std::vector<ZomboToolBoxItemRendererEntry> _entries;
		graphics::Texture* _actualTexture;
		geometry::Aabb2d _bounds;
	};
}

#endif /*ZOMBO_TOOL_BOX_ITEM_RENDERER_H*/