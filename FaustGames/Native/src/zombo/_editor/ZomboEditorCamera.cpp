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
		mode = &ZomboCameraMoveXY::Default;
		_actualModeName = ZomboCameraMoveXY::ModeName;
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
		if (mode == &ZomboCameraMoveXY::Default)
		{
			position.setX(position.getX() - dx);
			position.setY(position.getY() - dy);
		}
	}

	void ZomboEditorCamera::update()
	{
		mode->updateInput();
		
		updateInterpoaltedScale();
		core::Matrix translate = core::Matrix::createTranslate(-position.getX(), -position.getY(), 0);
		float minFov = core::Math::atan2(1.0f, depth*0.5f) * 2.0f;
		if (fov < minFov)
		{
			core::Matrix ortho = core::Matrix::createOrtho(ZomboEditorViewport::Default.getAspect(), _interpoaltedScale, depth*0.5f);
			matrix.setValue(core::Matrix::mul(translate, rotation, ortho));
		}
		else
		{
			core::Matrix proj = core::Matrix::createProjection(fov, ZomboEditorViewport::Default.getAspect(), 0.01f, depth);
			float z = 1.0f / core::Math::tan(fov * 0.5f);
			core::Matrix translateZ = core::Matrix::createTranslate(0, 0, z);
			core::Matrix scale = core::Matrix::createScale(1.0f / _interpoaltedScale, 1.0f / _interpoaltedScale, 1.0f / _interpoaltedScale);
			matrix.setValue(core::Matrix::mul(translate, rotation, scale, translateZ, proj));
		}
	}

	IntPtr ZomboEditorCamera::getMode()
	{
		char *str = const_cast<char *>(_actualModeName.c_str());
		return str;

	}

	void ZomboEditorCamera::setMode(String modeName)
	{
		setEditorModeInternal(modeName);
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

	void ZomboEditorCamera::setEditorModeInternal(String modeName)
	{
		_actualModeName = modeName;
		if (_actualModeName == ZomboCameraMoveXY::ModeName)
		{
			mode = &ZomboCameraMoveXY::Default;
		}
		else if (_actualModeName == ZomboCameraRotate::ModeName)
		{
			mode = &ZomboCameraRotate::Default;
		}
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
