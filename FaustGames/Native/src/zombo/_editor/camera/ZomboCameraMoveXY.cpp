#include "ZomboCameraMoveXY.h"
#include "../../common/ZomboGameEnvironment.h"
#include "../ZomboEditorInput.h"
#include "../ZomboEditorViewport.h"
#include "../commands/ZomboEditorCommands.h"
#include "../../ZomboLog.h"

namespace zombo
{
	ZomboCameraMoveXY ZomboCameraMoveXY::Default;
	std::string ZomboCameraMoveXY::ModeName("Move");


	ZomboCommandCameraMoveXY::ZomboCommandCameraMoveXY(const core::Vector2 prevPosition, const core::Vector2 newPosition)
	{
		_targetPosition = newPosition;
		_prevPosition = prevPosition;
	}

	void ZomboCommandCameraMoveXY::invalidate(const core::Vector2 newPosition)
	{
		_targetPosition = newPosition;
	}

	bool ZomboCommandCameraMoveXY::isExecutionAvaliable()
	{
		return !core::Vector2::equals(_prevPosition, _targetPosition);
	}

	bool ZomboCommandCameraMoveXY::isUndoAvaliable()
	{
		return !core::Vector2::equals(_prevPosition, _targetPosition);
	}

	void ZomboCommandCameraMoveXY::execute()
	{
		ZomboLog::Default.m("Do: Move camera");
		ZomboEditorCamera::Default.setPositionXY(_targetPosition);
	}

	void ZomboCommandCameraMoveXY::undo()
	{
		ZomboLog::Default.m("Undo: Move camera");
		ZomboEditorCamera::Default.setPositionXY(_prevPosition);
	}

	ZomboCameraMoveXY::ZomboCameraMoveXY(): _actualCommand(nullptr), _prevMidButtonState(false)
	{
		_lastMouse = core::Vector2(-1, -1);
	}

	void ZomboCameraMoveXY::activated()
	{
		float p, h, b;
		ZomboEditorCamera::Default.rotator.ternimateInterpolator();
		ZomboEditorCamera::Default.rotator.actualRotation.getRotation(p, h, b);
		_pAngle.set(p);
		_hAngle.set(h);
		_bAngle.set(b);
		_pAngle.setTarget(0);
		_hAngle.setTarget(0);
		_bAngle.setTarget(0);
	}

	void ZomboCameraMoveXY::updateInput()
	{
		_pAngle.update();
		_hAngle.update();
		_bAngle.update();
		ZomboEditorCamera::Default.rotator.setRotation(core::Matrix::createEuler(_pAngle.get(), _hAngle.get(), _bAngle.get()));

		ZomboEditorMouse *mouse = ZomboEditorInput::Default.mouse();

		float ellapsedTime = ZomboGameEnvironment::ellapsedSeconds;
		bool isMiddlePressed = mouse->isMiddlePressed();
		if (isMiddlePressed)
		{
			if (!_prevMidButtonState)
			{
				_prevPosition = ZomboEditorCamera::Default.getPositionXY();
				_actualCommand = nullptr;
			}
			if ((_lastMouse.getX() >= 0) && (_lastMouse.getY() >= 0))
			{
				float dx = 2.0f * (mouse->position.getX() - _lastMouse.getX()) * ZomboEditorCamera::Default.getInterpoaltedScale() / ZomboEditorViewport::Default.h;
				float dy = 2.0f * (mouse->position.getY() - _lastMouse.getY()) * ZomboEditorCamera::Default.getInterpoaltedScale() / ZomboEditorViewport::Default.h;
				
				velocityStack.updateMove(core::Vector2(-dx, dy));
				ZomboEditorCamera::Default.addPositionXY(core::Vector2(-dx, dy));							
			}
			_lastMouse = mouse->position;
		}
		else
		{
			if (_prevMidButtonState)
			{
				//if (_actualCommand == nullptr)
				{
					_actualCommand = new ZomboCommandCameraMoveXY(_prevPosition, ZomboEditorCamera::Default.getPositionXY());
					if (ZomboEditorCommands::camera()->doCommand(_actualCommand) != CommandExecutonStatus::CommandExecuted)
					{
						delete _actualCommand;
						_actualCommand = nullptr;
					}
				}
			}
			
			core::Vector2 v = velocityStack.updateVelocity();

			_lastMouse.setX(-1);
			_lastMouse.setY(-1);
			
			if (!core::Vector2::equals(v, core::Vector2::empty)) 
			{
				ZomboEditorCamera::Default.addPositionXY(v*ellapsedTime);
				if (_actualCommand != nullptr)
				{
					_actualCommand->invalidate(ZomboEditorCamera::Default.getPositionXY());
				}
			}
			
		}
		/*
		if (_actualCommand != nullptr)
		{
			_actualCommand->invalidate(ZomboEditorCamera::Default.getPositionXY());
		}
		*/
		_prevMidButtonState = isMiddlePressed;
	}
}
