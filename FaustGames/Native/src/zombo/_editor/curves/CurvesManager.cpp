#include "CurvesManager.h"
#include "../ZomboEditorRenderService.h"
#include "../ZomboEditorInput.h"
#include "../ZomboEditorViewport.h"
#include "../ZomboEditorCamera.h"
#include "../drawing/Drawer.h"
#include "../../ZomboLog.h"
#include "../commands/ZomboEditorCommands.h"

namespace zombo
{
	ZomboCommandMoveCurvePoint::ZomboCommandMoveCurvePoint(CurvesPoint* point, const core::Vector2 prevPosition, const core::Vector2 newPosition)
	{
		_point = point;
		_prevPosition = prevPosition;
		_targetPosition = newPosition;
	}

	void ZomboCommandMoveCurvePoint::invalidate(const core::Vector2 newPosition)
	{
		_targetPosition = newPosition;
	}

	bool ZomboCommandMoveCurvePoint::isExecutionAvaliable()
	{
		return !core::Vector2::equals(_prevPosition, _targetPosition);
	}

	bool ZomboCommandMoveCurvePoint::isUndoAvaliable()
	{
		return !core::Vector2::equals(_prevPosition, _targetPosition);
	}

	void ZomboCommandMoveCurvePoint::execute()
	{
		ZomboLog::Default.m("Do: Move point");
		_point->x.setTargetValueIfNotEqual(_targetPosition.getX());
		_point->y.setTargetValueIfNotEqual(_targetPosition.getY());
	}

	void ZomboCommandMoveCurvePoint::undo()
	{
		ZomboLog::Default.m("Undo: Move point");
		_point->x.setTargetValueIfNotEqual(_prevPosition.getX());
		_point->y.setTargetValueIfNotEqual(_prevPosition.getY());
	}

	core::Vector2 CurvesPoint::getXY() const
	{
		return core::Vector2(x.getValue(), y.getValue());
	}

	core::Vector2 CurvesPoint::getTargetXY() const
	{
		return core::Vector2(x.getTargetValue(), y.getTargetValue());
	}

	CurvesPoint::CurvesPoint() : _scale(1.0f), _rot(0.0f)
	{
	}

	CurvesPoint::CurvesPoint(core::Vector2 p): x(p.getX()), y(p.getY()), _scale(1.0f), _alpha(1.0f), _rot(0.0f)
	{
		_scale.setDuration(0.1f);
		_alpha.setDuration(0.3f);
	}

	float CurvesPoint::getR()
	{
		return 0.1f;
	}

	bool CurvesPoint::isUnderMouse()
	{
		core::Vector2 m = CurvesManager::Default.mousePos;
		float mx = m.getX();
		float my = m.getY();
		float sx = getR()*_scale.getValue();
		float sy = getR()*_scale.getValue();
		float px = x.getValue();
		float py = y.getValue();
		return
			px - sx <= mx && mx <= px + sx &&
			py - sy <= my && my <= py + sy;
	}
	

	void renderEdge(core::Vector2 p0, core::Vector2 p1)
	{
		uint c = 0xffffffff;
		ColorVertex pointRest[2] =
		{
			ColorVertex(p0, c),
			ColorVertex(p1, c),
		};

		ushort indices[8] =
		{
			0, 1,
		};
		ZomboEditorRenderService::Default.drawLines(pointRest, 2, indices, 1);
	}

	void renderPoint(core::Vector2 xy, float r, float a, float rot)
	{
		uint c0 = graphics::Color::mulA(0xff000000, a);
		uint c1 = graphics::Color::mulA(0xffffffff, a);
		
		ZomboDrawer::Default.fillRing(c1, xy.toVector3(), r, r*0.02f);
		ZomboDrawer::Default.fillRect(c1, xy.toVector3(), r*0.8f, rot);

	}
		
	void CurvesPoint::update()
	{
		x.update();
		y.update();
		_scale.update();
		_alpha.update();
		_rot.update();
		renderPoint(core::Vector2(x.getValue(), y.getValue()), getR() * _scale.getValue(), _alpha.getValue(), _rot.getValue());
	}

	void CurvesPoint::setScale(float scale)
	{
		_scale.setTargetValueIfNotEqual(scale);
	}

	void CurvesPoint::setAlpha(float a)
	{
		_alpha.setTargetValueIfNotEqual(a);
	}

	void CurvesPoint::setRot(float rot)
	{
		_rot.setTargetValueIfNotEqual(rot);
	}

	void CurvesPoint::updateSelectedState()
	{
		setAlpha(1.0f);
		setScale(1.5f);
		setRot(core::Math::Pi * 1.25);
	}

