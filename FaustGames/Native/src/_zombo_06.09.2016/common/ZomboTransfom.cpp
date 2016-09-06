#include "ZomboTransfom.h"

namespace zombo
{
	ZomboComponentTypes::e ZomboTransfom::ComponentType(ZomboComponentTypes::Transform);


	ZomboTransfom* ZomboTransfom::create()
	{
		return new ZomboTransfom();
	}

	ZomboTransfom::ZomboTransfom()
		: position(core::Vector3::empty), rotationScale(core::Matrix2::identity)
	{
	}

	ZomboTransfom::~ZomboTransfom()
	{
	}

	void ZomboTransfom::dispose() const
	{
		delete this;
	}
}
