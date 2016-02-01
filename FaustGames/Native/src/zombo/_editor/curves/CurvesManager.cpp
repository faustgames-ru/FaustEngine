#include "CurvesManager.h"
#include "../ZomboEditorRenderService.h"
#include "../ZomboEditorInput.h"
#include "../ZomboEditorViewport.h"
#include "../ZomboEditorCamera.h"
#include "../drawing/Drawer.h"
#include "../../ZomboLog.h"
#include "../commands/ZomboEditorCommands.h"
#include "CurvesStateSelect.h"

namespace zombo
{
	void CurvesState::start()
	{
	}

	void CurvesState::update()
	{
	}

	void CurvesState::finish()
	{
	}

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
		_point->xy.setTarget(_targetPosition);
	}

	void ZomboCommandMoveCurvePoint::undo()
	{
		ZomboLog::Default.m("Undo: Move point");
		_point->xy.setTarget(_prevPosition);
	}

	core::Vector2 CurvesPoint::getXY() const
	{
		return xy.get();
	}

	core::Vector2 CurvesPoint::getTargetXY() const
	{
		return xy.getTarget();
	}

	CurvesPoint::CurvesPoint() : _scale(1.0f), _rot(core::Math::Pi * 0.25f)
	{
	}

	CurvesPoint::CurvesPoint(core::Vector2 p): xy(p), _scale(1.0f), _scaleEx(1.0f), _alpha(1.0f), _rot(core::Math::Pi * 0.25f)
	{
		_scale.setConfig(&SConfig::Fast);
		_alpha.setConfig(&SConfig::Default);
		_scaleEx.setConfig(&SConfig::Slow);
	}

	float CurvesPoint::getR() const
	{
		return 0.1f;
	}

	bool CurvesPoint::isUnderMouse() const
	{
		core::Vector2 m = CurvesManager::Default.mousePos;
		float mx = m.getX();
		float my = m.getY();
		float sx = getR()*_scale.get();
		float sy = getR()*_scale.get();
		core::Vector2 p = xy.get();
		float px = p.getX();
		float py = p.getY();
		return
			px - sx <= mx && mx <= px + sx &&
			py - sy <= my && my <= py + sy;
	}

	float CurvesPoint::distanceToMouse() const
	{
		return (CurvesManager::Default.mousePos - getXY()).length();
	}

	void CurvesPoint::updateCoords()
	{
		xy.update();
	}

	void renderEdge(uint c, core::Vector2 p0, core::Vector2 p1, float r)
	{		
		ZomboDrawer::Default.fillEdge(c, p0.toVector3(), p1.toVector3(), r);
		//ZomboDrawer::Default.fillCircle(c, p0.toVector3(), r);
		//ZomboDrawer::Default.fillCircle(c, p1.toVector3(), r);
	}

	void renderPoint(core::Vector2 xy, float r, float a, float rot)
	{
		uint c1 = 0xffffffff;
		uint c0 = graphics::Color::mulA(0xffffffff, a);
		
		ZomboDrawer::Default.fillRing(c0, xy.toVector3(), r, r*0.02f);
		ZomboDrawer::Default.fillRect(c1, xy.toVector3(), r*0.8f, rot);
	}
		
	void CurvesPoint::update()
	{
		_scale.update();
		_scaleEx.update();
		_alpha.update();
		_rot.update();
		renderPoint(xy.get(), getR() * _scale.get()*_scaleEx.get(), _alpha.get(), _rot.get());
	}

	void CurvesPoint::setScale(float scale)
	{
		_scale.setTarget(scale);
	}

	void CurvesPoint::setScaleEx(float scale)
	{
		_scaleEx.setTarget(scale);
	}

	void CurvesPoint::setScaleFullEx(float scale)
	{
		_scaleEx.setAll(scale);
	}

	void CurvesPoint::setAlpha(float a)
	{
		_alpha.setTarget(a);
	}

	void CurvesPoint::setRot(float rot)
	{
		_rot.setTarget(rot);
	}

	void CurvesPoint::updateSelectedState()
	{
		setAlpha(1.0f);
		setScale(1.5f);
		setRot(core::Math::Pi * 1.25f);
	}

	void CurvesPoint::updateHoverState()
	{
		setAlpha(1.0f);
		setScale(1.2f);
		setRot(core::Math::Pi * 0.5f);
	}

	void CurvesPoint::updateRegularState()
	{
		setAlpha(1.0f);
		setScale(1.0f);
		setRot(core::Math::Pi * 0.25f);
	}

	void CurvesPoint::updateHidenState()
	{
		setAlpha(0.0f);
		setScale(1.0f);
		setRot(core::Math::Pi * 0.25f);
	}

	geometry::Aabb2d CurvesPoint::getAabb()
	{
		return geometry::Aabb2d(getXY() - core::Vector2(getR(), getR()), getXY() + core::Vector2(getR(), getR()));
	}

	float CurvesPoint::getScale() const
	{
		return _scale.get();
	}

	float CurvesPoint::getScaleEx() const
	{
		return _scaleEx.get();
	}

	CurveSegment::CurveSegment(): p0(nullptr), p1(nullptr), color(0xffffffff), _scale(1.0f)
	{
		_scale.setConfig(&SConfig::Fast);
	}

	void CurveSegment::updateInput()
	{
	}

	uint segmentDetail = 16;
	float segnemtLen = 0.1f;

	void CurveSegment::update()
	{
		//renderPoint(p0->xy + d0, p0->_scale.getValue() * p0->getR() * 0.5f, p0->_alpha.getValue());
		//renderPoint(p1->xy + d1, p1->_scale.getValue() * p1->getR() * 0.5f, p1->_alpha.getValue());
		_scale.update();
		uint count = calcDetail();// segmentDetail;

		core::Vector2 ps = core::Vector2::cubic(p0->getXY(), p0->getXY() + d0, p1->getXY() + d1, p1->getXY(), 0);
		for (uint i = 1; i < count; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(count - 1);
			core::Vector2 pf = core::Vector2::cubic(p0->getXY(), p0->getXY() + d0, p1->getXY() + d1, p1->getXY(), u);
			renderEdge(color, ps, pf, getR()*_scale.get());
			ps = pf;
		}
	}

	uint CurveSegment::calcDetail() const
	{
		uint r = static_cast<uint>(core::Math::round(getBaseLen() / segnemtLen));
		if (r < 6)
			r = 6;
		if (r > 64)
			r = 64;
		return r;
	}

	float CurveSegment::getBaseLen() const
	{
		return d0.length() + (p0->getXY() + d0 - p1->getXY() - d1).length() + d1.length();
	}

	float CurveSegment::getR() const
	{
		return 0.01f;
	}

	bool CurveSegment::isUnderMouse() const
	{
		core::Vector2 m = CurvesManager::Default.mousePos;

		geometry::Aabb2d bounds = getAabb();
		if (!bounds.contains(m)) return false;

		uint count = calcDetail();
		core::Vector2 ps = core::Vector2::cubic(p0->getXY(), p0->getXY() + d0, p1->getXY() + d1, p1->getXY(), 0);
		for (uint i = 1; i < count; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(count - 1);
			core::Vector2 pf = core::Vector2::cubic(p0->getXY(), p0->getXY() + d0, p1->getXY() + d1, p1->getXY(), u);
			float l = core::Vector2::distanceToEdge(m, pf, ps);
			ps = pf;
			if (l < getR()*2.0f)
				return true;
		}
		return false;
	}

	float CurveSegment::distanceToMouse() const
	{
		core::Vector2 m = CurvesManager::Default.mousePos;
		uint count = calcDetail();
		float minL = core::Math::MaxValue;
		core::Vector2 ps = core::Vector2::cubic(p0->getXY(), p0->getXY() + d0, p1->getXY() + d1, p1->getXY(), 0);
		for (uint i = 1; i < count; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(count - 1);
			core::Vector2 pf = core::Vector2::cubic(p0->getXY(), p0->getXY() + d0, p1->getXY() + d1, p1->getXY(), u);
			float l = core::Vector2::distanceToEdge(m, pf, ps);
			ps = pf;
			if (l < minL)
			{
				minL = l;
			}
		}
		return minL;
	}

	void CurveSegment::updateRegularState()
	{
		//color = 0xffffffff;
		_scale.setTarget(1.0f);
	}

	void CurveSegment::updateHoverState()
	{
		_scale.setTarget(2.0f);
		//color = 0xffff00ff;
	}

	geometry::Aabb2d CurveSegment::getAabb() const
	{
		geometry::Aabb2d bounds;
		bounds.expand(p0->getXY());
		bounds.expand(p0->getXY() + d0);
		bounds.expand(p1->getXY());
		bounds.expand(p1->getXY() + d1);
		return bounds;
	}

	void CurvesVisibleItems::clear()
	{
		points.clear();
		segments.clear();
	}


	CurvesSelection::CurvesSelection(): point(nullptr), segment(nullptr)
	{
	}

	CurvesSelection::CurvesSelection(CurvesPoint* p) : point(p), segment(nullptr)
	{
	}

	CurvesSelection::CurvesSelection(CurveSegment* s): point(nullptr), segment(s)
	{
	}

	CurvesManager CurvesManager::Default;

	CurvesManager::CurvesManager() : scale(1.0f), _snappingRange(0.1f)
	{
		_actualState = &CurvesStateSelect::Default;
	}

	CurvesPoint *CurvesManager::snap(core::Vector2& p, CurvesPoint *selection)
	{		
		for (uint i = 0; i < _visibleItems.points.size(); i++)
		{
			if (_visibleItems.points[i] == selection) continue;
			float r = (p - _visibleItems.points[i]->getXY()).length();
			if (r < _snappingRange)
			{
				p = _visibleItems.points[i]->getXY();
				return _visibleItems.points[i];
			}
		}		
		return nullptr;
	}

	void CurvesManager::update()
	{
		scale.update();
		for (uint i = 0; i < _points.size(); i++)
		{
			_points[i]->updateCoords();
		}

		mousePos = ZomboEditorCamera::Default.getMouseProjection(0);
		_visibleItems.clear();
		queryVisibleItems(_visibleItems);
		if (_actualState != nullptr)
		{
			_actualState->update();
		}
		
		for (uint i = 0; i < _visibleItems.points.size(); i++)
		{
			_visibleItems.points[i]->update();
		}
		for (uint i = 0; i < _visibleItems.segments.size(); i++)
		{
			_visibleItems.segments[i]->update();
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

	CurvesSelection CurvesManager::findSelection(CurvesVisibleItems& items)
	{
		float l = core::Math::MaxValue;
		CurvesPoint * selectedPoint = nullptr;
		for (uint i = 0; i < items.points.size(); i++)
		{
			if (items.points[i]->isUnderMouse())
			{
				float ml = items.points[i]->distanceToMouse();
				if (ml < l)
				{
					selectedPoint = items.points[i];
					l = ml;
				}
			}
		}
		if (selectedPoint != nullptr)
		{
			return CurvesSelection(selectedPoint);
		}
		CurveSegment * selectedSegment= nullptr;
		for (uint i = 0; i < items.segments.size(); i++)
		{
			if (items.segments[i]->isUnderMouse())
			{
				float ml = items.segments[i]->distanceToMouse();
				if (ml < l)
				{
					selectedSegment = items.segments[i];
					l = ml;
				}
			}
		}
		return CurvesSelection(selectedSegment);
	}

	CurvesVisibleItems& CurvesManager::getVisibleItems()
	{
		return _visibleItems;
	}

	void CurvesManager::queryVisibleItems(CurvesVisibleItems& items)
	{
		geometry::Frustum frustum = ZomboEditorCamera::Default.frustum;
		float scale = ZomboEditorViewport::Default.h * 0.5f / ZomboEditorCamera::Default.getInterpoaltedScale();
		segnemtLen = 8 / scale;
		// todo:: optimize;
		for (uint i = 0; i < _points.size(); i++)
		{
			geometry::Aabb2d aabb = _points[i]->getAabb();
			core::Vector2 size = (aabb.Max - aabb.Min)*scale;
			float pxMin = 16;
			if (size.getX() < pxMin && size.getY() < pxMin)
			{
				_points[i]->setScaleEx(0);
				if (_points[i]->getScaleEx() < 0.001f)
					continue;
			}
			else
			{
				_points[i]->setScaleEx(1);
			}
			if (!frustum.include(aabb, 0)) {
				continue;
			}
			items.points.push_back(_points[i]);
		}
		for (uint i = 0; i < _segments.size(); i++)
		{
			if (frustum.include(_segments[i]->getAabb(), 0))
			{
				items.segments.push_back(_segments[i]);
			}
		}
	}

	void CurvesManager::setState(ICurvesState* state)
	{
		if (_actualState != nullptr)
			_actualState->finish();
		_actualState = state;
		if (_actualState != nullptr)
			_actualState->start();
	}
}
