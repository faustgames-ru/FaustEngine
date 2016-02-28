#include "ZomboEditorCamera.h"
#include "ZomboEditorViewport.h"
#include "../common/ZomboGameEnvironment.h"
#include "ZomboEditorInput.h"
#include "camera/ZomboCameraMoveXY.h"
#include "camera/ZomboCameraRotate.h"
#include "commands/ZomboEditorCommands.h"
#include "../ZomboLog.h"
#include "../common/ValuesAnimator.h"

namespace zombo
{
	ZomboEditorCamera ZomboEditorCamera::Default;

	ZomboCommandCameraSetMode::ZomboCommandCameraSetMode(const std::string& mode)
	{
		targetMode = mode;
	}

	bool ZomboCommandCameraSetMode::isExecutionAvaliable()
	{
		return ZomboEditorCamera::Default.getModeInternal() != targetMode;
	}

	bool ZomboCommandCameraSetMode::isUndoAvaliable()
	{
		return ZomboEditorCamera::Default.getModeInternal() != originMode;
	}

	void ZomboCommandCameraSetMode::execute()
	{
		ZomboLog::Default.m("Camera mode: "+ targetMode);
		_prevRotation = ZomboEditorCamera::Default.rotator.actualRotation;
		originMode = ZomboEditorCamera::Default.getModeInternal();
		ZomboEditorCamera::Default.setModeInternal(targetMode.c_str());
	}

	void ZomboCommandCameraSetMode::undo()
	{
		ZomboLog::Default.m("Camera mode: " + originMode);
		ZomboEditorCamera::Default.rotator.setRotationAnimated(_prevRotation);
		ZomboEditorCamera::Default.setModeInternal(originMode.c_str());
		originMode = "";
	}

	ZomboCommandCameraSetFov::ZomboCommandCameraSetFov(float fov): _fov(fov), _prevFov(0)
	{
	}

	bool ZomboCommandCameraSetFov::isExecutionAvaliable()
	{
		return !core::Math::equals(ZomboEditorCamera::Default.getFov(), _fov);
	}

	bool ZomboCommandCameraSetFov::isUndoAvaliable()
	{
		return !core::Math::equals(ZomboEditorCamera::Default.getFov(), _prevFov);
	}

	void ZomboCommandCameraSetFov::execute()
	{
		ZomboLog::Default.m("Do: Change fov");
		_prevFov = ZomboEditorCamera::Default.getFov();
		ZomboEditorCamera::Default.setFovInternal(_fov);
	}

	void ZomboCommandCameraSetFov::undo()
	{
		ZomboLog::Default.m("Undo: Change fov");
		ZomboEditorCamera::Default.setFovInternal(_prevFov);
	}

	ZomboCommandCameraSetScale::ZomboCommandCameraSetScale(float prevScale, float scale): _scale(scale), _prevscale(prevScale)
	{		
	}

	bool ZomboCommandCameraSetScale::isExecutionAvaliable()
	{
		return !core::Math::equals(_prevscale, _scale);
	}

	bool ZomboCommandCameraSetScale::isUndoAvaliable()
	{
		return !core::Math::equals(_scale, _prevscale);
	}

	void ZomboCommandCameraSetScale::execute()
	{
		//_prevscale = ZomboEditorCamera::Default.getScale();
		if (_prevscale > _scale)
		{
			ZomboLog::Default.m("Do: Zoom in");
		}
		else
		{
			ZomboLog::Default.m("Do: Zoom out");
		}
		ZomboEditorCamera::Default.setScaleInternal(_scale);
	}

	void ZomboCommandCameraSetScale::undo()
	{
		if (_prevscale > _scale)
		{
			ZomboLog::Default.m("Undo: Zoom in");
		}
		else
		{
			ZomboLog::Default.m("Undo: Zoom out");
		}
		ZomboEditorCamera::Default.setScaleInternal(_prevscale);
	}

	void ZomboCommandCameraSetScale::invalidate(float value)
	{
		_scale = value;
	}

	ZomboCameraVelocityEntry::ZomboCameraVelocityEntry(ulong t, core::Vector2 v)
	{
		time = t;
		velocity = v;
	}

