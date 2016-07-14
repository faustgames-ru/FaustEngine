#ifndef RAZOR_DRAWING_H
#define RAZOR_DRAWING_H

#include "razor.h"

#include "../razor_classes.h"
#include "../../graphics/pipelines/RenderVertex.h"
#include "../../geometry/Aabb.h"

namespace razor
{
	struct SpriteTransform
	{
		core::Vector3 position;
		float s;
		float c;
		SpriteTransform();
		SpriteTransform(core::Vector3 position);
		SpriteTransform(core::Vector3 position, float scale);
		SpriteTransform(core::Vector3 position, float scale, float rotation);
		core::Vector3 tansform(core::Vector3 p) const;
		core::Vector3 tansform(core::Vector2 p) const;
	};

	struct Pen
	{
		uint color;
		float width;
		Pen();
	};
	
	class Drawing
	{
	public:
		static Drawing Default;
		Pen pen;
		Drawing();
		void drawSprite(const SpriteTransform& transform, content::ContentImage* sprite);
		void drawSprite(const SpriteTransform& transform, uint color, content::ContentImage* sprite);
		void drawAabb(const geometry::Aabb& aabb);
		void drawLine(const core::Vector3& a, const core::Vector3& b);
		void drawLine(float x1, float y1, float z1, float x2, float y2, float z2);
	private:
		std::vector<graphics::SpriteVertex> _spriteVertices;
		std::vector<graphics::ColorVertex> _solidVertices;
		ushort _quadIndices[6];
	};
}

#endif
