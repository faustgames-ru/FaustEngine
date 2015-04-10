// llge.cpp : Defines the exported functions for the DLL application.
//

#include "llge.h"

#include "../entities/entities.h"
#include "../drawing/RenderMesh2d.h"

namespace llge
{
	/*
	class CEntity : public IEntity
	{
	public:
		entities::EntitiesWorld *World;
		entities::Entity * Entity;
		entities::Aadd2dComponent *AaddComponent;
		entities::Render2dComponent *RenderComponent;
		entities::Transform2dComponent * TransformComponent;
		drawing::RenderMesh2d *RenderMesh;

		CEntity(entities::EntitiesWorld *world) : World(world)
		{
			Entity = World->createEntity();
			
			AaddComponent = Entity->createComponent<entities::Aadd2dComponent>();
			TransformComponent = Entity->createComponent<entities::Transform2dComponent>();
			RenderComponent = Entity->createComponent<entities::Render2dComponent>();
			RenderComponent->RenderItem = RenderMesh = core::Mem::construct<drawing::RenderMesh2d>();
		}

		virtual void* API_CALL getInternalInstance()
		{
			return (void*)Entity;
		}

		virtual void API_CALL setBounds(float minX, float minY, float maxX, float maxY, float zOrder)
		{
			AaddComponent->Aadd = geometry::Aabb2d(minX, minY, maxX, maxY);
			AaddComponent->ZOrder = zOrder;
		}

		virtual void API_CALL setMesh(ITexture *texture, void* vertices, int verticesCount, void* indices, int indicesCount)
		{
			RenderMesh->setData(texture->getId(), (drawing::Mesh2dVertex *)vertices, verticesCount, (ushort *)indices, indicesCount);
		}

		virtual void API_CALL setWorldPosition(float x, float y, float z)
		{
			TransformComponent->WorldTransform.Position.setData(x, y, z);
			TransformComponent->WorldTransformState = entities::Transform2dState::Updated;
			World->updateComponent(Entity, entities::ComponentType::Transform2d);
		}

		virtual void API_CALL setWorldRotation(float value)
		{
			TransformComponent->WorldTransform.Rotation = value;
			TransformComponent->WorldTransformState = entities::Transform2dState::Updated;
			World->updateComponent(Entity, entities::ComponentType::Transform2d);
		}

		virtual void API_CALL setWorldScale(float value)
		{
			TransformComponent->WorldTransform.Scale = value;
			TransformComponent->WorldTransformState = entities::Transform2dState::Updated;
			World->updateComponent(Entity, entities::ComponentType::Transform2d);
		}

		virtual void API_CALL setLocalPivot(float x, float y, float z)
		{
			TransformComponent->LocalTransform.Pivot.setData(x, y, z);
			TransformComponent->LocalTransformState = entities::Transform2dState::Updated;
			World->updateComponent(Entity, entities::ComponentType::Transform2d);
		}

		virtual void API_CALL setLocalPosition(float x, float y, float z)
		{
			TransformComponent->LocalTransform.Position.setData(x, y, z);
			TransformComponent->LocalTransformState = entities::Transform2dState::Updated;
			World->updateComponent(Entity, entities::ComponentType::Transform2d);
		}

		
		virtual void API_CALL setLocalRotation(float value) 
		{
			TransformComponent->LocalTransform.Rotation = value;
			TransformComponent->LocalTransformState = entities::Transform2dState::Updated;
			World->updateComponent(Entity, entities::ComponentType::Transform2d);
		}
		
		virtual void API_CALL setLocalScale(float value)
		{
			TransformComponent->LocalTransform.Scale = value;
			TransformComponent->LocalTransformState = entities::Transform2dState::Updated;
			World->updateComponent(Entity, entities::ComponentType::Transform2d);
		}

		virtual void API_CALL addToWorld()
		{
			World->addEntity(Entity);
		}
		virtual void API_CALL removeFromWorld()
		{
			World->removeEntity(Entity);
		}
		
		virtual void API_CALL dispose()
		{
			Entity->dispose();
			delete this;
		}
	};
	*/
	class CCamera : public ICamera
	{
	public:
		drawing::Camera *Camera;
		virtual void API_CALL setPosition(float x, float y, float z)
		{
			Camera->setPosition(x, y, z);
		}
		
		virtual void API_CALL setFov(float fov)
		{
			Camera->setFov(fov);
		}
		
		virtual void API_CALL setAspect(float aspect)
		{
			Camera->setAspect(aspect);
		}
		
		virtual void API_CALL setRotation(float rotationZ)
		{
			Camera->setRotation(rotationZ);
		}
		
		virtual void API_CALL setPlanes(float zn, float zf)
		{
			Camera->setPlanes(zn, zf);
		}
	};

	class CEntitiesWorld : public IEntitiesWorld
	{
	public:
		entities::EntitiesWorld EntitiesWorld;
		entities::QuadtreeEntitiesFilter Filter;
		entities::RenderSystem2d RenderSystem;
		entities::TransformSystem2d TransformSystem;
		CCamera Camera;

		CEntitiesWorld()
		{
			EntitiesWorld.setFilter(&Filter);
			EntitiesWorld.addUpdateSystem(&TransformSystem);
			EntitiesWorld.addRenderSystem(&RenderSystem);
			Camera.Camera = RenderSystem.getCamera();
		}
		virtual ICamera * API_CALL getCamera()
		{
			return &Camera;
		}


		virtual void API_CALL setUnpdateBounds(float minX, float minY, float maxX, float maxY)
		{
			Filter.setUpdateAabb(geometry::Aabb2d(minX, minY, maxX, maxY));
		}

		virtual void API_CALL setRenderBounds(float minX, float minY, float maxX, float maxY)
		{
			Filter.setRenderAabb(geometry::Aabb2d(minX, minY, maxX, maxY));
		}

		virtual IEntity * API_CALL createEntity()
		{
			return EntitiesWorld.createEntity();
		}

		virtual int API_CALL update(float elapsed)
		{
			EntitiesWorld.updateWorld(elapsed);
			EntitiesWorld.renderWorld(elapsed);
			return RenderSystem.getDrawCalls();
		}

		virtual void API_CALL updateEntity(IEntity *entity, ComponentsTypes types)
		{
			EntitiesWorld.updateComponent((entities::Entity *)entity->getSelfInstance(), (entities::ComponentMask::e)types);
		}

		virtual void API_CALL addEntity(IEntity *entity)
		{
			EntitiesWorld.addEntity((entities::Entity *)entity->getSelfInstance());
		}
		
		virtual void API_CALL removeEntity(IEntity *entity)
		{
			EntitiesWorld.removeEntity((entities::Entity *)entity->getSelfInstance());
		}

		virtual void API_CALL clear()
		{
			EntitiesWorld.clear();
		}


		virtual void API_CALL dispose()
		{
			delete this;
		}
	};

	class EntitiesFactory : public IEntitiesFactory
	{
	public:
		virtual IEntitiesWorld * API_CALL createEntitiesWorld()
		{
			return new CEntitiesWorld();
		}
		virtual void API_CALL dispose() 
		{
			delete this;
		}
	};

	extern "C" DLLEXPORT  IEntitiesFactory * API_CALL createEntitiesFactory()
	{
		return new EntitiesFactory();
	}
}