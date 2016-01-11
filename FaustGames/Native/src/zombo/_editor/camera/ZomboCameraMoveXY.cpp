#include "ZomboCameraMoveXY.h"
#include "../../common/ZomboGameEnvironment.h"
#include "../ZomboEditorInput.h"
#include "../ZomboEditorViewport.h"
#include "../commands/ZomboEditorCommands.h"

namespace zombo
{
	ZomboCameraMoveXY ZomboCameraMoveXY::Default;
	std::string ZomboCameraMoveXY::ModeName("Move");


	ZomboCopmmandCameraMoveXY::ZomboCopmmandCameraMoveXY(const core::Vector2 prevPosition, const core::Vector2 newPosition)
	{
		_targetPosition = newPosition;
		_prevPosition = prevPosition;
	}

	void ZomboCopmmandCameraMoveXY::invalidate(const core::Vector2 newPosition)
	{
		_targetPosition = newPosition;
	}

	bool ZomboCopmmandCameraMoveXY::isExecutionAvaliable()
	{
		return !core::Vector2::equals(_prevPosition, _targetPosition);
	}

	bool ZomboCopmmandCameraMoveXY::isUndoAvaliable()
	{
		return !core::Vector2::equals(_prevPosition, _targetPosition);
	}

	void ZomboCopmmandCameraMoveXY::execute()
	{
		ZomboEditorCamera::Default.setPositionXY(_targetPosition);
	}

	void ZomboCopmmandCameraMoveXY::undo()
	{
		ZomboEditorCamera::Default.setPositionXY(_prevPosition);
	}

	ZomboCameraMoveXY::ZomboCameraMoveXY(): _actualCommand(nullptr), _prevMidButtonState(false)
	{
		_lastMouse = core::Vector2(-1, -1);
	}

	void ZomboCameraMoveXY::activated()
	{
		float p, h, b;
		ZomboEditorCamera::Default.rotation.GetRotation(p, h, b);
		_pAngle.setValue(p);
		_hAngle.setValue(h);
		_bAngle.setValue(b);
		_pAngle.setTargetValue(0);
		_hAngle.setTargetValue(0);
		_bAngle.setTargetValue(0);
	}

	void ZomboCameraMoveXY::updateInput()
	{
		_pAngle.update();
		_hAngle.update();
		_bAngle.update();
		ZomboEditorCamera::Default.rotation = core::Matrix::createEuler(_pAngle.getValue(), _hAngle.getValue(), _bAngle.getValue());

		float ellapsedTime = ZomboGameEnvironment::ellapsedSeconds;
		bool isMiddlePressed = ZomboEditorInput::Default.mouse.isMiddlePressed();
		if (isMiddlePressed)
		{
			if (!_prevMidButtonState)
			{
				_prevPosition = ZomboEditorCamera::Default.getPositionXY();
				_actualCommand = nullptr;
			}
			if ((_lastMouse.getX() >= 0) && (_lastMouse.getY() >= 0))
			{
				float dx = 2.0f * (ZomboEditorInput::Default.mouse.position.getX() - _lastMouse.getX()) * ZomboEditorCamera::Default.getInterpoaltedScale() / ZomboEditorViewport::Default.h;
				float dy = 2.0f * (ZomboEditorInput::Default.mouse.position.getY() - _lastMouse.getY()) * ZomboEditorCamera::Default.getInterpoaltedScale() / ZomboEditorViewport::Default.h;
				
				velocityStack.updateMove(core::Vector2(-dx, dy));
				ZomboEditorCamera::Default.addPositionXY(core::Vector2(-dx, dy));							
			}
			_lastMouse = ZomboEditorInput::Default.mouse.position;
		}
		else
		{
			if (_prevMidButtonState)
			{
				if (_actualCommand == nullptr)
				{
					_actualCommand = new ZomboCopmmandCameraMoveXY(_prevPosition, ZomboEditorCamera::Default.getPositionXY());
				}
			}
			
			core::Vector2 v = velocityStack.updateVelocity();

			_lastMouse.setX(-1);
			_lastMouse.setY(-1);
			
			if (!core::Vector2::equals(v, core::Vector2::empty)) 
			{
				ZomboEditorCamera::Default.addPositionXY(v*ellapsedTime);
			}
			else
			{
				if (_actualCommand != nullptr)
				{
					_actualCommand->invalidate(ZomboEditorCamera::Default.getPositionXY());
					if (ZomboEditorCommands::Default.doCommand(_actualCommand) != CommandExecutonStatus::CommandExecuted)
					{
						delete _actualCommand;
						_actualCommand = nullptr;
					}
					_actualCommand = nullptr;
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
