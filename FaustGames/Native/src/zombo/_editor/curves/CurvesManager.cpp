#include "CurvesManager.h"
#include "../ZomboEditorRenderService.h"
#include "../ZomboEditorInput.h"
#include "../ZomboEditorViewport.h"
#include "../ZomboEditorCamera.h"
#include "../drawing/Drawer.h"
#include "../../ZomboLog.h"
#include "../commands/ZomboEditorCommands.h"
#include "CurvesStateSelect.h"
#include "CurvesStateMoveBinding.h"
#include "CurvesStateMovePoint.h"

namespace zombo
{
	float CurvesPoint::r(0.1f);
	core::Vector2 CurvesPoint::size(0.2f, 0.2f);
	float CurvePointBinding::r(0.05f);
	core::Vector2 CurvePointBinding::size(0.1f, 0.1f);
	float CurvePointBinding::trim(0.15f);

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

	ZomboCommandMoveCurveBinding::ZomboCommandMoveCurveBinding(CurvePointBinding* binding, const core::Vector2 prevPosition, const core::Vector2 newPosition)
	{
		_binding = binding;
		_prevPosition = prevPosition;
		_targetPosition = newPosition;

	}

	void ZomboCommandMoveCurveBinding::invalidate(const core::Vector2 newPosition)
	{
		_targetPosition = newPosition;
	}

	bool ZomboCommandMoveCurveBinding::isExecutionAvaliable()
	{
		return !core::Vector2::equals(_prevPosition, _targetPosition);
	}

	bool ZomboCommandMoveCurveBinding::isUndoAvaliable()
	{
		return !core::Vector2::equals(_prevPosition, _targetPosition);
	}

	void ZomboCommandMoveCurveBinding::execute()
	{
		ZomboLog::Default.m("Do: Move tangent");
		_binding->setTargetXY(_targetPosition);
	}

