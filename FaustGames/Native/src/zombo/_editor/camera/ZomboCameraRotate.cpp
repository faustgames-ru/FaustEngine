#include "ZomboCameraRotate.h"
#include "../ZomboEditorInput.h"
#include "../ZomboEditorViewport.h"
#include "../../common/ZomboGameEnvironment.h"

namespace zombo
{
	ZomboCameraRotate ZomboCameraRotate::Default;
	std::string ZomboCameraRotate::ModeName("Rotate");


	ZomboCameraRotate::ZomboCameraRotate()
	{
		_mouseDownPosition = core::Vector2(-1.0f, -1.0f);
	}

	void ZomboCameraRotate::activated()
	{
	}

	void ZomboCameraRotate::updateInput()
	{
		core::Vector2 position = ZomboEditorInput::Default.mouse.position;
		if (ZomboEditorInput::Default.mouse.isMiddlePressed())
		{
			if ((_mouseDownPosition.getX() < 0) || (_mouseDownPosition.getY() < 0))
			{
				_originRotation = ZomboEditorCamera::Default.rotation;
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
				core::Vector3 n3 = core::Vector3(n.getX(), n.getY(), 0).normalize();
				float l = n.length();
				if (l > core::Math::Epsilon)
				{
					ZomboEditorCamera::Default.rotation = core::Matrix::mul(_originRotation, core::Matrix::createRotation(n3, -core::Math::Pi * l / ZomboEditorViewport::Default.h));
				}
				else
				{
					ZomboEditorCamera::Default.rotation = _originRotation;
				}				
			}
		}
		else
		{
			float ellapsedTime = ZomboGameEnvironment::ellapsedSeconds;
			core::Vector2 v = _velocityStack.updateVelocity();
			v.inverseY();
			core::Vector2 d = v * ellapsedTime;
			core::Vector2 n = d.rotate90cw();
			core::Vector3 n3 = core::Vector3(n.getX(), n.getY(), 0).normalize();
			float l = n.length();
			if (l > core::Math::Epsilon)
			{
				ZomboEditorCamera::Default.rotation = core::Matrix::mul(ZomboEditorCamera::Default.rotation, core::Matrix::createRotation(n3, -core::Math::Pi * l / ZomboEditorViewport::Default.h));
			}
			_lastMouse = _mouseDownPosition = core::Vector2(-1.0f, -1.0f);
		}
	}

}
