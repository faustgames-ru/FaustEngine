#ifndef GAME_CLASSES_H
#define GAME_CLASSES_H

#include "llge.h"
#include "game_platform.h"
#include "game_references.h"
#include "../content/ContentBlock.h"

namespace game
{
	class Scene;
	class Entity;
	class Component;

	struct UpdateArgs
	{
		Scene* scene;
	};

	struct LoadArgs
	{
		content::ContentValue* value;
		content::ContentBlock* content;
	};

	struct UpdateOrder
	{
		enum e
		{
			None = 0,
			Behaviors = 1,
			Animations = 2
		};
	};

	struct RenderOrder
	{
		enum e
		{
			None = 0,
			Default = 1,
			Gui = 2,
		};
	};
}

#endif /*GEOMETRY_CLASSES_H*/
