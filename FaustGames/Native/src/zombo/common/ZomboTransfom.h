#ifndef ZOMBO_TRANSFORM_H
#define ZOMBO_TRANSFORM_H

#include "../zombo_classes.h"

namespace zombo
{
	class ZomboTransfom
	{
	public:
		core::Vector3 position;
		core::Matrix2 rotationScale;

		static ZomboComponentTypes::e ComponentType;
		static ZomboTransfom* create();

		ZomboTransfom();
		~ZomboTransfom();
		void dispose() const;
	};
}

#endif /* ZOMBO_TRANSFORM_H */