#include "Camera.h"

namespace graphics
{
	Camera2d::Camera2d(): viewport(nullptr), fov(core::Math::Pi * 40.0f / 180.0f), scale(1.0f), aspect(16.0f / 9.0f), depth(1000.0f), pixelSize(0.1f), target(core::Vector3::empty)
	{
	}

	core::Vector3 Camera2d::unproject(core::Vector2 screenPos, float z)
	{
		core::Vector2 p;
		if(viewport != nullptr)
		{
			p = core::Vector2(
				-1.0f + screenPos.getX() * 2.0f / static_cast<float>(viewport->w),
				-1.0f + screenPos.getY() * 2.0f / static_cast<float>(viewport->h));
			p.inverseY();
		}
		else
		{
			p = screenPos;
		}
		return core::Matrix::transform(unTransform, p.toVector3(projectZ(z)));
	}

	core::Vector3 Camera2d::unproject(float windowX, float windowY, float worldZ)
	{
		return unproject(core::Vector2(windowX, windowY), worldZ);
	}

	float Camera2d::unprojectZ(float screenZ)
	{
		return core::Matrix::transformZ(unProjection, screenZ);
	}

	float Camera2d::projectZ(float worldZ)
	{
		return core::Matrix::transformZ(projection.Value, worldZ);
	}

	void Camera2d::update()
	{
		if (viewport != nullptr)
		{
			aspect = viewport->getAspect();
			pixelSize = 2.0f * scale / viewport->h;
		}
		float minFov = core::Math::atan2(1.0f, depth*0.5f) * 2.0f;
		core::Matrix translate = core::Matrix::createTranslate(-target.getX(), -target.getY(), target.getZ());
		core::Matrix rotation = core::Matrix::identity;
		//core::Matrix rotation = core::Matrix::createRotation(core::Vector3::eX, -core::Math::Pi / 4);
		if (fov < minFov)
		{
			core::Matrix ortho = core::Matrix::createOrtho(aspect, scale, depth*0.5f);
			core::Matrix skyboxToView = core::Matrix::mul(rotation, ortho);
			core::Matrix transformToView = core::Matrix::mul(translate, skyboxToView);
			unTransform = transformToView.inverse();
			unProjection = ortho.inverse();
			transform.setValue(transformToView);
			skybox.setValue(skyboxToView);
			projection.setValue(ortho);
		}
		else
		{
			core::Matrix proj = core::Matrix::createProjection(fov, aspect, 0.01f, depth);
			float z = 1.0f / core::Math::tan(fov * 0.5f);
			core::Matrix translateZ = core::Matrix::createTranslate(0, 0, z);
			core::Matrix scaleM = core::Matrix::createScale(1.0f / scale, 1.0f / scale, 1.0f / scale);
			core::Matrix projM = core::Matrix::mul(scaleM, translateZ, proj);
			core::Matrix skyboxToView = core::Matrix::mul(rotation, projM);
			core::Matrix transformToView = core::Matrix::mul(translate, skyboxToView);
			unTransform = transformToView.inverse();
			unProjection = projM.inverse();
			transform.setValue(transformToView);
			skybox.setValue(skyboxToView);
			projection.setValue(projM);
		}
	}
}
