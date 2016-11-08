#include "PhysicalJoint.h"
#include "PhysicalConverter.h"

namespace physics
{
	PhysicalJoint::PhysicalJoint(b2Joint* jointInstance, PhysicalConverter dimensionsValue): index(-1), dimensions(dimensionsValue), joint(jointInstance)
	{
	}

	PhysicalJoint::~PhysicalJoint()
	{
	}

	PhysicalFixedJoint::PhysicalFixedJoint(b2MouseJoint * jointInstance, PhysicalConverter dimensions) : PhysicalJoint(jointInstance, dimensions), _joint(jointInstance)
	{
	}

	PhysicalFixedJoint::~PhysicalFixedJoint()
	{
	}

	void PhysicalFixedJoint::setTarget(float x, float y)
	{
		_joint->SetTarget(b2Vec2(dimensions.toWorld(x), dimensions.toWorld(y)));
	}

	void PhysicalFixedJoint::setMaxForce(float value)
	{
		_joint->SetMaxForce(value);
	}

	void PhysicalFixedJoint::setFrequency(float value)
	{
		_joint->SetFrequency(value);
	}

	void PhysicalFixedJoint::setDampingRatio(float value)
	{
		_joint->SetDampingRatio(value);
	}

	IntPtr PhysicalFixedJoint::getNativeInstance()
	{
		return this;
	}

	void PhysicalFixedJoint::setJointTarget(float x, float y)
	{
		setTarget(x, y);
	}

	void PhysicalFixedJoint::setJointMaxForce(float value)
	{
		setMaxForce(value);
	}

	void PhysicalFixedJoint::setJointFrequency(float value)
	{
		setFrequency(value);
	}

	void PhysicalFixedJoint::setJointDampingRatio(float value)
	{
		setDampingRatio(value);
	}
}