#include "ZomboEditorCamera.h"
#include "ZomboEditorViewport.h"
#include "../common/ZomboGameEnvironment.h"
#include "ZomboEditorInput.h"
#include "camera/ZomboCameraMoveXY.h"
#include "camera/ZomboCameraRotate.h"

namespace zombo
{
	ZomboEditorCamera ZomboEditorCamera::Default;

	VelocityStackEntry::VelocityStackEntry(float t, core::Vector2 v)
	{
		time = t;
		velocity = v;
	}

	ZomboEditorCamera::ZomboEditorCamera() : scale(1.0f), fov(0.0f), depth(5000.0f), _interpoaltedScale(1.0), _scaleVelocity(0)
	{
		rotation = core::Matrix::identity;
		//mode = &ZomboCameraMoveXY::Default;
		mode = &ZomboCameraRotate::Default;
	}

	ZomboEditorCamera::~ZomboEditorCamera()
	{
	}

	void ZomboEditorCamera::updateInterpoaltedScale()
	{
		float newInterpoaltedScale;
		float x = (ZomboEditorInput::Default.mouse.position.getX() - ZomboEditorViewport::Default.w * 0.5f) * _interpoaltedScale / ZomboEditorViewport::Default.h;
		float y = (-ZomboEditorInput::Default.mouse.position.getY() + ZomboEditorViewport::Default.h * 0.5f) * _interpoaltedScale / ZomboEditorViewport::Default.h;
		if (core::Math::abs(_interpoaltedScale - scale) > core::Math::Epsilon)
		{
			_scaleVelocity = 0;
		}
		_scaleVelocity = core::Math::abs(scale - _interpoaltedScale)*10.0f;
		if (_scaleVelocity < 1)
			_scaleVelocity = 1;
		if (_interpoaltedScale < scale)
		{
			newInterpoaltedScale = _interpoaltedScale + ZomboGameEnvironment::ellapsedSeconds * _scaleVelocity;
			if (newInterpoaltedScale > scale)
				newInterpoaltedScale = scale;
		}
		else
		{
			newInterpoaltedScale = _interpoaltedScale - ZomboGameEnvironment::ellapsedSeconds * _scaleVelocity;
			if (newInterpoaltedScale < scale)
				newInterpoaltedScale = scale;
		}

		_interpoaltedScale = newInterpoaltedScale;

		float nx = (ZomboEditorInput::Default.mouse.position.getX() - ZomboEditorViewport::Default.w * 0.5f) * _interpoaltedScale / ZomboEditorViewport::Default.h;
		float ny = (-ZomboEditorInput::Default.mouse.position.getY() + ZomboEditorViewport::Default.h * 0.5f) * _interpoaltedScale / ZomboEditorViewport::Default.h;
		float dx = nx - x;
		float dy = ny - y;
		position.setX(position.getX() - dx);
		position.setY(position.getY() - dy);
	}

	void ZomboEditorCamera::update()
	{
		mode->updateInput();
		
		updateInterpoaltedScale();
		core::Matrix translate = core::Matrix::createTranslate(-position.getX(), -position.getY(), -position.getZ());
		if (fov < core::Math::Epsilon)
		{
			core::Matrix ortho = core::Matrix::createOrtho(ZomboEditorViewport::Default.getAspect(), _interpoaltedScale, depth);
			matrix.setValue(core::Matrix::mul(translate, rotation, ortho));
		}
		else
		{
			// todo: projection matrix support
		}
	}

	void ZomboEditorCamera::setScale(float value)
	{
		scale = value;
	}

	void ZomboEditorCamera::setFov(float value)
	{
		fov = value;
	}

	float ZomboEditorCamera::getScale()
	{
		return scale;
	}

	float ZomboEditorCamera::getFov()
	{
		return fov;
	}

	float ZomboEditorCamera::getInterpoaltedScale() const
	{
		return _interpoaltedScale;
	}

	void ZomboEditorCamera::setPositionX(float x)
	{
		position.setX(x);
	}

	void ZomboEditorCamera::setPositionY(float y)
	{
		position.setY(y);
	}

	float ZomboEditorCamera::getPositionX() const
	{
		return position.getX();
	}

	float ZomboEditorCamera::getPositionY() const
	{
		return position.getY();
	}

	extern "C" DLLEXPORT IZomboEditorCamera* API_CALL getZomboEditorCamera()
	{
		return &ZomboEditorCamera::Default;
	}
}
