#include "PhysicalConverter.h"

namespace physics
{
	PhysicalConverter::PhysicalConverter()
	{
		_scale = 1.0f;
		_unScale = 1.0f;
	}

	PhysicalConverter::~PhysicalConverter()
	{
	}

	float PhysicalConverter::toWorld(float value)
	{
		return value * _scale;
	}

	float PhysicalConverter::fromWorld(float value)
	{
		return value * _unScale;
	}

	void PhysicalConverter::setup(float scale)
	{
		_scale = scale;
		_unScale = 1.0f / scale;
	}
}