#ifndef COMPONENT_MESH_2D_H
#define COMPONENT_MESH_2D_H

#include "entities_classes.h"

namespace entities
{
	struct MeshVertex2d
	{
		float X;
		float Y;
		float Z;
		unsigned short U;
		unsigned short V;
		unsigned int Color;
	};
	
	struct ComponentMesh2d
	{
		typedef core::HeapPool<EntitiesConstants::MeshHeapSize> Pool;

		MeshVertex2d *Vertices;
		int VerticesCount;
		unsigned short *Indices;
		int IndicesCount;
		
		ComponentMesh2d()
		{
			Vertices = 0;
			VerticesCount = 0;
			Indices = 0;
			IndicesCount = 0;
		}

		ComponentMesh2d(int verticesCount, int indicesCount)
		{
			VerticesCount = verticesCount;
			IndicesCount = indicesCount;
			Vertices = (MeshVertex2d *)Pool::alloc(verticesCount * sizeof(MeshVertex2d));
			Indices = (unsigned short *)Pool::alloc(indicesCount * sizeof(unsigned short));
		}

		~ComponentMesh2d()
		{
		}

		static ComponentMesh2d *get(int id)
		{
			return (ComponentMesh2d *)Pool::getDataByOffset(id);
		}

		static int createId(int verticesCount, int indicesCount)
		{
			ComponentMesh2d *result = new (Pool::alloc(sizeof(ComponentMesh2d))) ComponentMesh2d();
			return Pool::getOffset(result);
		}

		static ComponentMesh2d *create(int verticesCount, int indicesCount)
		{
			return new (Pool::alloc(sizeof(ComponentMesh2d))) ComponentMesh2d();
		}

		static void releaseAll()
		{
			Pool::clear();
		}
	};
}

#endif /*COMPONENT_MESH_2D_H*/
