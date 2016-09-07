#include "ZomboCamera.h"

namespace zombo
{	
	ZomboCamera* ZomboCamera::create()
	{
		return new ZomboCamera();
	}

	ZomboCamera::ZomboCamera() :
		fov(0.0f),
		scale(1.0f), 
		aspect(1.0f),
		nearPlane(1), 
		farPlane(100)
	{
	}

	ZomboCamera::~ZomboCamera()
	{
	}

	void ZomboCamera::dispose() const
	{
		delete this;
	}

	bool ZomboCamera::areBoundsVisible(const core::Vector3& position, const core::Vector2& size)
	{
		return false;
	}
}
