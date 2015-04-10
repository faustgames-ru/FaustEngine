#ifndef ENTITY_H
#define ENTITY_H

#include "entities_classes.h"
#include "Component.h"
#include "Aabb2dComponent.h"
#include "Render2dComponent.h"
#include "Transform2dComponent.h"
#include "MatrixTransformComponent.h"

namespace entities
{
	class Entity : public llge::IEntity
	{
	public:
		inline Entity(int id)
		{
			_id = id;
			for (int i = 0; i < ComponentType::ComponentsCount; i++)
			{
				_componentsOrigins.data[i] = 0;
				_components.data[i] = 0;
			}
		}

		~Entity()
		{
			for (int i = 0; i < ComponentType::ComponentsCount; i++)
			{
				if (_components.data[i])
				{
					core::Mem::dispose(_components.data[i]);
				}
			}
		}

		void API_CALL dispose()
		{
			core::Mem::dispose(this);
		}

		inline int getId() { return _id; }

		template<typename T>
		inline T* getComponent()
		{
			return (T*)_componentsOrigins.data[(int)T::Type];
		}

		template<typename T>
		inline T* createComponent()
		{
			if (_componentsOrigins.data[(int)T::Type])
				return (T *)_componentsOrigins.data[(int)T::Type];

			T* component = core::Mem::construct<T>();
			_componentsOrigins.data[(int)T::Type] = (void *)component;
			_components.data[(int)T::Type] = (Component *)component;
			if (component)
				component->Owner = this;
			return component;
		}
		
		virtual IntPtr getSelfInstance()
		{
			return (void *)this;
		}

		virtual void setComponents(llge::ComponentsTypes types)
		{
			if (types & llge::Aadd2d)
			{
				createComponent<Aadd2dComponent>();
			}
			if (types & llge::Render2d)
			{
				createComponent<Render2dComponent>();
			}
			if (types & llge::Transform2d)
			{
				createComponent<Transform2dComponent>();
			}
			if (types & llge::MatrixTransform)
			{
				createComponent<MatrixTransformComponent>();
			}
		}
		
		virtual llge::IAabb2d* API_CALL getAabb2d()
		{
			return getComponent<Aadd2dComponent>();
		}
		virtual llge::IRender2d* API_CALL getRender2d()
		{
			return getComponent<Render2dComponent>();
		}
		virtual llge::ITransform2d* API_CALL getTransform2d()
		{
			return getComponent<Transform2dComponent>();
		}
		virtual llge::IMatrixTransform* API_CALL getMatrixTransform()
		{
			return getComponent<MatrixTransformComponent>();
		}
	private:
		int _id;
		core::StaticArray<void *, ComponentType::ComponentsCount> _componentsOrigins;
		core::StaticArray<Component *, ComponentType::ComponentsCount> _components;
	};
}

#endif /*ENTITY_H*/