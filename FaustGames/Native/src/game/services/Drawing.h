#ifndef GAME_SERVICES_DRAWING_H
#define GAME_SERVICES_DRAWING_H

#include "../game_classes.h"
#include "../../graphics/pipelines/RenderVertex.h"
#include "../../geometry/Aabb.h"

namespace game
{
	struct Rotation
	{
		float s;
		float c;
		Rotation();
		Rotation(float scale);
		Rotation(float scale, float rotation);
	};
	struct SpriteTransform
	{
		core::Vector3 position;
		Rotation rotation;
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
		void fillRect(const core::Vector3& center, const core::Vector2& size);
	private:
		std::vector<graphics::SpriteVertex> _spriteVertices;
		std::vector<graphics::ColorVertex> _solidVertices;
		ushort _quadIndices[6];
	};
}

#endif /*GAME_SERVICES_DRAWING_H*/
