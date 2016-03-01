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

	BindingSnapType::e BindingSnapping::snap(core::Vector2& p)
	{
		// todo: all
		core::Vector2 c = _center->getXY();
		float l = 4.0f *_scale.get();
		float minD = core::Math::MaxValue;
		core::Vector2 axis;
		BindingSnapType::e snap = BindingSnapType::None;
		float scale = 0.0f;
		float len = (p - c).length();
		float sens = CurvePointBinding::r;
		//if (core::Vector2::equals(origin, c))
		{
			if (len < CurvePointBinding::trim)
			{
				p = c + (p - c).normalize()*CurvePointBinding::trim;
				return BindingSnapType::None;
				//sens = sens*0.2f;
			}
		}

		for (uint i = 0; i < ZOMBO_BINDING_SNAPPING_AXIS_COUNT; i++)
		{
			core::Vector2 a = _axis[i] * l;
			float d = core::Vector2::distanceToEdge(p, c, c + a);
			if (d < sens)
			{
				if (minD > d)
				{
					minD = d;
					axis = _axis[i];
					_axisIndex = i;
					snap = BindingSnapType::Line;
				}
			}
		}
		
		if (snap != BindingSnapType::None)
		{
			minD = core::Math::MaxValue;
			float newLen = len;
			for (float n = CurvePointBinding::trim; n < l; n += _step)
			{
				float d = core::Math::abs(n - len);
				if (d < sens)
				{
					if (minD > d)
					{
						newLen = n;
						minD = d;
					}
					snap = BindingSnapType::Line;
				}
			}
			if (newLen < core::Math::Epsilon)
			{
				_snapScale.setTarget(0.0f);
			}
			else
			{
				_snapScale.setTarget(1.0f);
			}
			p = c + axis * newLen;
		}
		else
		{
			_snapScale.setTarget(0.0f);
		}
		return snap;
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

	CurvesStateMoveBinding::CurvesStateMoveBinding(): selectedBinding(nullptr), _snap(BindingSnapType::None)
	{
	}

	void CurvesStateMoveBinding::start()
	{
		_downMousePos = CurvesManager::Default.mousePos;
		_prevSelectedPosition = selectedBinding->getWorldP();// selectedBinding->getXY();
		_prevPosition = selectedBinding->getWorldP();// getTargetXY();

		selectedBinding->setWorldP(_prevPosition);
		_prevPosition = selectedBinding->getWorldP();// getTargetXY();


		_snap = BindingSnapType::None;
		BindingSnapping::Default.setCenter(selectedBinding->p);
		BindingSnapping::Default.origin = _prevSelectedPosition;
		BindingSnapping::Default.show();
	}

	void CurvesStateMoveBinding::update()
	{
		ZomboEditorMouse *mouse = ZomboEditorInput::Default.queryMouse(this);
		if (mouse == nullptr) return;

		bool isLeftPressed = mouse->isLeftPressed();
		core::Vector2 mousePos = CurvesManager::Default.mousePos;
		if (isLeftPressed)
		{
			mouse->handle(this);
			core::Vector2 md = mousePos - _downMousePos;
			core::Vector2 sp = _prevSelectedPosition + md;
			// todo: snapping
			
			BindingSnapType::e snap = BindingSnapping::Default.snap(sp);
			selectedBinding->setWorldP(sp);
			selectedBinding->p->invalidateBindings(selectedBinding);

			//BindingSnapType::e snap = BindingSnapping::Default.snap(sp);
			//float l = selectedBinding->getLocalP(sp).length();
			//selectedBinding->p->invalidateBindingsLen(l);
			/*
			if (snap != _snap)
			{
				//selectedBinding->setWorldP(sp);
				//selectedBinding->p->invalidateBindings(selectedBinding);
				
				selectedBinding->p->invalidateBindingsLen(l);
				//selectedBinding->setTargetXY(sp);
			}
			else
			{
				//selectedBinding->setWorldP(sp);
				//selectedBinding->p->invalidateBindings(selectedBinding);

				selectedBinding->p->invalidateBindingsLen(l);
				//selectedBinding->setXY(sp);
			}
			_snap = snap;
			*/
			//selectedBinding->updateSelectedState();
			
		}
		else
		{
			// todo: undo action
			
			ZomboCommandMoveCurveBinding *actualMoveCommand = new ZomboCommandMoveCurveBinding(selectedBinding, _prevPosition, selectedBinding->getWorldP());
			if (ZomboEditorCommands::Default.doCommand(actualMoveCommand) != CommandExecutonStatus::CommandExecuted)
			{
				delete actualMoveCommand;
			}
			
			CurvesManager::Default.setState(&CurvesStateSelect::Default);
			mouse->handle(nullptr);
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
