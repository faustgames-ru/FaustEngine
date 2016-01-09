#include "ZomboCameraMoveXY.h"
#include "../../common/ZomboGameEnvironment.h"
#include "../ZomboEditorInput.h"
#include "../ZomboEditorViewport.h"

namespace zombo
{
	ZomboCameraMoveXY ZomboCameraMoveXY::Default;
	std::string ZomboCameraMoveXY::ModeName("Move");

	ZomboCameraMoveXY::ZomboCameraMoveXY() : _midDownTime(0)
	{
		_lastMouse = core::Vector2(-1, -1);

	}

	void ZomboCameraMoveXY::updateInput()
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
				float dx = 2.0f * (ZomboEditorInput::Default.mouse.position.getX() - _lastMouse.getX()) * ZomboEditorCamera::Default.getInterpoaltedScale() / ZomboEditorViewport::Default.h;
				float dy = 2.0f * (ZomboEditorInput::Default.mouse.position.getY() - _lastMouse.getY()) * ZomboEditorCamera::Default.getInterpoaltedScale() / ZomboEditorViewport::Default.h;


				ZomboEditorCamera::Default.position += core::Vector2(-dx, dy);

				velocities.push(VelocityStackEntry(_midDownTime, core::Vector2(-dx / ellapsedTime, dy / ellapsedTime)));
				while ((velocities.front().time < (_midDownTime - 0.1f)) || (velocities.size() > 100))
				{
					velocities.pop();
					if (velocities.empty()) break;
				}
			}
			_lastMouse = ZomboEditorInput::Default.mouse.position;
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

			ZomboEditorCamera::Default.position += _velocity * ellapsedTime;
			_velocity -= _velocityOrigin*3.0f*ellapsedTime;
		}
	}
}
