#ifndef PHYSICAL_FIXTURE_H
#define PHYSICAL_FIXTURE_H

#include "physics_classes.h"

namespace physics
{
	class PhysicalFixture : public llge::IPhysicalFixture
	{
	public:
		int index;
		b2Fixture* fixture;
		PhysicalFixture(b2Fixture* fixtureInstance);
		~PhysicalFixture();

		virtual IntPtr API_CALL getNativeInstance() OVERRIDE;

		virtual float API_CALL getDensity() OVERRIDE;
		virtual float API_CALL getFriction() OVERRIDE;
		virtual float API_CALL getBonce() OVERRIDE;
		virtual bool API_CALL isSensor() OVERRIDE;
		virtual uint API_CALL getCollidesWith() OVERRIDE;
		virtual uint API_CALL getCollisionGroup() OVERRIDE;
		virtual void API_CALL setDensity(float value) OVERRIDE;
		virtual void API_CALL setFriction(float value) OVERRIDE;
		virtual void API_CALL setBonce(float value) OVERRIDE;
		virtual void API_CALL setSensor(bool value) OVERRIDE;
		virtual void API_CALL setCollidesWith(uint value) OVERRIDE;
		virtual void API_CALL setCollisionGroup(uint value) OVERRIDE;
	private:
	};
}

#endif /*PHYSICAL_FIXTURE_H*/