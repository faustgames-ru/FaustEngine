#ifndef ZOMBO_ENTITY_H
#define ZOMBO_ENTITY_H

#include "zombo_classes.h"

namespace zombo
{
	class ZomboEntity
	{
	public:		
		ZomboEntity();
		~ZomboEntity();
		
		template <typename TComponent>
		void createComponent();		
		
		template <typename TComponent>
		void disposeComponent();

		template <typename TComponent>
		TComponent* getComponent();
	private:
		void *_components[ZomboComponentTypes::Count];
	};

	template <typename TComponent>
	void ZomboEntity::createComponent()
	{
		TComponent *component = TComponent::create();
		_components[TComponent::ComponentType] = component;
	}

	template <typename TComponent>
	void ZomboEntity::disposeComponent()
	{
		TComponent *component = static_cast<TComponent *>(_components[TComponent::ComponentType]);
		if (component != nullptr)
		{
			component->dispose();
		}
	}

	template <typename TComponent>
	TComponent* ZomboEntity::getComponent()
	{
		return static_cast<TComponent*>(_components[TComponent::ComponentType]);
	}
}

#endif /* ZOMBO_ENTITY_H */