	void CurvesPoint::updateHoverState()
	{
		setAlpha(1.0f);
		setScale(1.2f);
		setRot(core::Math::Pi * 0.5);
	}

	void CurvesPoint::updateRegularState()
	{
		setAlpha(1.0f);
		setScale(1.0f);
		setRot(core::Math::Pi * 0.25);
	}

	void CurvesPoint::updateHidenState()
	{
		setAlpha(0.0f);
		setScale(1.0f);
		setRot(core::Math::Pi * 0.25);
	}

	void CurveSegment::updateInput()
	{
	}

	void CurveSegment::update()
	{
		//renderPoint(p0->xy + d0, p0->_scale.getValue() * p0->getR() * 0.5f, p0->_alpha.getValue());
		//renderPoint(p1->xy + d1, p1->_scale.getValue() * p1->getR() * 0.5f, p1->_alpha.getValue());

		uint count = 16;

		core::Vector2 ps = core::Vector2::cubic(p0->getXY(), p0->getXY() + d0, p1->getXY() + d1, p1->getXY(), 0);
		for (uint i = 1; i < count; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(count - 1);
			core::Vector2 pf = core::Vector2::cubic(p0->getXY(), p0->getXY() + d0, p1->getXY() + d1, p1->getXY(), u);
			renderEdge(ps, pf);
			ps = pf;
		}
	}

	CurvesManager CurvesManager::Default;

	CurvesManager::CurvesManager() : scale(1.0f), _selectedPoint(nullptr), _actualMovePointCommand(nullptr)
	{
	}

	void CurvesManager::update()
	{
		scale.update();
		mousePos = ZomboEditorCamera::Default.getMouseProjection(0);		
		bool isLeftPressed = ZomboEditorInput::Default.mouse.isLeftPressed();
		bool wasLeftPressed = ZomboEditorInput::Default.mouse.isPrevStateLeftPressed();
		if (isLeftPressed)
		{
			if (!wasLeftPressed)
			{
				for (uint i = 0; i < _points.size(); i++)
				{
					if (_points[i]->isUnderMouse())
					{
						_selectedPoint = _points[i];
						_prevSelectedPosition = _selectedPoint->getTargetXY();
						break;
					}
				}
			}
			else
			{
				if (_selectedPoint != nullptr)
				{
					core::Vector2 md = mousePos - prevMousePos;
					_selectedPoint->x.setAllValues(_selectedPoint->x.getTargetValue() + md.getX());
					_selectedPoint->y.setAllValues(_selectedPoint->y.getTargetValue() + md.getY());
					for (uint i = 0; i < _points.size(); i++)
					{
						if (_points[i] != _selectedPoint)
						{
							_points[i]->updateHidenState();
						}
						else
						{
							_points[i]->updateSelectedState();

						}
					}
				}
				else
				{
					for (uint i = 0; i < _points.size(); i++)
					{
						if (_points[i]->isUnderMouse())
						{
							_points[i]->updateHoverState();
						}
						else
						{
							_points[i]->updateRegularState();
						}
					}
				}
			}
		}
		else
		{
			if (wasLeftPressed)
			{
				if (_selectedPoint != nullptr)
				{
					_actualMovePointCommand = new ZomboCommandMoveCurvePoint(_selectedPoint, _prevSelectedPosition, _selectedPoint->getTargetXY());
					if (ZomboEditorCommands::Default.doCommand(_actualMovePointCommand) != CommandExecutonStatus::CommandExecuted)
					{
						delete _actualMovePointCommand;
						_actualMovePointCommand = nullptr;
					}
				}
			}

			_selectedPoint = nullptr;
			for (uint i = 0; i < _points.size(); i++)
			{
				if (_points[i]->isUnderMouse())
				{
					_points[i]->updateHoverState();
				} 
				else
				{
					_points[i]->updateRegularState();
				}
			}
		}
		prevMousePos = mousePos;
		for (uint i = 0; i < _points.size(); i++)
		{
			_points[i]->update();
		}
		for (uint i = 0; i < _segments.size(); i++)
		{
			_segments[i]->update();
		}		
	}

	void CurvesManager::addCurve(core::Vector2 p0, core::Vector2 p1, core::Vector2 p2, core::Vector2 p3)
	{
		CurvesPoint *cp0 = new CurvesPoint(p0);
		CurvesPoint *cp1 = new CurvesPoint(p3);
		core::Vector2 d0 = p1 - p0;
		core::Vector2 d1 = p2 - p3;
		CurveSegment *seg = new CurveSegment();
		seg->p0 = cp0;
		seg->p1 = cp1;
		seg->d0 = d0;
		seg->d1 = d1;
		_points.push_back(cp0);
		_points.push_back(cp1);
		_segments.push_back(seg);
	}
}