	void ZomboCommandMoveCurveBinding::undo()
	{
		ZomboLog::Default.m("Undo: Move tangent");
		_binding->setTargetXY(_prevPosition);
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

	float CurvesPoint::getR()
	{
		return r;
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

	void renderPoint(core::Vector2 xy, float r, float a, float a1, float rot)
	{
		uint c0 = graphics::Color::mulA(0xffffffff, a);
		uint c1 = graphics::Color::mulA(0xffffffff, a1);

		ZomboDrawer::Default.fillRing(c0, xy.toVector3(), r, r*0.02f);
		ZomboDrawer::Default.fillRect(c1, xy.toVector3(), r*0.8f, rot);
	}
		
	void renderExtraPoint(core::Vector2 xy, float r, float a, float rot)
	{
		//uint c1 = 0xffffffff;
		uint c0 = graphics::Color::mulA(0xffffffff, a);
		ZomboDrawer::Default.fillRing(c0, xy.toVector3(), r, r*0.02f);
		ZomboDrawer::Default.fillRect(c0, xy.toVector3(), r*0.4f, rot);
	}

	void CurvesPoint::update(float scale, float alpha, float snapAlpha)
	{
		_scale.update();
		_scaleEx.update();
		_alpha.update();
		_rot.update();
		renderPoint(xy.get(), getR() * _scale.get()*scale, alpha, snapAlpha, _rot.get());
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

	geometry::Aabb2d CurvesPoint::getAabb() const
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

	CurvePointBinding::CurvePointBinding(): p(nullptr), _scale(1.0f), _rot(core::Math::Pi * 0.25f)
	{
	}

	CurvePointBinding::CurvePointBinding(CurvesPoint* point, const core::Vector2 &direction) : p(point), d(direction), _scale(1.0f), _rot(core::Math::Pi * 0.25f)
	{
	}

	bool CurvePointBinding::isUnderMouse() const
	{
		core::Vector2 m = CurvesManager::Default.mousePos;
		float mx = m.getX();
		float my = m.getY();
		float sx = getR();
		float sy = getR();
		core::Vector2 pos = p->xy.get()+d.get();
		float px = pos.getX();
		float py = pos.getY();
		return
			px - sx <= mx && mx <= px + sx &&
			py - sy <= my && my <= py + sy;
	}

	float CurvePointBinding::distanceToMouse() const
	{
		return (CurvesManager::Default.mousePos - p->xy.get() - d.get()).length();
	}

	void CurvePointBinding::updateRegularState()
	{
		setScale(1.0f);
		setRot(core::Math::Pi * 0.25f);
	}

	void CurvePointBinding::updateHoverState()
	{
		setScale(1.2f);
		setRot(core::Math::Pi * 0.5f);
	}

	core::Vector2 CurvePointBinding::getTargetXY() const
	{
		return p->getTargetXY() + d.getTarget();
	}

	core::Vector2 CurvePointBinding::getXY() const
	{
		return p->getXY() + d.get();
	}

	core::Vector2 CurvePointBinding::calcD(const core::Vector2& pos) const
	{
		core::Vector2 newD = pos - p->getTargetXY();
		if (newD.length() < trim)
		{
			newD = newD.normalize() * trim;
		}
		return newD;
	}

	void CurvePointBinding::setTargetXY(const core::Vector2& pos)
	{
		d.setTarget(calcD(pos));
	}

	void CurvePointBinding::setXY(const core::Vector2& pos)
	{
		d.setAll(calcD(pos));
	}

	void CurvePointBinding::updateSelectedState()
	{
		setScale(1.5f);
		setRot(core::Math::Pi * 1.25f);
	}

	float CurvePointBinding::getR()
	{
		return r;
	}

	void CurvePointBinding::updateCoords()
	{
		d.update();
	}

	void CurvePointBinding::update(float scale, float alpha)
	{
		_scale.update();
		_rot.update();
		renderExtraPoint(p->getXY() + d.get(), getR()*scale*_scale.get(), alpha, _rot.get());
		uint color = graphics::Color::mulA(0xffffffff, alpha * 0.5f);
		renderEdge(color, p->getXY(), getCurveP(), getR()*scale*_scale.get()*0.1f);
		core::Vector3 p0 = getCurveP().toVector3();
		core::Vector3 p1 = getArrowP().toVector3();
		ZomboDrawer::Default.fillArrow(color, p0, p1, r*scale*0.75f);
	}

	void CurvePointBinding::setScale(float a)
	{
		_scale.setTarget(a);
	}

	void CurvePointBinding::setRot(float rot)
	{
		_rot.setTarget(rot);
	}

	geometry::Aabb2d CurvePointBinding::getAabb() const
	{
		core::Vector2 pos = p->getXY() + d.get();
		return geometry::Aabb2d(pos - core::Vector2(getR(), getR()), pos + core::Vector2(getR(), getR()));
	}

	core::Vector2 CurvePointBinding::getCurveP() const
	{
		core::Vector2 dv = d.get();
		core::Vector2 dl = dv.length();
		return p->getXY() + dv.normalize() * (dl - trim);
	}

	core::Vector2 CurvePointBinding::getArrowP() const
	{
		core::Vector2 dv = d.get();
		core::Vector2 dl = dv.length();
		return p->getXY() + dv.normalize() * (dl - getR());
	}

	core::Vector2 CurveSegment::get0() const
	{
		return p0->p->getXY();
	}

	core::Vector2 CurveSegment::get1() const
	{
		return p0->getCurveP();
	}

	core::Vector2 CurveSegment::get2() const
	{
		return p1->getCurveP();
	}

	core::Vector2 CurveSegment::get3() const
	{
		return p1->p->getXY();
	}

	CurveSegment::CurveSegment(): color(0xffffffff), _scale(1.0f)
	{
		p0 = new CurvePointBinding();
		p1 = new CurvePointBinding();
		_scale.setConfig(&SConfig::Fast);
	}

	void CurveSegment::updateInput()
	{
	}

	void CurveSegment::updateCoords()
	{
		p0->updateCoords();
		p1->updateCoords();
	}

	uint segmentDetail = 16;
	float segnemtLen = 0.1f;

	void CurveSegment::update()
	{
		//renderPoint(p0->xy + d0, p0->_scale.getValue() * p0->getR() * 0.5f, p0->_alpha.getValue());
		//renderPoint(p1->xy + d1, p1->_scale.getValue() * p1->getR() * 0.5f, p1->_alpha.getValue());
		_scale.update();
		uint count = calcDetail();// segmentDetail;

		core::Vector2 ps = core::Vector2::cubic(get0(), get1(), get2(), get3(), 0);
		for (uint i = 1; i < count; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(count - 1);
			core::Vector2 pf = core::Vector2::cubic(get0(), get1(), get2(), get3(), u);
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
		return p0->d.get().length() + (p0->p->getXY() + p0->d.get() - p1->p->getXY() - p1->d.get()).length() + p1->d.get().length();
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
		core::Vector2 ps = core::Vector2::cubic(get0(), get1(), get2(), get3(), 0);
		for (uint i = 1; i < count; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(count - 1);
			core::Vector2 pf = core::Vector2::cubic(get0(), get1(), get2(), get3(), u);
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
		core::Vector2 ps = core::Vector2::cubic(get0(), get1(), get2(), get3(), 0);
		for (uint i = 1; i < count; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(count - 1);
			core::Vector2 pf = core::Vector2::cubic(get0(), get1(), get2(), get3(), u);
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
		bounds.expand(get0());
		bounds.expand(get1());
		bounds.expand(get2());
		bounds.expand(get3());
		return bounds;
	}

	void CurvesVisibleItems::clear()
	{
		points.clear();
		segments.clear();
		pointsBindings.clear();
	}


	bool CurvesSelection::isEmpty() const
	{
		return point == nullptr && segment == nullptr && binding == nullptr;
	}

	CurvesSelection::CurvesSelection(): point(nullptr), segment(nullptr), binding(nullptr)
	{
	}

	CurvesSelection::CurvesSelection(CurvesPoint* p) : point(p), segment(nullptr), binding(nullptr)
	{
	}

	CurvesSelection::CurvesSelection(CurveSegment* s): point(nullptr), segment(s), binding(nullptr)
	{
	}

	CurvesSelection::CurvesSelection(CurvePointBinding* b): point(nullptr), segment(nullptr), binding(b)
	{
	}

	CurvesManager CurvesManager::Default;

	CurvesManager::CurvesManager() : scale(1.0f), _snappingRange(0.1f), _pointsScale(1.0f), _extraPointsScale(1.0f)
	{
		_actualState = &CurvesStateSelect::Default;
		_extraPointsAlpha.setConfig(&SConfig::VerySlow);
		_pointsSnapAlpha.setConfig(&SConfig::VerySlow);
		_pointsAlpha.setConfig(&SConfig::VerySlow);
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

	void CurvesManager::snapBinding(const core::Vector2& p0, core::Vector2& p1)
	{
		//core::Vector2 d = p1 - p0;
		//float l = (p1 - p0).length() - CurvePointBinding::trim;
		//l = core::Math::round(l *0.1) * 10;
		//p1 = p0 + d.normalize() * (l + CurvePointBinding::trim);
	}

	void CurvesManager::update()
	{
		_pointsAlpha.update();
		_pointsSnapAlpha.update();
		_extraPointsAlpha.update();
		_pointsScale.update();
		_extraPointsScale.update();
		_pointsSnapAlpha.update();
		scale.update();
		for (uint i = 0; i < _points.size(); i++)
		{
			_points[i]->updateCoords();
		}
		for (uint i = 0; i < _segments.size(); i++)
		{
			_segments[i]->updateCoords();
		}

		mousePos = ZomboEditorCamera::Default.getMouseProjection(0);
		_visibleItems.clear();
		queryVisibleItems(_visibleItems);
		BindingSnapping::Default.update();
		PointSnapping::Default.update();
		if (_actualState != nullptr)
		{
			_actualState->update();
		}
		if (selection.isEmpty())
		{
			_pointsSnapAlpha.setTarget(1.0f);
			_extraPointsAlpha.setTarget(1.0f);
			_pointsAlpha.setTarget(1.0f);
		}
		else
		{
			if (selection.point != nullptr)
			{
				_pointsSnapAlpha.setTarget(1.0f);
			}
			else
			{
				_pointsSnapAlpha.setTarget(0.0f);
			}
			_extraPointsAlpha.setTarget(0.0f);
			_pointsAlpha.setTarget(0.0f);
		}
		for (uint i = 0; i < _visibleItems.points.size(); i++)
		{
			if(_visibleItems.points[i] == lastSelection.point)
			{
				_visibleItems.points[i]->update(_pointsScale.get(), 1.0f, 1.0f);
			}
			else
			{
				_visibleItems.points[i]->update(_pointsScale.get(), _pointsAlpha.get(), _pointsSnapAlpha.get());
			}
		}
		for (uint i = 0; i < _visibleItems.segments.size(); i++)
		{
			_visibleItems.segments[i]->update();
		}
		for (uint i = 0; i < _visibleItems.pointsBindings.size(); i++)
		{
			if (_visibleItems.pointsBindings[i] == lastSelection.binding)
			{
				_visibleItems.pointsBindings[i]->update(_extraPointsScale.get(), 1.0f);
			}
			else
			{				
				_visibleItems.pointsBindings[i]->update(_extraPointsScale.get(), _pointsAlpha.get());
			}
		}
	}
	
	void CurvesManager::addCurve(core::Vector2 p0, core::Vector2 p1, core::Vector2 p2, core::Vector2 p3)
	{
		CurvesPoint *cp0 = new CurvesPoint(p0);
		CurvesPoint *cp1 = new CurvesPoint(p3);
		core::Vector2 d0 = p1 - p0;
		core::Vector2 d1 = p2 - p3;
		CurveSegment *seg = new CurveSegment();
		seg->p0->p = cp0;
		seg->p1->p = cp1;
		seg->p0->d = d0;
		seg->p1->d = d1;
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
		CurvePointBinding * selectedBinding = nullptr;
		for (uint i = 0; i < items.pointsBindings.size(); i++)
		{
			if (items.pointsBindings[i]->isUnderMouse())
			{
				float ml = items.pointsBindings[i]->distanceToMouse();
				if (ml < l)
				{
					selectedBinding = items.pointsBindings[i];
					l = ml;
				}
			}
		}
		if (selectedBinding != nullptr)
		{
			return CurvesSelection(selectedBinding);
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
		
		float pxMin = 6;
		core::Vector2 size = CurvesPoint::size*scale;
		bool arePointsVisible = true;
		if (size.getX() < pxMin && size.getY() < pxMin)
		{
			_pointsScale.setTarget(0);
			if (_pointsScale.get() < 0.001f)
				arePointsVisible = false;
		}
		else
		{
			_pointsScale.setTarget(1);
		}


		if (arePointsVisible)
		{
			for (uint i = 0; i < _points.size(); i++)
			{
				geometry::Aabb2d aabb = _points[i]->getAabb();
				if (!frustum.include(aabb, 0)) {
					continue;
				}
				items.points.push_back(_points[i]);
			}
		}

		for (uint i = 0; i < _segments.size(); i++)
		{
			if (frustum.include(_segments[i]->getAabb(), 0))
			{
				items.segments.push_back(_segments[i]);
				
				if (size.getX() < pxMin && size.getY() < pxMin)
				{
					continue;
				}
			}
		}

		size = CurvePointBinding::size*scale;
		bool areExtraPointsVisible = true;
		if (size.getX() < pxMin && size.getY() < pxMin)
		{
			_extraPointsScale.setTarget(0);
			if (_extraPointsScale.get() < 0.001f)
				areExtraPointsVisible = false;
		}
		else
		{
			_extraPointsScale.setTarget(1);
		}
		if (areExtraPointsVisible)
		{
			for (uint i = 0; i < items.segments.size(); i++)
			{
				items.pointsBindings.push_back(items.segments[i]->p0);
				items.pointsBindings.push_back(items.segments[i]->p1);
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
