#include "TerrainFactory.h"
#include "StripeBuilder.h"

namespace terrain
{
	llge::ITerrainStripeBuilder* TerrainFactory::createStripeBuilder()
	{
		return new StripeBuilder();
	}

	void TerrainFactory::dispose()
	{
		delete this;
	}
}