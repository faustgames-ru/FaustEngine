#ifndef ENTITIY_H
#define ENTITIY_H

#include "entities_classes.h"

namespace entities
{
	struct LayerIndex
	{
		float Z;
		int LayerIndex;
	};

	struct BoundingBox
	{
		geometry::Aabb2d Aabb;
	};

	struct MeshVertex
	{
		float x;
		float y;
		float z;
		short u;
		short v;
		unsigned int Color;

		inline static graphics::VertexFormat *getFormat(){ return graphics::VertexFormats::positionTextureColor(); }
	};

	struct Sprite
	{
		MeshVertex Vertices[4];
		int imageId;

		inline static int size() { return 4; }		
		typedef core::FixedPool<Sprite, 16384> Pool;
	};

	class Entity
	{
	public:
		Entity(StaticComponentsMasks::e componentsSet);
	protected:
	private:
		core::StaticArray<int, StaticComponentsTypes::ComponentsCount> _cpmponentsIds;
	};

	class AabbCullingSystem
	{
	public:
		AabbCullingSystem() : _mask(StaticComponentsMasks::Aabb)
		{
		}
		
		void update()
		{
			// resort copmponents with aabbvisibility
		}
	private:
		int _mask;
		geometry::FilterResult _filterResults;
	};
	
	class StaticRenderLayer
	{
	};

	class DynamicRenderLayer
	{
	};

	class SpritesRenderSystem
	{
	public:
		SpritesRenderSystem() : _mask(StaticComponentsMasks::Aabb | StaticComponentsMasks::Sprite | StaticComponentsMasks::LayerIndex)
		{
		}
		void update()
		{
			// query visible entities with sprite copmponents
			// sort sprites with layers
			// sort Dynamic layer
			// batch layers
			// render layers
		}
	private:
		int _mask;

	};
}

#endif /*ENTITIY_H*/

