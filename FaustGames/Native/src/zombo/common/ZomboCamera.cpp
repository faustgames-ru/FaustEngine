#include "ZomboCamera.h"

namespace zombo
{
	ZomboCamera* ZomboCamera::create()
	{
		return new ZomboCamera();
	}

	ZomboCamera::ZomboCamera(): 
		position(0.0f, 0.0, -10.0f), 
		fov(core::Math::Pi * 0.5f), 
		nearPlane(10), 
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

	ZomboRect ZomboCamera::getFrustumRect(float zLayer)
	{

	}

	bool ZomboCamera::areBoundsVisible(const core::Vector3& position, const core::Vector2& size)
	{

	}
}
