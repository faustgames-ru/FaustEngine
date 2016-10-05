#ifndef PHYSICAL_FIXTURE_H
#define PHYSICAL_FIXTURE_H

#include "physics_classes.h"
#include "PhysicalConverter.h"

namespace physics
{

	class PhysicalFixture : public llge::IPhysicalFixture
	{
	public:
		int contactsCounter;
		int index;
		b2Fixture* fixture;
		PhysicalFixture(b2Fixture* fixtureInstance, PhysicalConverter dimensions);
		~PhysicalFixture();
		
		bool polygonOverlap(float x, float y, const core::Vector2* points, uint count);
		virtual bool API_CALL testPolygonOverlap(float x, float y, IntPtr polygon2f, uint count) OVERRIDE;
		virtual bool API_CALL testOverlap(llge::IPhysicalFixture* f) OVERRIDE;
		virtual bool API_CALL testPolygonOverlapEx(float x, float y, IntPtr polygon2f, uint count, IntPtr contactPoint);
		virtual bool API_CALL testOverlapEx(IPhysicalFixture* fixture, IntPtr contactPoint);

		virtual void API_CALL pauseCollisions(uint leaveGroup) OVERRIDE;
		virtual void API_CALL resumeCollisions() OVERRIDE;

		virtual IntPtr API_CALL getNativeInstance() OVERRIDE;
		virtual int API_CALL getContactsCounter() OVERRIDE;
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
		virtual void API_CALL setRaycastGroup(ushort value) OVERRIDE;
		ushort getRaycastGroup();
	private:
		ushort _collisionGroup;
		ushort _raycastGroup;
		PhysicalConverter _dimensions;
	};
}

#endif /*PHYSICAL_FIXTURE_H*/