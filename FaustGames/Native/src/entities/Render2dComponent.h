#ifndef RENDER_2D_COMPONENT_H
#define RENDER_2D_COMPONENT_H

#include "entities_classes.h"
#include "Component.h"

namespace entities
{
	class Render2dComponent : public Component, public llge::IRender2d
	{
	public:
		drawing::RenderItem2d * RenderItem;
		static ComponentType::e Type;
		Render2dComponent() : RenderItem(0)
		{
		}
		virtual void API_CALL setMesh(llge::ITexture *texture, void* vertices, int verticesCount, void* indices, int indicesCount)
		{
			if (RenderItem != 0)
				core::Mem::dispose(RenderItem);
			drawing::RenderMesh2d *mesh = core::Mem::construct<drawing::RenderMesh2d>();
			mesh->setData(texture, (drawing::Mesh2dVertex *)vertices, verticesCount, (unsigned short *)indices, indicesCount);
			RenderItem = mesh;
		}
		virtual ~Render2dComponent()
		{
			if (RenderItem != 0)
				core::Mem::dispose(RenderItem);
		}
	private:
	};
}

#endif /*RENDER_2D_COMPONENT_H*/