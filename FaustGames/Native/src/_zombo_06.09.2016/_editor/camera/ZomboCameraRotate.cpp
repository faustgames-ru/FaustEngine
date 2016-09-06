#include "ZomboCameraRotate.h"
#include "../ZomboEditorInput.h"
#include "../ZomboEditorViewport.h"
#include "../../common/ZomboGameEnvironment.h"
#include "../commands/ZomboEditorCommands.h"
#include "../../ZomboLog.h"

namespace zombo
{
	ZomboCameraRotate ZomboCameraRotate::Default;
	std::string ZomboCameraRotate::ModeName("Rotate");


	ZomboCommandCameraRotate::ZomboCommandCameraRotate(core::Vector3 n, float a, const core::Matrix& prev, const core::Matrix& target)
	{
		_normal = n;
		_angle = a;
		_prev = prev;
		_target = target;
	}

	void ZomboCommandCameraRotate::invalidate(core::Vector3 n, float a, const core::Matrix& target)
	{
		_normal = n;
		_angle = a;
		_target = target;
	}

	bool ZomboCommandCameraRotate::isExecutionAvaliable()
	{
		return true;
	}

	bool ZomboCommandCameraRotate::isUndoAvaliable()
	{
		return true;
	}

	void ZomboCommandCameraRotate::execute()
	{
		ZomboLog::Default.m("Do: Rotate camera");
		ZomboEditorCamera::Default.rotator.rotate(_prev, _normal, _angle, _target);
	}

	void ZomboCommandCameraRotate::undo()
	{
		ZomboLog::Default.m("Undo: Rotate camera");
		ZomboEditorCamera::Default.rotator.rotate(_target, _normal, -_angle, _prev);
	}

	ZomboCameraRotate::ZomboCameraRotate(): _actualCommand(nullptr), _prevMidButtonState(false), _angle(0)
	{
		_mouseDownPosition = core::Vector2(-1.0f, -1.0f);
	}

	void ZomboCameraRotate::activated()
	{
	}

	void ZomboCameraRotate::updateInput()
	{
		ZomboEditorMouse *mouse = ZomboEditorInput::Default.mouse();
		core::Vector2 position = mouse->position;
		if (mouse->isMiddlePressed())
		{
			if (!_prevMidButtonState)
			{
				_actualCommand = nullptr;
				_n = core::Vector3::empty;
			}
			if ((_mouseDownPosition.getX() < 0) || (_mouseDownPosition.getY() < 0))
			{
				_originRotation = ZomboEditorCamera::Default.rotator.actualRotation;
				_mouseDownPosition = position;
				_lastMouse = position;
			}
			else
			{
				_velocityStack.updateMove(position - _lastMouse);
				_lastMouse = position;
				core::Vector2 d = position - _mouseDownPosition;
				d.inverseY();
				core::Vector2 n = d.rotate90cw();
				core::Vector3 n3 = _n = core::Vector3(n.getX(), n.getY(), 0).normalize();
				float l = n.length();
				if (l > core::Math::Epsilon)
				{
					_angle = -core::Math::Pi * l / ZomboEditorViewport::Default.h;
					ZomboEditorCamera::Default.rotator.setRotation(core::Matrix::mul(_originRotation, core::Matrix::createRotation(n3, _angle)));
				}
				else
				{
					_angle = 0;
					ZomboEditorCamera::Default.rotator.setRotation(_originRotation);
				}				
			}
		}
		else
		{
			if (_prevMidButtonState)
			{
				
				_actualCommand = new ZomboCommandCameraRotate(_n, _angle, _originRotation, core::Matrix::mul(_originRotation, core::Matrix::createRotation(_n, _angle)));
				if (ZomboEditorCommands::camera()->doCommand(_actualCommand) != CommandExecutonStatus::CommandExecuted)
				{
					delete _actualCommand;
					_actualCommand = nullptr;
				}
				
			}

			float ellapsedTime = ZomboGameEnvironment::ellapsedSeconds;
			core::Vector2 v = _velocityStack.updateVelocity();
			v.inverseY();
			core::Vector2 d = v * ellapsedTime;
			core::Vector2 n = d.rotate90cw();
			core::Vector3 n3 = _n;// core::Vector3(n.getX(), n.getY(), 0).normalize();
			float l = n.length();
			if ((l > core::Math::Epsilon) && !_n.isEmpty())
			{
				_angle += -core::Math::Pi * l / ZomboEditorViewport::Default.h;
				core::Matrix resultRotation = core::Matrix::mul(_originRotation, core::Matrix::createRotation(n3, _angle));
				ZomboEditorCamera::Default.rotator.setRotation(resultRotation);
				if (_actualCommand != nullptr)
				{
					_actualCommand->invalidate(n3, _angle, resultRotation);
				}
			}
			_lastMouse = _mouseDownPosition = core::Vector2(-1.0f, -1.0f);
		}
		_prevMidButtonState = mouse->isMiddlePressed();
	}

}
