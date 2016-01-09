#include "ZomboCameraRotate.h"
#include "../ZomboEditorInput.h"
#include "../ZomboEditorViewport.h"

namespace zombo
{
	ZomboCameraRotate ZomboCameraRotate::Default;
	std::string ZomboCameraRotate::ModeName("Rotate");


	ZomboCameraRotate::ZomboCameraRotate()
	{
		_mouseDownPosition = core::Vector2(-1.0f, -1.0f);
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
			}
			else
			{
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
			_mouseDownPosition = core::Vector2(-1.0f, -1.0f);
			// apply agular velocity;
		}
	}

}
