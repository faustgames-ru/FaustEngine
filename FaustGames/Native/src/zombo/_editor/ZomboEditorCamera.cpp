#include "ZomboEditorCamera.h"
#include "ZomboEditorViewport.h"
#include "../common/ZomboGameEnvironment.h"
#include "ZomboEditorInput.h"

namespace zombo
{
	ZomboEditorCamera ZomboEditorCamera::Default;

	VelocityStackEntry::VelocityStackEntry(float t, core::Vector2 v)
	{
		time = t;
		velocity = v;
	}

	ZomboEditorCamera::ZomboEditorCamera() : scale(1.0f), fov(0.0f), depth(5000.0f), _interpoaltedScale(1.0), _scaleVelocity(0), _midDownTime(0)
	{
		_lastMouse = core::Vector2(-1, -1);
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
		if ((_velocity.getX() *_velocityOrigin.getX()) < 0)
		{
			_velocity.setX(0.0f);
			_velocityOrigin.setX(0.0f);
		}
		if ((_velocity.getY() *_velocityOrigin.getY()) < 0)
		{
			_velocity.setY(0.0f);
			_velocityOrigin.setY(0.0f);
		}
		if (ZomboEditorInput::Default.mouse.isMiddlePressed())
		{
			if ((_lastMouse.getX() >= 0) && (_lastMouse.getY() >= 0))
			{
				_midDownTime += ellapsedTime;
				float dx = (ZomboEditorInput::Default.mouse.x - _lastMouse.getX()) * _interpoaltedScale / ZomboEditorViewport::Default.h;
				float dy = (ZomboEditorInput::Default.mouse.y - _lastMouse.getY()) * _interpoaltedScale / ZomboEditorViewport::Default.h;

				position.setX(position.getX() - dx);
				position.setY(position.getY() + dy);
				velocities.push(VelocityStackEntry(_midDownTime, core::Vector2(-dx / ellapsedTime, dy / ellapsedTime)));
				while ((velocities.front().time < (_midDownTime - 0.1f)) || (velocities.size() > 100))
				{
					velocities.pop();
					if (velocities.empty()) break;
				}
			}
			_lastMouse.setX(ZomboEditorInput::Default.mouse.x);
			_lastMouse.setY(ZomboEditorInput::Default.mouse.y);
		}
		else
		{
			if (!velocities.empty())
			{
				float vx = 0;
				float vy = 0;
				int vc = 0;
				while (!velocities.empty())
				{
					vx += velocities.front().velocity.getX();
					vy += velocities.front().velocity.getY();
					vc++;
					velocities.pop();
				}
				_velocity.setX(vx / vc);
				_velocity.setY(vy / vc);
				_velocityOrigin.setX(vx / vc);
				_velocityOrigin.setY(vy / vc);
			}

			_midDownTime = 0;

			_lastMouse.setX(-1);
			_lastMouse.setY(-1);
			position.setX(position.getX() + _velocity.getX() * ellapsedTime);
			position.setY(position.getY() + _velocity.getY() * ellapsedTime);
			_velocity.setX(_velocity.getX() - _velocityOrigin.getX()*3.0f*ellapsedTime) ;
			_velocity.setY(_velocity.getY() - _velocityOrigin.getY()*3.0f*ellapsedTime);
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
