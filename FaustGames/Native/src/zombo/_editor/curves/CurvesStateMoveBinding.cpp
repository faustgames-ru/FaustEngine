#include "CurvesStateMoveBinding.h"
#include "../ZomboEditorInput.h"
#include "CurvesStateSelect.h"
#include "../commands/ZomboEditorCommands.h"
#include "../drawing/Drawer.h"

namespace zombo
{
	BindingSnapping BindingSnapping::Default;
	CurvesStateMoveBinding CurvesStateMoveBinding::Default;

	BindingSnapping::BindingSnapping(): _center(nullptr), _step(0.5f), _axisIndex(0)
	{
		_axis[0] = core::Vector2(1.0f, 0.0f);
		_axis[1] = core::Vector2(0.0f, 1.0f);
		_axis[2] = core::Vector2(1.0f, 1.0f).normalize();
		_axis[3] = core::Vector2(-1.0f, 1.0f).normalize();
		_axis[4] = core::Vector2(-1.0f, 0.0f);
		_axis[5] = core::Vector2(0.0f, -1.0f);
		_axis[6] = core::Vector2(-1.0f, -1.0f).normalize();
		_axis[7] = core::Vector2(1.0f, -1.0f).normalize();
		_scale.setConfig(&SConfig::VerySlow);
	}

	void BindingSnapping::show()
	{
		_scale.setTarget(1.0f);
	}

	void BindingSnapping::setCenter(CurvesPoint* p)
	{
		_center = p;
	}

	void BindingSnapping::snap(core::Vector2& p)
	{
		// todo: all
		core::Vector2 c = _center->getXY();
		float l = 4.0f *_scale.get();
		float minD = core::Math::MaxValue;
		core::Vector2 axis;
		bool snap = false;
		float scale = 0.0f;
		for (uint i = 0; i < ZOMBO_BINDING_SNAPPING_AXIS_COUNT; i++)
		{
			core::Vector2 a = _axis[i] * l;
			float d = core::Vector2::distanceToEdge(p, c, c + a);
			if (d < CurvePointBinding::r)
			{
				if (minD > d)
				{
					minD = d;
					axis = _axis[i];
					_axisIndex = i;
					snap = true;
				}
			}
		}
		if (snap)
		{
			_snapScale.setTarget(1.0f);
			minD = core::Math::MaxValue;
			float len = (p - c).length();
			float newLen = len;
			for (float n = CurvePointBinding::trim; n < l; n += _step)
			{
				float d = core::Math::abs(n - len);
				if (d < CurvePointBinding::r)
				{
					if (minD > d)
					{
						newLen = n;
						minD = d;
					}
				}
			}

			p = c + axis * newLen;
		}
		else
		{
			_snapScale.setTarget(0.0f);
		}
	}

	void BindingSnapping::update()
	{
		_snapScale.update();
		_scale.update();
		if (_center == nullptr)
		{
			return;
		}
		if (_scale.get() < core::Math::Epsilon)
		{
			return;
		}
		core::Vector3 c = _center->getXY().toVector3();
		float l = 4.0f *_scale.get();
		uint color;
		for (uint i = 0; i < ZOMBO_BINDING_SNAPPING_AXIS_COUNT; i++)
		{
			float d = 0;
			if (_axisIndex == i)
			{
				color = graphics::Color::mulA(0xffffffff, (0.5f + _snapScale.get()*0.5f)*_scale.get());
				d = _snapScale.get() * 0.005f;
			}
			else
			{
				color = graphics::Color::mulA(0xffffffff, 0.5*_scale.get());
				d = 0;
			}
			core::Vector2 a = _axis[i] * l;
			ZomboDrawer::Default.fillEdge(color, c, c + a, d);
			if (_axisIndex == i && _snapScale.get() > core::Math::Epsilon)
			{
				core::Vector3 norm = _axis[i].rotate90cw().toVector3()*CurvePointBinding::r*_snapScale.get()*0.5f;
				for (float n = CurvePointBinding::trim; n < l; n += _step)
				{
					core::Vector3 p = c + _axis[i] * n;
					ZomboDrawer::Default.fillEdge(color, p - norm, p + norm, d*0.5f);
				}
			}
		}
		
		ZomboDrawer::Default.fillCircle(color, c, 0.01f*_scale.get());
	}

	void BindingSnapping::hide()
	{
		_scale.setTarget(0);
	}

	CurvesStateMoveBinding::CurvesStateMoveBinding(): selectedBinding(nullptr)
	{
	}

	void CurvesStateMoveBinding::start()
	{
		_downMousePos = CurvesManager::Default.mousePos;
		_prevSelectedPosition = selectedBinding->getXY();
		BindingSnapping::Default.setCenter(selectedBinding->p);
		BindingSnapping::Default.show();
	}

	void CurvesStateMoveBinding::update()
	{
		bool isLeftPressed = ZomboEditorInput::Default.mouse.isLeftPressed();
		core::Vector2 mousePos = CurvesManager::Default.mousePos;
		if (isLeftPressed)
		{
			core::Vector2 md = mousePos - _downMousePos;
			core::Vector2 sp = _prevSelectedPosition + md;
			
			// todo: snapping
			BindingSnapping::Default.snap(sp);
			selectedBinding->setXY(sp);			
			selectedBinding->updateSelectedState();
		}
		else
		{
			// todo: undo action
			ZomboCommandMoveCurveBinding *actualMoveCommand = new ZomboCommandMoveCurveBinding(selectedBinding, _prevSelectedPosition, selectedBinding->getXY());
			if (ZomboEditorCommands::Default.doCommand(actualMoveCommand) != CommandExecutonStatus::CommandExecuted)
			{
				delete actualMoveCommand;
			}
			CurvesManager::Default.setState(&CurvesStateSelect::Default);
		}
	}

	void CurvesStateMoveBinding::finish()
	{
		BindingSnapping::Default.hide();
	}

	void CurvesStateMoveBinding::setSelection(CurvePointBinding* binding)
	{
		selectedBinding = binding;
	}
}