	ZomboCameraVelocityStack::ZomboCameraVelocityStack() : _resistance(6.0f)
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
			while ((_velocities.front().time < lowTime) || (_velocities.size() > 100))
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
			_velocity = velocity / static_cast<float>(vc);
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

	ZomboEditorCameraRotator::ZomboEditorCameraRotator()
	{
		originRotation = targetRotation = actualRotation = core::Matrix::identity;
	}

	void ZomboEditorCameraRotator::setRotation(const core::Matrix& matrix)
	{
		originRotation = targetRotation = actualRotation = matrix;
	}


	void ZomboEditorCameraRotator::ternimateInterpolator()
	{
		setRotation(actualRotation);
		_angle.setAll(0);
	}

	void ZomboEditorCameraRotator::rotate(core::Matrix originMatrix, core::Vector3 n, float a, core::Matrix targetMatrix)
	{
		if (core::Matrix::equals(targetRotation, targetMatrix))
			return;
		originRotation = originMatrix;
		targetRotation = targetMatrix;
		_normal = n;
		_angle.set(0);
		_angle.setTarget(a);
	}

	void ZomboEditorCameraRotator::setRotationAnimated(const core::Matrix& matrix)
	{
		float p, h, b;
		matrix.getRotation(p, h, b);
		originRotation = actualRotation;
		targetRotation = matrix;
		_pAngle.setTarget(p);
		_hAngle.setTarget(h);
		_bAngle.setTarget(b);
	}
	void ZomboEditorCameraRotator::updateEuler()
	{		
		float p, h, b;
		actualRotation.getRotation(p, h, b);
		_pAngle.setAll(p);
		_hAngle.setAll(h);
		_bAngle.setAll(b);
	}

	void ZomboEditorCameraRotator::update()
	{
		_angle.update();
		_pAngle.update();
		_hAngle.update();
		_bAngle.update();
		if (_angle.isUpdating())
		{
			actualRotation = core::Matrix::mul(originRotation, core::Matrix::createRotation(_normal, _angle.get()));
			updateEuler();
		}
		else if (_pAngle.isUpdating() || _hAngle.isUpdating() || _bAngle.isUpdating())
		{
			actualRotation = core::Matrix::createEuler(_pAngle.get(), _hAngle.get(), _bAngle.get());
		}
		else
		{
			actualRotation = targetRotation;
			updateEuler();
		}
	}

	ZomboEditorCamera::ZomboEditorCamera() : depth(5000.0f), _scaleValue(1.0f), _fovValue(core::Math::Pi * 40.0f / 180.0f), _lastZoomCommand(nullptr)
	{
		mode = &ZomboCameraMoveXY::Default;
		_actualModeName = ZomboCameraMoveXY::ModeName;
	}

	ZomboEditorCamera::~ZomboEditorCamera()
	{
	}

	void ZomboEditorCamera::updateInterpoaltedScale()
	{	
		//_scaleValue.update();
	}

	void ZomboEditorCamera::update()
	{
		mode->updateInput();
		rotator.update();
		_fovValue.update();
		_position.update();
		updateInterpoaltedScale();
		core::Vector2 pos = _position.get();
		core::Matrix translate = core::Matrix::createTranslate(-pos.getX(), -pos.getY(), 0);
		float minFov = core::Math::atan2(1.0f, depth*0.5f) * 2.0f;
		float fov = _fovValue.get();
		if (fov < minFov)
		{
			core::Matrix ortho = core::Matrix::createOrtho(ZomboEditorViewport::Default.getAspect(), _scaleValue, depth*0.5f);
			core::Matrix orthoSkybox = core::Matrix::createOrtho(ZomboEditorViewport::Default.getAspect(), 1.0f, depth*0.5f);
			skyboxMatrix.setValue(orthoSkybox);
			transformToView = core::Matrix::mul(translate, rotator.actualRotation, ortho);
			matrix.setValue(transformToView);
		}
		else
		{
			core::Matrix proj = core::Matrix::createProjection(fov, ZomboEditorViewport::Default.getAspect(), 0.01f, depth);
			float z = 1.0f / core::Math::tan(fov * 0.5f);
			core::Matrix translateZ = core::Matrix::createTranslate(0, 0, z);
			float scaleValue = _scaleValue;// .get();
			core::Matrix scale = core::Matrix::createScale(1.0f / scaleValue, 1.0f / scaleValue, 1.0f / scaleValue);
			transformToView = core::Matrix::mul(translate, rotator.actualRotation, scale, translateZ, proj);
			matrix.setValue(transformToView);
			skyboxMatrix.setValue(core::Matrix::mul(rotator.actualRotation, proj));
		}
		frustum = geometry::Frustum(transformToView);
		transformFromView = transformToView.inverse();
	}

