#ifndef ENTITY_H
#define ENTITY_H

#include "entities_classes.h"
#include "Component.h"

namespace entities
{
	class Entity
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
		
		void dispose()
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

	private:
		int _id;
		core::StaticArray<void *, ComponentType::ComponentsCount> _componentsOrigins;
		core::StaticArray<Component *, ComponentType::ComponentsCount> _components;
	};
}

#endif /*ENTITY_H*/