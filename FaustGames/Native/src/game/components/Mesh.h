#ifndef GAME_COMPONENTS_MESH_H
#define GAME_COMPONENTS_MESH_H

#include "../game_classes.h"
#include "../Component.h"
#include "../services/Drawing.h"
#include "../../content/ContentMesh.h"

namespace game
{
	class Mesh : public Component
	{
	public:
		core::Matrix3 transform;
		Mesh();
		virtual void enqueueResources(const LoadArgs& e) OVERRIDE;
		virtual void render(const RenderArgs& e) OVERRIDE;
	private:
		void resourcesLoaded(content::ContentStatus* status);
		content::ContentMesh* _contentMesh;
	};
}

#endif /*GAME_COMPONENTS_MESH_H*/