	core::Vector2 ZomboEditorCamera::getMouseProjection(float z) const
	{
		core::Vector2 m;
		m.setX(-1.0f + ZomboEditorInput::Default.mouse()->position.getX() * 2.0f / ZomboEditorViewport::Default.w);
		m.setY(1.0f - ZomboEditorInput::Default.mouse()->position.getY() * 2.0f / ZomboEditorViewport::Default.h);

		core::Vector3 zero = core::Matrix::transform(transformToView, core::Vector3(0, 0, z));
		return core::Matrix::transform(transformFromView, m.toVector3(zero.getZ())).toVector2();
	}

	IntPtr ZomboEditorCamera::getMode()
	{
		char *str = const_cast<char *>(_actualModeName.c_str());
		return str;

	}

	void ZomboEditorCamera::setMode(String modeName)
	{
		ZomboEditorCommand *command = new ZomboCommandCameraSetMode(modeName);
		if (ZomboEditorCommands::camera()->doCommand(command) == CommandExecutonStatus::CommandExecutionNotAvaliable)
		{
			delete command;
		}
	}

	void ZomboEditorCamera::setScale(float value)
	{		
		if (Animators::Float.animating(&_scaleValue) && _lastZoomCommand != nullptr)
		{
			_lastZoomCommand->invalidate(value);
			setScaleInternal(value);
		}
		else
		{
			_lastZoomCommand = new ZomboCommandCameraSetScale(_scaleValue, value);
			if (ZomboEditorCommands::camera()->doCommand(_lastZoomCommand) != CommandExecutonStatus::CommandExecuted)
			{
				delete _lastZoomCommand;
				_lastZoomCommand = nullptr;
			}
		}
	}

	void ZomboEditorCamera::setFov(float value)
	{
		ZomboCommandCameraSetFov* command = new ZomboCommandCameraSetFov(value);
		if (ZomboEditorCommands::camera()->doCommand(command) != CommandExecutonStatus::CommandExecuted)
		{
			delete command;
		}
	}

	float ZomboEditorCamera::getScale()
	{
		return Animators::Float.getTarget(&_scaleValue);
	}

	float ZomboEditorCamera::getFov()
	{
		return _fovValue.getTarget();
	}

	bool ZomboEditorCamera::isUndoAvaliable()
	{
		return ZomboEditorCommands::camera()->isUndoAvaliable();
	}

	bool ZomboEditorCamera::isRedoAvaliable()
	{
		return ZomboEditorCommands::camera()->isRedoAvaliable();
	}

	void ZomboEditorCamera::undo()
	{
		ZomboEditorCommands::camera()->undo();
	}

	void ZomboEditorCamera::redo()
	{
		ZomboEditorCommands::camera()->redo();
	}

	void ZomboEditorCamera::setModeInternal(String modeName)
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
		return _scaleValue;
	}

	void ZomboEditorCamera::setPositionXY(const core::Vector2 &v)
	{
		_position.setTarget(v);
	}

	core::Vector2 ZomboEditorCamera::getPositionXY() const
	{
		return _position.getTarget();
	}

	void ZomboEditorCamera::setFovInternal(float value)
	{
		_fovValue.setTarget(value);
	}

	void ZomboEditorCamera::setScaleInternal(float value)
	{
		Animators::Float.animate(&_scaleValue, value);
	}

	std::string ZomboEditorCamera::getModeInternal()
	{
		return _actualModeName;
	}

	void ZomboEditorCamera::addPositionXY(const core::Vector2 &d)
	{
		_position.setAll(_position.get() + d);
	}

	extern "C" DLLEXPORT IZomboEditorCamera* API_CALL getZomboEditorCamera()
	{
		return &ZomboEditorCamera::Default;
	}
}
