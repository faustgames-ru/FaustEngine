#include "Camera.h"

namespace drawing
{
	Camera::Camera() : _position(0, 0, 0), _aspect(1.0f), _rotationZ(0), _zn(0.1f), _zf(1.0f)
	{
	}

	void Camera::setPosition(float x, float y, float z)
	{
		_position.setData(x, y, z);
	}
	
	void Camera::setFov(float fov)
	{
		_fov = fov;
	}
	
	void Camera::setAspect(float aspect)
	{
		_aspect = aspect;
	}

	void Camera::setAspect(float w, float h)
	{
		_aspect = h / w;
	}


	void Camera::setRotation(float rotationZ)
	{
		_rotationZ = rotationZ;
	}

	void Camera::setPlanes(float zn, float zf)
	{
		_zn = zn;
		_zf = zf;
	}

	
	core::Matrix Camera::getMatrix()
	{
		core::Matrix proj = getProjectionMatrix();
		core::Matrix t = getTranslationMatrix();
		return core::Matrix::mul(t, proj);
	}

	core::Vector3 Camera::getPosition()
	{
		return _position;
	}


	core::Matrix Camera::getProjectionMatrix()
	{
		return core::Matrix::createProjection(_fov, _aspect, _zn, _zf);
	}

	core::Matrix Camera::getTranslationMatrix()
	{
		return core::Matrix::createTranslate(-_position.getX(), -_position.getY(), -_position.getZ());
	}


}