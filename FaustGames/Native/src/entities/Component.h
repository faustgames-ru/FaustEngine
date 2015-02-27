#ifndef COMPONENT_H
#define COMPONENT_H

#include "entities_classes.h"

namespace entities
{
	class Component
	{
	public:
		Entity * Owner;
		virtual ~Component(){}
	protected:
	private:
	};
}

#endif /*COMPONENT_H*/