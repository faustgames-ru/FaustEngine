#include "ZomboEditorCamera.h"
#include "ZomboEditorViewport.h"
#include "../common/ZomboGameEnvironment.h"
#include "ZomboEditorInput.h"

namespace zombo
{
	ZomboEditorCamera ZomboEditorCamera::Default;

	ZomboEditorCamera::ZomboEditorCamera() : scale(1.0f), fov(0.0f), depth(5000.0f), _interpoaltedScale(1.0), _scaleVelocity(0), _lastMouseX(0), _lastMouseY(0), _velocityX(0), _velocityY(0), _velocityOriginX(0), _velocityOriginY(0)
	{
	}

	ZomboEditorCamera::~ZomboEditorCamera()
	{
	}

	void ZomboEditorCamera::updateInterpoaltedScale()
	{
		float newInterpoaltedScale;
		float x = (ZomboEditorInput::Default.mouse.x - ZomboEditorViewport::Default.w * 0.5f) * _interpoaltedScale / ZomboEditorViewport::Default.h;
		float y = (-ZomboEditorInput::Default.mouse.y + ZomboEditorViewport::Default.h * 0.5f) * _interpoaltedScale / ZomboEditorViewport::Default.h;
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

		float nx = (ZomboEditorInput::Default.mouse.x - ZomboEditorViewport::Default.w * 0.5f) * _interpoaltedScale / ZomboEditorViewport::Default.h;
		float ny = (-ZomboEditorInput::Default.mouse.y + ZomboEditorViewport::Default.h * 0.5f) * _interpoaltedScale / ZomboEditorViewport::Default.h;
		float dx = nx - x;
		float dy = ny - y;
		position.setX(position.getX() - dx);
		position.setY(position.getY() - dy);
	}

	void ZomboEditorCamera::updateInput()
	{
		float ellapsedTime = ZomboGameEnvironment::ellapsedSeconds;
		if ((_velocityX *_velocityOriginX) < 0)
			_velocityX = 0;
		if ((_velocityY *_velocityOriginY) < 0)
			_velocityY = 0;
		if (ZomboEditorInput::Default.mouse.isMiddlePressed())
		{
			if (_lastMouseX > 0)
			{
				float dx = (ZomboEditorInput::Default.mouse.x - _lastMouseX) * _interpoaltedScale / ZomboEditorViewport::Default.h;
				_velocityOriginX = _velocityX = -dx / ellapsedTime;
				position.setX(position.getX() - dx);
			}
			if (_lastMouseY > 0)
			{
				float dy = (ZomboEditorInput::Default.mouse.y - _lastMouseY) * _interpoaltedScale / ZomboEditorViewport::Default.h;
				_velocityOriginY = _velocityY = dy / ellapsedTime;
				position.setY(position.getY() + dy);
			}
			_lastMouseX = ZomboEditorInput::Default.mouse.x;
			_lastMouseY = ZomboEditorInput::Default.mouse.y;
		}
		else
		{
			_lastMouseX = -1;
			_lastMouseY = -1;
			position.setX(position.getX() + _velocityX * ellapsedTime);
			position.setY(position.getY() + _velocityY * ellapsedTime);
			_velocityX -= _velocityOriginX*3.0f*ellapsedTime;
			_velocityY -= _velocityOriginY*3.0f*ellapsedTime;
		}
	}

	void ZomboEditorCamera::update()
	{
		updateInput();
		updateInterpoaltedScale();
		core::Matrix translate = core::Matrix::createTranslate(-position.getX(), -position.getY(), -position.getZ());
		if (fov < core::Math::Epsilon)
		{
			core::Matrix ortho = core::Matrix::createOrtho(ZomboEditorViewport::Default.getAspect(), _interpoaltedScale, depth);
			matrix.setValue(core::Matrix::mul(translate, ortho));
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

	extern "C" DLLEXPORT IZomboEditorCamera* API_CALL getZomboEditorCamera()
	{
		return &ZomboEditorCamera::Default;
	}
}
