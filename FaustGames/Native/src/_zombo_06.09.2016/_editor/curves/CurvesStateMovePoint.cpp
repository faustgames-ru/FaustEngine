#include "CurvesStateMovePoint.h"
#include "../ZomboEditorInput.h"
#include "CurvesStateSelect.h"
#include "../commands/ZomboEditorCommands.h"
#include "../drawing/Drawer.h"
#include "../../common/ValuesAnimator.h"

namespace zombo
{
	PointSnapping PointSnapping::Default;
	CurvesStateMovePoint CurvesStateMovePoint::Default;

	PointSnapping::PointSnapping() : _step(1.0f), _size(10), _snapX(core::Math::MaxValue), _snapY(core::Math::MaxValue)
	{
		_scale.setConfig(&SConfig::VerySlow);
		//_snapScaleX.setConfig(&SConfig::VerySlow);
		//_snapScaleY.setConfig(&SConfig::VerySlow);
	}

	void PointSnapping::show()
	{
		_scale.setTarget(1.0f);
	}

	void PointSnapping::snap(core::Vector2& p)
	{
		bool snapX = false;
		bool snapY = false;
		for (float v = -_size; v <= _size; v += _step)
		{
			if (!snapX)
			{
				float dx = core::Math::abs(p.getX() - v);
				if (dx < CurvesPoint::getR())
				{
					_snapX = v;
					snapX = true;
				}
			}
			if (!snapY)
			{
				float dy = core::Math::abs(p.getY() - v);
				if (dy < CurvesPoint::getR())
				{
					_snapY = v;
					snapY = true;
				}
			}
			if (snapX && snapY)
				break;
		}
		if (snapX)
		{
			_snapScaleX.setTarget(1.0f);
			p.setX(_snapX);
		}
		else
		{
			_snapScaleX.setTarget(0.0f);
		}
		if (snapY)
		{
			_snapScaleY.setTarget(1.0f);
			p.setY(_snapY);
		}
		else
		{			
			_snapScaleY.setTarget(0.0f);
		}
	}

	void PointSnapping::update()
	{
		_scale.update();
		_snapScaleX.update();
		_snapScaleY.update();
		if (_scale.get() < core::Math::Epsilon)
			return;
		float s = _scale.get()*_size;
		uint color = graphics::Color::mulA(0xffffffff, 0.5f);
		for (float v = -_size; v <= _size; v+=_step)
		{
			core::Vector3 cx(v, 0, 0);
			core::Vector3 cy(0, v, 0);
			core::Vector3 l(v, -s, 0);
			core::Vector3 r(v, s, 0);
			core::Vector3 t(s, v, 0);
			core::Vector3 b(-s, v, 0);
			float d = 0.0f;
			if (core::Math::equals(v, _snapX))
			{
				color = graphics::Color::mulA(0xffffffff, (0.5f+ _snapScaleX.get()*0.5f)*_scale.get());
				d = _snapScaleX.get()*0.005f;
			}
			else
			{
				color = graphics::Color::mulA(0xffffffff, 0.5f * _scale.get());
			}
			ZomboDrawer::Default.fillEdge(color, cx, l, d);
			ZomboDrawer::Default.fillEdge(color, cx, r, d);
			d = 0.0f;
			if (core::Math::equals(v, _snapY))
			{
				color = graphics::Color::mulA(0xffffffff, (0.5f + _snapScaleY.get()*0.5f)*_scale.get());
				d = _snapScaleY.get()*0.005f;
			}
			else
			{
				color = graphics::Color::mulA(0xffffffff, 0.5f*_scale.get());
			}
			ZomboDrawer::Default.fillEdge(color, cy, t, d);
			ZomboDrawer::Default.fillEdge(color, cy, b, d);
		}
	}

	void PointSnapping::hide()
	{
		_scale.setTarget(0);
	}

	CurvesStateMovePoint::CurvesStateMovePoint(): selectedPoint(nullptr), _replacePoint(nullptr)
	{
	}

	void CurvesStateMovePoint::start()
	{
		_downMousePos = CurvesManager::Default.mousePos;
		_prevSelectedPosition = selectedPoint->getXY(); 
		_prevPosition = selectedPoint->getTargetXY();
		_replacePoint = nullptr;
		PointSnapping::Default.show();
	}

	void CurvesStateMovePoint::update()
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
			CurvesPoint *replacePoint = CurvesManager::Default.snap(sp, selectedPoint);
			if (replacePoint != _replacePoint)
			{
				Animators::Vector2.animate(&selectedPoint->xy, sp);
			}
			else
			{
				if (replacePoint == nullptr)
				{
					PointSnapping::Default.snap(sp);
				}
				Animators::Vector2.terminate(&selectedPoint->xy);
				selectedPoint->xy = sp;
			}
			_replacePoint = replacePoint;
			CurvesManager::Default.animateToSelect(selectedPoint);			
		}
		else
		{
			// todo: replace command support
			ZomboCommandMoveCurvePoint *actualMovePointCommand = new ZomboCommandMoveCurvePoint(selectedPoint, _prevPosition, selectedPoint->getTargetXY());
			if (ZomboEditorCommands::Default.doCommand(actualMovePointCommand) != CommandExecutonStatus::CommandExecuted)
			{
				delete actualMovePointCommand;
			}
			CurvesManager::Default.setState(&CurvesStateSelect::Default);
			mouse->handle(nullptr);
		}
	}

	void CurvesStateMovePoint::finish()
	{
		PointSnapping::Default.hide();
	}

	void CurvesStateMovePoint::setSelection(CurvesPoint* point)
	{
		selectedPoint = point;
	}
}
