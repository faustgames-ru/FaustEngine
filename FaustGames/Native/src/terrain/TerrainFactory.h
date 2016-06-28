#ifndef TERRAIN_FACTORY_H
#define TERRAIN_FACTORY_H

#include "terrain_classes.h"

namespace terrain
{
	class TerrainFactory: public llge::ITerrainFactory
	{
	public:
		virtual llge::ITerrainStripeBuilder * API_CALL createStripeBuilder() OVERRIDE;
		virtual void API_CALL dispose() OVERRIDE;
	};
}

#endif /*TERRAIN_FACTORY_H*/
