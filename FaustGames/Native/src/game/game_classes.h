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

	struct EmptyArgs
	{
		
	};

	struct UpdateArgs
	{
		Scene* scene;
	};

	struct LoadedArgs
	{
		Scene* scene;
	};

	struct RenderArgs
	{
		Scene* scene;
	};

	struct LoadArgs
	{
		Scene* scene;
		content::ContentValue* value;
		content::ContentBlock* content;
	};

	struct InputStatus
	{
		enum e : byte
		{
			None = 0,
			Handled = 1,
		};
	};

	struct InputOrder
	{
		enum e : byte
		{
			None = 0,
			Gui = 1,
			Default = 2,
		};
	};

	struct UpdateOrder
	{
		enum e : byte
		{
			None = 0,
			Behaviors = 1,
			Animations = 2
		};
	};

	struct RenderOrder
	{
		enum e : byte
		{
			None = 0,
			Default = 1,
			Gui = 2,
		};
	};
}

#endif /*GAME_CLASSES_H*/
