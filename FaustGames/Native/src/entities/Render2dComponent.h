#ifndef RENDER_2D_COMPONENT_H
#define RENDER_2D_COMPONENT_H

#include "entities_classes.h"
#include "Component.h"

namespace entities
{
	class Render2dComponent : public Component, public llge::IRender2d
	{
	public:
		int MeshesCount;
		drawing::RenderMesh2d ** RenderItems;
		static ComponentType::e Type;
		Render2dComponent() : RenderItems(0), MeshesCount(0)
		{
		}
		
		virtual void API_CALL setMeshesCount(int meshesCount)
		{
			if (MeshesCount == meshesCount) return;
			for (int i = 0; i < MeshesCount; i++)
				core::Mem::dispose(RenderItems[i]);
			MeshesCount = meshesCount;
			if (RenderItems)
				core::Mem::deallocate(RenderItems);
			RenderItems = (drawing::RenderMesh2d **)core::Mem::allocate(MeshesCount * sizeof(drawing::RenderMesh2d *));
			for (int i = 0; i < MeshesCount; i++)
				RenderItems[i] = core::Mem::construct<drawing::RenderMesh2d>();
		}

		virtual void API_CALL setMesh(int meshIndex, llge::ITexture *texture, void* vertices, int verticesCount, void* indices, int indicesCount)
		{
			drawing::RenderMesh2d *mesh = RenderItems[meshIndex];
			mesh->setData(texture, (drawing::Mesh2dVertex *)vertices, verticesCount, (unsigned short *)indices, indicesCount);
		}

		virtual ~Render2dComponent()
		{
			for (int i = 0; i < MeshesCount; i++)
				core::Mem::dispose(RenderItems[i]);
			if (RenderItems)
				core::Mem::deallocate(RenderItems);
		}
	private:
	};
}

#endif /*RENDER_2D_COMPONENT_H*/