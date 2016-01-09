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

	ZomboEditorCamera::ZomboEditorCamera() : depth(5000.0f), _scaleValue(1.0f), _fovValue(0.0f)
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
		float x = (ZomboEditorInput::Default.mouse.position.getX() - ZomboEditorViewport::Default.w * 0.5f) * _scaleValue.getValue() / ZomboEditorViewport::Default.h;
		float y = (-ZomboEditorInput::Default.mouse.position.getY() + ZomboEditorViewport::Default.h * 0.5f) * _scaleValue.getValue() / ZomboEditorViewport::Default.h;
		
		_scaleValue.update();
		
		float nx = (ZomboEditorInput::Default.mouse.position.getX() - ZomboEditorViewport::Default.w * 0.5f) * _scaleValue.getValue() / ZomboEditorViewport::Default.h;
		float ny = (-ZomboEditorInput::Default.mouse.position.getY() + ZomboEditorViewport::Default.h * 0.5f) * _scaleValue.getValue() / ZomboEditorViewport::Default.h;
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
		_fovValue.update();
		updateInterpoaltedScale();
		core::Matrix translate = core::Matrix::createTranslate(-position.getX(), -position.getY(), 0);
		float minFov = core::Math::atan2(1.0f, depth*0.5f) * 2.0f;
		float fov = _fovValue.getValue();
		if (fov < minFov)
		{
			core::Matrix ortho = core::Matrix::createOrtho(ZomboEditorViewport::Default.getAspect(), _scaleValue.getValue(), depth*0.5f);
			matrix.setValue(core::Matrix::mul(translate, rotation, ortho));
		}
		else
		{
			core::Matrix proj = core::Matrix::createProjection(fov, ZomboEditorViewport::Default.getAspect(), 0.01f, depth);
			float z = 1.0f / core::Math::tan(fov * 0.5f);
			core::Matrix translateZ = core::Matrix::createTranslate(0, 0, z);
			float scaleValue = _scaleValue.getValue();
			core::Matrix scale = core::Matrix::createScale(1.0f / scaleValue, 1.0f / scaleValue, 1.0f / scaleValue);
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
		_scaleValue.setTargetValue(value);
	}

	void ZomboEditorCamera::setFov(float value)
	{
		_fovValue.setTargetValue(value);
	}

	float ZomboEditorCamera::getScale()
	{
		return _scaleValue.getTargetValue();
	}

	float ZomboEditorCamera::getFov()
	{
		return _fovValue.getTargetValue();
	}

	void ZomboEditorCamera::setEditorModeInternal(String modeName)
	{
		_actualModeName = modeName;
		if (_actualModeName == ZomboCameraMoveXY::ModeName)
		{
			if (mode != &ZomboCameraMoveXY::Default)
			{
				mode = &ZomboCameraMoveXY::Default;
				mode->activated();
			}
		}
		else if (_actualModeName == ZomboCameraRotate::ModeName)
		{
			if (mode != &ZomboCameraRotate::Default)
			{
				mode = &ZomboCameraRotate::Default;
				mode->activated();
			}
		}
	}

	float ZomboEditorCamera::getInterpoaltedScale() const
	{
		return _scaleValue.getValue();
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
