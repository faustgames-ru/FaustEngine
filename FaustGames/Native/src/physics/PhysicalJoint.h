#ifndef PHYSICAL_JOINT_H
#define PHYSICAL_JOINT_H

#include "physics_classes.h"
#include "PhysicalConverter.h"

namespace physics
{

	class PhysicalJoint
	{
	public:
		int index;
		PhysicalConverter dimensions;
		b2Joint * joint;
		PhysicalJoint(b2Joint * jointInstance, PhysicalConverter dimensionsValue);
	};
	
	class PhysicalFixedJoint: public PhysicalJoint, public llge::IPhysicalFixedJoint
	{
	public:
		PhysicalFixedJoint(b2MouseJoint * jointInstance, PhysicalConverter dimensions);

		void setTarget(float x, float y);
		void setMaxForce(float value);
		void setFrequency(float value);
		void setDampingRatio(float value);
		
		virtual IntPtr API_CALL getNativeInstance() OVERRIDE;
		virtual void API_CALL setJointTarget(float x, float y) OVERRIDE;
		virtual void API_CALL setJointMaxForce(float value) OVERRIDE;
		virtual void API_CALL setJointFrequency(float value) OVERRIDE;
		virtual void API_CALL setJointDampingRatio(float value) OVERRIDE;
	private:
		b2MouseJoint * _joint;
	};

}

#endif /*PHYSICAL_JOINT_H*/