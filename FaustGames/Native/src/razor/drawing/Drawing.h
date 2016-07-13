#ifndef RAZOR_DRAWING_H
#define RAZOR_DRAWING_H

#include "razor.h"

#include "../razor_classes.h"

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

	class Drawing
	{
	public:
		static Drawing Default;
		void drawSprite(const SpriteTransform& transform, content::ContentImage* sprite);
	};
}

#endif
