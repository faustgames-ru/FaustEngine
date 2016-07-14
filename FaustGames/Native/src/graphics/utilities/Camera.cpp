#include "Camera.h"

namespace graphics
{
	Camera2d::Camera2d(): fov(core::Math::Pi * 40.0f / 180.0f), scale(1.0f), aspect(16.0f / 9.0f), depth(1000.0f), target(core::Vector3::empty)
	{
	}

	void Camera2d::update()
	{
		float minFov = core::Math::atan2(1.0f, depth*0.5f) * 2.0f;
		core::Matrix translate = core::Matrix::createTranslate(-target.getX(), -target.getY(), target.getZ());
		core::Matrix rotation = core::Matrix::identity;
		if (fov < minFov)
		{
			core::Matrix ortho = core::Matrix::createOrtho(aspect, scale, depth*0.5f);
			core::Matrix transformToView = core::Matrix::mul(translate, rotation, ortho);
			projection.setValue(transformToView);
		}
		else
		{
			core::Matrix proj = core::Matrix::createProjection(fov, aspect, 0.01f, depth);
			float z = 1.0f / core::Math::tan(fov * 0.5f);
			core::Matrix translateZ = core::Matrix::createTranslate(0, 0, z);
			core::Matrix scaleM = core::Matrix::createScale(1.0f / scale, 1.0f / scale, 1.0f / scale);
			core::Matrix transformToView = core::Matrix::mul(translate, rotation, scaleM, translateZ, proj);
			projection.setValue(transformToView);
		}
	}
}
