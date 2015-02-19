#ifndef ENTITIY_H
#define ENTITIY_H

#include "entities_classes.h"
#include "ComponentsFactory.h"

namespace entities
{	
	class Entity
	{
	public:		
		Entity()
		{
			_staticComponentsMask = 0;
		}

		void setComponent(StaticComponentsTypes::e type, int id)
		{
			_componentsIds.data[type] = id;
		}
		
		int getComponent(StaticComponentsTypes::e type)
		{
			return _componentsIds.data[type];
		}

		template<typename T>
		T * getComponent()
		{
			return 0;
		}

		inline unsigned int getStaticComponentsMask(){ return 0; }
		
		template<typename T>
		void createComponent()
		{
			_componentsIds.data[T::Type] = T::Pool.create();
			_staticComponentsMask |= T::Mask;
		}

		template<typename T>
		inline static int create()
		{
			int result = Pool.create();
			Entity * instance = Pool.get(result);
			instance->_staticComponentsMask = T::Mask;
			instance->createComponent<T>()
		}

		template<typename T0, typename T1>
		inline static int create()
		{
			int result = Pool.create();
			Entity * instance = Pool.get(result);
			instance->createComponent<T0>();
			instance->createComponent<T1>();
		}

		template<typename T0, typename T1, typename T2>
		inline static int create()
		{
			int result = Pool.create();
			Entity * instance = Pool.get(result);
			instance->createComponent<T0>();
			instance->createComponent<T1>();
			instance->createComponent<T2>();
		}

		template<typename T0, typename T1, typename T2, typename T3>
		inline static int create()
		{
			int result = Pool.create();
			Entity * instance = Pool.get(result);
			instance->createComponent<T0>();
			instance->createComponent<T1>();
			instance->createComponent<T2>();
			instance->createComponent<T3>();
		}

		template<typename T0, typename T1, typename T2, typename T3, typename T4>
		inline static int create()
		{
			int result = Pool.create();
			Entity * instance = Pool.get(result);
			instance->createComponent<T0>();
			instance->createComponent<T1>();
			instance->createComponent<T2>();
			instance->createComponent<T3>();
			instance->createComponent<T4>();
		}
	protected:
	private:
		int _staticComponentsMask;
		core::StaticArray<int, StaticComponentsTypes::ComponentsCount> _componentsIds;
	};
}

#endif /*ENTITIY_H*/

