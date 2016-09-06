#ifndef ZOMBO_BOUNDS_H
#define ZOMBO_BOUNDS_H

#include "../zombo_classes.h"

namespace zombo
{
	class ZomboBounds
	{
	public:
		core::Vector3 position;
		core::Vector2 size;

		static ZomboComponentTypes::e ComponentType;
		static ZomboBounds* create();

		ZomboBounds();
		~ZomboBounds();
		void dispose() const;
	};
}

#endif /* ZOMBO_BOUNDS_H */