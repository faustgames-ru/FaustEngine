#include "ZomboEditorCamera.h"
#include "ZomboEditorViewport.h"
#include "../common/ZomboGameEnvironment.h"
#include "ZomboEditorInput.h"
#include "camera/ZomboCameraMoveXY.h"
#include "camera/ZomboCameraRotate.h"

namespace zombo
{
	ZomboEditorCamera ZomboEditorCamera::Default;

	ZomboCameraVelocityEntry::ZomboCameraVelocityEntry(ulong t, core::Vector2 v)
	{
		time = t;
		velocity = v;
	}

	ZomboCameraVelocityStack::ZomboCameraVelocityStack() : _resistance(4.0f)
	{
	}

	void ZomboCameraVelocityStack::updateMove(const core::Vector2& v)
	{
		ulong time = ZomboGameEnvironment::gameTime;
		float ellapsedTime = ZomboGameEnvironment::ellapsedSeconds;
		_velocities.push_back(ZomboCameraVelocityEntry(time, v / ellapsedTime));
		ulong d = ZomboGameEnvironment::getTime(0.1f);
		if (time > d)
		{
			ulong lowTime = time - d;
			while ((_velocities.front().time < lowTime) || (_velocities.size() > 500))
			{
				_velocities.pop_front();
				if (_velocities.empty()) break;
			}
		}
		core::Vector2 velocity = core::Vector2::empty;
		int vc = 0;
		for (Velocities::iterator i = _velocities.begin(); i != _velocities.end(); ++i)
		{
			velocity += i->velocity;
			vc++;
		}
		if (vc > 0)
		{
			_velocity = velocity / vc;
			_velocityOrigin = _velocity;
		}
		else
		{
			_velocity = _velocityOrigin = core::Vector2::empty;
		}
	}

	core::Vector2 ZomboCameraVelocityStack::updateVelocity()
	{
		_velocities.clear();
		float ellapsedTime = ZomboGameEnvironment::ellapsedSeconds;
		_velocity -= _velocityOrigin*(_resistance*ellapsedTime);
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
		return _velocity;
	}
	ZomboEditorCamera::ZomboEditorCamera() : depth(5000.0f), _scaleValue(1.0f), _fovValue(core::Math::Pi * 40.0f / 180.0f)
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
		/*
		float x = (ZomboEditorInput::Default.mouse.position.getX() - ZomboEditorViewport::Default.w * 0.5f) * _scaleValue.getValue() / ZomboEditorViewport::Default.h;
		float y = (-ZomboEditorInput::Default.mouse.position.getY() + ZomboEditorViewport::Default.h * 0.5f) * _scaleValue.getValue() / ZomboEditorViewport::Default.h;
		*/

		_scaleValue.update();
		/*
		float nx = (ZomboEditorInput::Default.mouse.position.getX() - ZomboEditorViewport::Default.w * 0.5f) * _scaleValue.getValue() / ZomboEditorViewport::Default.h;
		float ny = (-ZomboEditorInput::Default.mouse.position.getY() + ZomboEditorViewport::Default.h * 0.5f) * _scaleValue.getValue() / ZomboEditorViewport::Default.h;
		
		float dx = nx - x;
		float dy = ny - y;
		
		if (mode == &ZomboCameraMoveXY::Default)
		{
			_positionX.setAllValues(_positionX.getValue() - dx);
			_positionY.setAllValues(_positionY.getValue() - dy);
		}
		*/
	}

	void ZomboEditorCamera::update()
	{
		mode->updateInput();
		_fovValue.update();
		_positionX.update();
		_positionY.update();
		updateInterpoaltedScale();
		core::Matrix translate = core::Matrix::createTranslate(-_positionX.getValue(), -_positionY.getValue(), 0);
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
		_positionX.setTargetValue(x);
	}

	void ZomboEditorCamera::setPositionY(float y)
	{
		_positionY.setTargetValue(y);
	}

	float ZomboEditorCamera::getPositionX() const
	{
		return _positionX.getTargetValue();
	}

	float ZomboEditorCamera::getPositionY() const
	{
		return _positionY.getTargetValue();
	}

	void ZomboEditorCamera::setPositionXY(const core::Vector2 &v)
	{
		_positionX.setTargetValue(v.getX());
		_positionY.setTargetValue(v.getY());
	}

	core::Vector2 ZomboEditorCamera::getPositionXY() const
	{
		return core::Vector2(_positionX.getTargetValue(), _positionY.getTargetValue());
	}

	void ZomboEditorCamera::addPositionXY(const core::Vector2 &d)
	{
		_positionX.setAllValues(_positionX.getValue() + d.getX());
		_positionY.setAllValues(_positionY.getValue() + d.getY());
	}

	extern "C" DLLEXPORT IZomboEditorCamera* API_CALL getZomboEditorCamera()
	{
		return &ZomboEditorCamera::Default;
	}
}
