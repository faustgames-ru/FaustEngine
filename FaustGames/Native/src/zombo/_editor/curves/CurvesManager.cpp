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
#include "../ZomboEditor.h"
#include "../../common/ValuesAnimator.h"

namespace zombo
{
	float CurvesPoint::r(0.1f);
	core::Vector2 CurvesPoint::size(0.2f, 0.2f);
	float CurvePointBinding::r(0.1f);
	core::Vector2 CurvePointBinding::size(0.2f, 0.2f);
	float CurvePointBinding::trim(0.15f);
	//float CurvePointBinding::trim(0.0f);

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
		Animators::Vector2.animate(&_point->xy, _targetPosition);
	}

	void ZomboCommandMoveCurvePoint::undo()
	{
		ZomboLog::Default.m("Undo: Move point");
		Animators::Vector2.animate(&_point->xy, _prevPosition);
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
		_binding->setWorldP(_targetPosition);
		_binding->p->invalidateBindings(_binding);
		//_binding->setTargetXY(_targetPosition);
	}

	void ZomboCommandMoveCurveBinding::undo()
	{
		ZomboLog::Default.m("Undo: Move tangent");
		_binding->setWorldP(_prevPosition);
		_binding->p->invalidateBindings(_binding);
		//_binding->setTargetXY(_prevPosition);
	}

	core::Vector2 CurvesPoint::getXY() const
	{
		return xy;
	}

	core::Vector2 CurvesPoint::getTargetXY() const
	{
		return Animators::Vector2.getTarget(&xy);
	}

	CurvesPoint::CurvesPoint(): lock(CurvesPointLock::All)
	{
	}

	CurvesPoint::CurvesPoint(core::Vector2 p): lock(CurvesPointLock::All), xy(p)
	{
	}

	void CurvesPoint::removeBinding(CurvePointBinding* b)
	{
		for (uint i = bindings.size() - 1; i >= 0; i--)
		{
			if (bindings[i] == b)
			{
				for (uint j = i, size = bindings.size() - 1; j < size; j++)
					bindings[j] = bindings[j + 1];
				bindings.pop_back();
			}
		}
	}

	void CurvesPoint::invalidateBindings(CurvePointBinding* b)
	{	
		
		if (lock == CurvesPointLock::None) return;
		for (uint i = 0, size = bindings.size(); i < size; i++)
		{
			if (b == bindings[i]) continue;
			if (lock == CurvesPointLock::Direction)
			{
				float l = bindings[i]->d.length();
				bindings[i]->d = b->d.normalize() *-l;
			}
			else if (lock == CurvesPointLock::All)
			{
				bindings[i]->d = b->d *-1.0f;
			}
		}		
	}

	void CurvesPoint::invalidateBindingsLen(float l)
	{
		if (lock == CurvesPointLock::None) return;
		for (uint i = 0, size = bindings.size(); i < size; i++)
		{
			bindings[i]->d = bindings[i]->d.normalize() *l;
		}
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
		float sx = getR();
		float sy = getR();
		core::Vector2 p = xy;
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

	void renderEdge(uint c, core::Vector2 p0, core::Vector2 p1, float r)
	{		
		ZomboDrawer::Default.drawEdge(c, p0.toVector3(), p1.toVector3());
		//ZomboDrawer::Default.fillEdge(c, p0.toVector3(), p1.toVector3(), r);
		
		//ZomboDrawer::Default.fillCircle(c, p0.toVector3(), r);
		//ZomboDrawer::Default.fillCircle(c, p1.toVector3(), r);
	}

	void renderPoint(core::Vector2 xy, float r, float a, float a1, float rot)
	{
		if (!CurvesManager::Default.hasImages()) return;
		ZomboDrawer::Default.drawSprite(graphics::Color::mulA(0xffffffff, a), rot, r*2.0f, xy.toVector3(), CurvesManager::Default.getPointRingImage());
		ZomboDrawer::Default.drawSprite(graphics::Color::mulA(0xffffffff, a1), rot, r*2.0f, xy.toVector3(), CurvesManager::Default.getPointBoxImage());
	}
		
	void renderExtraPoint(core::Vector2 xy, float r, float a, float rot)
	{
		if (!CurvesManager::Default.hasImages()) return;
		ZomboDrawer::Default.drawSprite(graphics::Color::mulA(0xffffffff, a), rot, r*2.0f, xy.toVector3(), CurvesManager::Default.getPointRingImage());
		ZomboDrawer::Default.drawSprite(graphics::Color::mulA(0xffffffff, a), rot, r*2.0f, xy.toVector3(), CurvesManager::Default.getPointBoxImage());
	}


	geometry::Aabb2d CurvesPoint::getAabb() const
	{
		return geometry::Aabb2d(getXY() - core::Vector2(getR(), getR()), getXY() + core::Vector2(getR(), getR()));
	}

	CurvePointBinding::CurvePointBinding(): p(nullptr), segment(nullptr), snap(BindingSnap::None)
	{
	}

	CurvePointBinding::CurvePointBinding(CurvesPoint* point, const core::Vector2 &direction) : p(point), d(direction), segment(nullptr), snap(BindingSnap::None)
	{
	}

	core::Vector2 CurvePointBinding::getWorldP() const
	{
		core::Vector2 ex = calcTangent();// pp3 - pp0;
		core::Vector2 ey = ex.rotate90ccw();
		core::Matrix2 transform(ex.getX(), ex.getY(), ey.getX(), ey.getY());
		return p->xy + core::Matrix2::transform(transform, d);
	}

	core::Vector2 CurvePointBinding::getWorldD() const
	{
		core::Vector2 ex = calcTangent();// pp3 - pp0;
		core::Vector2 ey = ex.rotate90ccw();
		core::Matrix2 transform(ex.getX(), ex.getY(), ey.getX(), ey.getY());
		return core::Matrix2::transform(transform, d);
	}

	void CurvePointBinding::setWorldP(core::Vector2 wp)
	{
		core::Vector2 ex = calcTangent();// pp3 - pp0;
		core::Vector2 ey = ex.rotate90ccw();
		core::Matrix2 transform(ex.getX(), ex.getY(), ey.getX(), ey.getY());
		transform = transform.inverse();
		d = core::Matrix2::transform(transform, wp - p->getTargetXY());
	}

	core::Vector2 CurvePointBinding::getLocalP(core::Vector2 wp) const
	{
		core::Vector2 ex = calcTangent();// pp3 - pp0;
		core::Vector2 ey = ex.rotate90ccw();
		core::Matrix2 transform(ex.getX(), ex.getY(), ey.getX(), ey.getY());
		transform = transform.inverse();
		return core::Matrix2::transform(transform, wp - p->getTargetXY());
	}

	bool CurvePointBinding::isUnderMouse() const
	{
		core::Vector2 m = CurvesManager::Default.mousePos;
		float mx = m.getX();
		float my = m.getY();
		float sx = r;
		float sy = r;
		core::Vector2 pos = getWorldP();
		float px = pos.getX();
		float py = pos.getY();
		return
			px - sx <= mx && mx <= px + sx &&
			py - sy <= my && my <= py + sy;
	}
	
	float CurvePointBinding::distanceToMouse() const
	{
		return (CurvesManager::Default.mousePos - p->xy - d).length();
	}
	/*
		void CurvePointBinding::updateRegularState()
		{
			setScale(1.0f);
			setRot(0);
		}
	
		void CurvePointBinding::updateHoverState()
		{
			setScale(1.2f);
			setRot(core::Math::Pi * 0.25f);
		}
	
		core::Vector2 CurvePointBinding::getTargetXY() const
		{
			return p->getTargetXY() + Animators::Vector2.getTarget(&d);
		}
	
		core::Vector2 CurvePointBinding::getXY() const
		{
			return p->getXY() + d;
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
			Animators::Vector2.animate(&d, pos - p->getTargetXY());
		}
	
		void CurvePointBinding::setXY(const core::Vector2& pos)
		{
			Animators::Vector2.terminate(&d);
			d = pos - p->getTargetXY();
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
		*/
	void CurvePointBinding::update(float scale, float alpha)
	{
		
		//_scale.update();
		//_rot.update();
		core::Vector2 wp = getWorldP();
		renderExtraPoint(wp, r*scale, alpha, 0);
		uint color = graphics::Color::mulA(0xffffffff, alpha * 0.5f);
		renderEdge(color, p->getXY(), wp, r*scale*0.1f);
		//core::Vector3 p0 = getCurveP().toVector3();
		//core::Vector3 p1 = getArrowP().toVector3();
		//ZomboDrawer::Default.fillArrow(color, p0, p1, r*scale*0.75f);
		
	}

	core::Vector2 CurvePointBinding::calcTangent() const
	{
		return core::Vector2::axisX;
		//return getTangent();
		core::Vector2 tangent = getTangent();
		float l = tangent.length();
		tangent = tangent.normalize();
		core::Vector2 counterTangent = core::Vector2::empty;
		for (uint i = 0; i < p->bindings.size(); i++)
		{
			if (p->bindings[i] == this) continue;
			tangent -= p->bindings[i]->getTangent().normalize();
		}
		tangent = tangent.normalize();
		return tangent*l;
	}

	core::Vector2 CurvePointBinding::getTangent() const
	{
		CurvePointBinding* other = segment->p0 == this ? segment->p1 : segment->p0;
		return other->p->xy - p->xy;
	}

	CurveSegmentRenderer::CurveSegmentRenderer()
	{
		image = nullptr;
	}

	CurveSegmentRenderer CurveSegmentRenderer::Default;


	void offset(core::Vector2 d1, core::Vector2 d2, core::Vector2 &p0, core::Vector2 &p1, core::Vector2 &p2, core::Vector2 &p3, float w0, float w1)
	{
		core::Vector2 n0 = d1/*(p1 - p0)*/.rotate90cw().normalize();
		core::Vector2 n1 = (p2 - p1).rotate90cw().normalize();
		core::Vector2 n2 = (d2 *-1.0f)/*(p3 - p2)*/.rotate90cw().normalize();
		if (n0.isEmpty())
			n0 = n1;
		if (n2.isEmpty())
			n2 = n1;
		if (n1.isEmpty())
			n1 = (n2 + n0).normalize();

		float dot0 = core::Vector2::dotProduct(n0, n1);
		float dot1 = core::Vector2::dotProduct(n1, n2);

		core::Vector2 n01;
		if (core::Math::equals(dot0, -1))
		{
			n01 = n0;
		}
		else
		{
			float q0 = 1.0f / (1.0f + dot0);
			n01 = (n0 + n1) * q0;
		}

		core::Vector2 n12;
		if (core::Math::equals(dot0, -1))
		{
			n12 = n2;
		}
		else
		{
			float q1 = 1.0f / (1.0f + dot1);
			n12 = (n1 + n2) * q1;
		}


		float w = (w0 + w1) *0.5f;
		p0 += n0 * w0;
		p1 += n01 * w;
		p2 += n12 * w;
		p3 += n2 * w1;
		/*
		float q0 = 1.0f / (1.0f + core::Vector2::dotProduct(n0, n1));
		float q1 = 1.0f / (1.0f + core::Vector2::dotProduct(n1, n2));


		core::Vector2 n01 = (n0 + n1) * q0;
		core::Vector2 n12 = (n1 + n2) * q1;
		float w = (w0 + w1) *0.5f;
		p0 += n0 * w0;
		p1 += n01 * w;
		p2 += n12 * w;
		p3 += n2 * w1;
		*/
	}

	struct curve
	{
		core::Vector2 p0;
		core::Vector2 p1;
		core::Vector2 p2;
		core::Vector2 p3;
	};

	uint calcDetail(CurveSegment* s, float segnemtLen)
	{
		uint r = static_cast<uint>(core::Math::round(s->getBaseLen() / segnemtLen));
		if (r < 2)
			r = 2;
		/*
		if (r < 6)
			r = 6;
		if (r > 64)
			r = 64;
		*/
		return r;
		/*
		uint r = static_cast<uint>(core::Math::round(getBaseLen() / segnemtLen));
		if (r < 6)
			r = 6;
		if (r > 64)
			r = 64;
		return r;
		*/
	}

	geometry::Aabb2d CurveSegmentRenderer::getBounds(const CurveSegment* s) const
	{
		if (image == nullptr)
		{
			return geometry::Aabb2d(core::Vector2::empty, core::Vector2::empty);
		}
		core::Vector2 pp0 = s->get0();
		core::Vector2 pp3 = s->get3();
		core::Vector2 pp1 = s->p0->getWorldP();
		core::Vector2 pp2 = s->p1->getWorldP();
		
		curve t;
		t.p0 = pp0;
		t.p1 = pp1;
		t.p2 = pp2;
		t.p3 = pp3;

		curve b;
		b.p0 = pp0;
		b.p1 = pp1;
		b.p2 = pp2;
		b.p3 = pp3;

		core::Vector2 size = image->bounds.Max - image->bounds.Min;
		float w = size.getX();
		float h = size.getY() * 0.5f;

		offset(s->p0->getWorldD(), s->p1->getWorldD(), t.p0, t.p1, t.p2, t.p3, h, h);
		offset(s->p0->getWorldD(), s->p1->getWorldD(), b.p0, b.p1, b.p2, b.p3, -h, -h);

		geometry::Aabb2d result;
		result.expand(t.p0);
		result.expand(t.p1);
		result.expand(t.p2);
		result.expand(t.p3);
		result.expand(b.p0);
		result.expand(b.p1);
		result.expand(b.p2);
		result.expand(b.p3);
		return result;
	}

	void CurveSegmentRenderer::renderTile(CurveSegment* s)
	{
		if (image == nullptr) return;
		core::Vector2 pp0 = s->get0();
		core::Vector2 pp3 = s->get3();
		core::Vector2 pp1 = s->p0->getWorldP();
		core::Vector2 pp2 = s->p1->getWorldP();;

		curve t;
		t.p0 = pp0;
		t.p1 = pp1;
		t.p2 = pp2;
		t.p3 = pp3;

		curve b;
		b.p0 = pp0;
		b.p1 = pp1;
		b.p2 = pp2;
		b.p3 = pp3;
		
		core::Vector2 size = image->bounds.Max - image->bounds.Min;
		float w = size.getX();
		float h = size.getY() * 0.5f;

		offset(s->p0->getWorldD(), s->p1->getWorldD(), t.p0, t.p1, t.p2, t.p3, h, h);
		offset(s->p0->getWorldD(), s->p1->getWorldD(), b.p0, b.p1, b.p2, b.p3, -h, -h);

		float count = calcDetail(s, w);
		uint color = 0xffffffff;
		for (uint i = 1; i < count; i++)
		{
			float u0 = static_cast<float>(i-1) / static_cast<float>(count - 1);;
			float u1 = static_cast<float>(i) / static_cast<float>(count - 1);;
			_vertices.clear();
			for (uint j = 0; j < image->vertices.size(); j++)
			{
				core::Vector2 xy = image->vertices[j].xy;
				float u = core::Math::lerp(u0, u1, (xy.getX() - image->bounds.Min.getX()) / size.getX());
				core::Vector2 txy = core::Vector2::cubic(t.p0, t.p1, t.p2, t.p3, u);
				core::Vector2 bxy = core::Vector2::cubic(b.p0, b.p1, b.p2, b.p3, u);
				core::Vector2 pos = core::Vector2::lerp(bxy, txy, (xy.getY() - image->bounds.Min.getY()) / size.getY());
				
				_vertices.push_back(RenderVertex(pos.toVector3(), color, image->vertices[j].u, image->vertices[j].v));
			}
			if (_vertices.size() > 0)
			{
				ZomboEditorRenderService::Default.drawTrianglesTextured(image->texture,
					_vertices.data(), _vertices.size(), image->indices.data(), image->indices.size() / 3);
			}
		}

		core::Vector2 pst = core::Vector2::cubic(t.p0, t.p1, t.p2, t.p3, 0);
		core::Vector2 psb = core::Vector2::cubic(b.p0, b.p1, b.p2, b.p3, 0);
		core::Vector2 ps = core::Vector2::cubic(pp0, pp1, pp2, pp3, 0);
		/*
		renderEdge(color, pst, psb, 1);
		for (uint i = 1; i < count; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(count - 1);
			core::Vector2 pft = core::Vector2::cubic(t.p0, t.p1, t.p2, t.p3, u);
			core::Vector2 pfb = core::Vector2::cubic(b.p0, b.p1, b.p2, b.p3, u);
			core::Vector2 pf = core::Vector2::cubic(pp0, pp1, pp2, pp3, u);
			//renderEdge(color, ps, pf, getR()*_scale.get());
			renderEdge(color, pst, pft, 1);
			renderEdge(color, psb, pfb, 1);
			renderEdge(0x80808080, pft, pfb, 1);

			pst = pft;
			psb = pfb;
			ps = pf;
		}
		renderEdge(color, pst, psb, 1);
		*/
	}

	/*
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
			core::Vector2 pos = p->getXY() + d;
			return geometry::Aabb2d(pos - core::Vector2(getR(), getR()), pos + core::Vector2(getR(), getR()));
		}
	
		core::Vector2 CurvePointBinding::getCurveP() const
		{
			core::Vector2 dv = d;
			float dl = dv.length();
			if (core::Math::abs(dl- trim) < 0.001f)
				return p->getXY();
			return p->getXY() + dv.normalize() * (dl - trim);
		}
	
		core::Vector2 CurvePointBinding::getCurveTargetP() const
		{
			core::Vector2 dv = Animators::Vector2.getTarget(&d);
			float dl = dv.length();
			if (core::Math::abs(dl - trim) < 0.001f)
				return p->getXY();
			return p->getXY() + dv.normalize() * (dl - trim);
		}
	
		core::Vector2 CurvePointBinding::getArrowP() const
		{
			core::Vector2 dv = d;
			core::Vector2 dl = dv.length();
			return p->getXY() + dv.normalize() * (dl - getR());
		}
		*/
	core::Vector2 CurveSegment::get0() const
	{
		return p0->p->getXY();
	}

	core::Vector2 CurveSegment::get1() const
	{
		//return p0->getCurveP();
		return get0() + p0->d;
	}

	core::Vector2 CurveSegment::get2() const
	{
		//return p1->getCurveP();
		return get3() + p1->d;
	}

	core::Vector2 CurveSegment::get3() const
	{
		return p1->p->getXY();
	}

	CurveSegment::CurveSegment(): color(0xfffffffff), _scale(1.0f)
	{
		p0 = new CurvePointBinding();
		p1 = new CurvePointBinding();
		p0->segment = this;
		p1->segment = this;
		_scale.setConfig(&SConfig::Fast);
	}

	CurveSegment::~CurveSegment()
	{
		if (p0->p != nullptr)
		{
			p0->p->removeBinding(p0);
		}
		if (p1->p != nullptr)
		{
			p1->p->removeBinding(p1);
		}
		delete p0;
		delete p1;
	}

	void CurveSegment::updateInput()
	{
	}

	void CurveSegment::updateCoords()
	{
	}

	uint segmentDetail = 16;
	float segnemtLen = 0.1f;


	void offset(core::Vector2 &p0, core::Vector2 &p1, core::Vector2 &p2, core::Vector2 &p3, float d)
	{
		core::Vector2 n0 = (p1 - p0).rotate90cw();
		core::Vector2 n1 = (p2 - p1).rotate90cw();
		core::Vector2 n2 = (p3 - p2).rotate90cw();
		if (n0.isEmpty(1e-6))
			n0 = n1;
		if (n2.isEmpty(1e-6))
			n2 = n1;
		if (n1.isEmpty(1e-6))
			n1 = (n2 + n0);

		n0 = n0.normalize();
		n1 = n1.normalize();
		n2 = n2.normalize();

		float dot0 = core::Vector2::dotProduct(n0, n1);
		float dot1 = core::Vector2::dotProduct(n1, n2);

		core::Vector2 n01;
		if (core::Math::equals(dot0, -1))
		{
			n01 = n0;
		}
		else
		{
			float q0 = 1.0f / (1.0f + dot0);
			n01 = (n0 + n1) * q0;
		}

		core::Vector2 n12;
		if (core::Math::equals(dot0, -1))
		{
			n12 = n2;
		}
		else
		{
			float q1 = 1.0f / (1.0f + dot1);
			n12 = (n1 + n2) * q1;
		}
			

		p0 += n0 * d;
		p1 += n01 * d;
		p2 += n12 * d;
		p3 += n2 * d;
	}
	
	void CurveSegment::update()
	{
		CurveSegmentRenderer::Default.renderTile(this);
		return;
		//renderPoint(p0->xy + d0, p0->_scale.getValue() * p0->getR() * 0.5f, p0->_alpha.getValue());
		//renderPoint(p1->xy + d1, p1->_scale.getValue() * p1->getR() * 0.5f, p1->_alpha.getValue());
		_scale.update();
		uint count = calcDetail();// segmentDetail;
		
		core::Vector2 pp0 = get0();
		core::Vector2 pp3 = get3();
		
		core::Vector2 pp1 = p0->getWorldP();
		core::Vector2 pp2 = p1->getWorldP();;

		// todo: remove crutch
		/*
		if (core::Vector2::equals(pp0, p0->getCurveTargetP()))
		{
			p0->setTargetXY(pp0 + (pp2 - pp0).normalize()*CurvePointBinding::trim);
		}
		if (core::Vector2::equals(pp3, p1->getCurveTargetP()))
		{
			p1->setTargetXY(pp3 + (pp1 - pp3).normalize()*CurvePointBinding::trim);
		}
		*/

		core::Vector2 ppt0 = pp0;
		core::Vector2 ppt1 = pp1;
		core::Vector2 ppt2 = pp2;
		core::Vector2 ppt3 = pp3;

		core::Vector2 ppb0 = pp0;
		core::Vector2 ppb1 = pp1;
		core::Vector2 ppb2 = pp2;
		core::Vector2 ppb3 = pp3;
		
		offset(p0->getWorldD(), p1->getWorldD(), ppt0, ppt1, ppt2, ppt3, p0->d.length(), p1->d.length());
		offset(p0->getWorldD(), p1->getWorldD(), ppb0, ppb1, ppb2, ppb3, -p0->d.length(), -p1->d.length());
		/*
		offset(ppt0, ppt1, ppt2, ppt3, 0.25f);
		offset(ppb0, ppb1, ppb2, ppb3, -0.25f);
		*/
		core::Vector2 pst = core::Vector2::cubic(ppt0, ppt1, ppt2, ppt3, 0);
		core::Vector2 psb = core::Vector2::cubic(ppb0, ppb1, ppb2, ppb3, 0);
		core::Vector2 ps = core::Vector2::cubic(pp0, pp1, pp2, pp3, 0);		

		renderEdge(color, pst, psb, getR()*_scale.get());
		for (uint i = 1; i < count; i++)
		{
			float u = static_cast<float>(i) / static_cast<float>(count - 1);
			core::Vector2 pft = core::Vector2::cubic(ppt0, ppt1, ppt2, ppt3, u);
			core::Vector2 pfb = core::Vector2::cubic(ppb0, ppb1, ppb2, ppb3, u);
			core::Vector2 pf = core::Vector2::cubic(pp0, pp1, pp2, pp3, u);
			//renderEdge(color, ps, pf, getR()*_scale.get());
			renderEdge(color, pst, pft, getR()*_scale.get());
			renderEdge(color, psb, pfb, getR()*_scale.get());
			renderEdge(0x80808080, pft, pfb, getR()*_scale.get());

			pst = pft;
			psb = pfb;
			ps = pf;
		}
		renderEdge(color, pst, psb, getR()*_scale.get());
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
		return p0->d.length() + (p0->p->getXY() + p0->d - p1->p->getXY() - p1->d).length() + p1->d.length();
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
		return CurveSegmentRenderer::Default.getBounds(this);
		/*
		geometry::Aabb2d bounds;
		bounds.expand(get0());
		bounds.expand(get1());
		bounds.expand(get2());
		bounds.expand(get3());
		return bounds;
		*/
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

	CurvesPointAnimator::CurvesPointAnimator(CurvesPoint* p): point(p), scale(1.0f), rotation(0.0f)
	{
		scale.setConfig(&SConfig::Fast);
	}

	bool CurvesPointAnimator::isAnimating() const
	{
		return scale.isUpdating() || rotation.isUpdating();
	}

	void CurvesPointAnimator::update()
	{
		scale.update();
		rotation.update();
	}

	CurvesPointState CurvesPointState::Regular(1.0f, 0.0f);
	CurvesPointState CurvesPointState::Hovered(1.2f, core::Math::Pi * 0.25f);
	CurvesPointState CurvesPointState::Selected(1.5f, core::Math::Pi);

	CurvesPointState::CurvesPointState(): scale(1.0f), rotation(0.0f)
	{
	}

	CurvesPointState::CurvesPointState(float s, float rot): scale(s), rotation(rot)
	{
	}

	CurvesManager CurvesManager::Default;

	CurvesManager::CurvesManager() : scale(1.0f), _snappingRange(0.1f), _pointsScale(1.0f), _extraPointsScale(1.0f), _pointBoxImage(nullptr), _pointRingImage(nullptr), _regularPointR(1.0f)
	{
		_actualState = &CurvesStateSelect::Default;
		_extraPointsAlpha.setConfig(&SConfig::Default);
		_pointsSnapAlpha.setConfig(&SConfig::Default);
		_pointsAlpha.setConfig(&SConfig::Default);
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

	bool CurvesManager::hasImages()
	{
		return _pointBoxImage != nullptr && _pointRingImage != nullptr;
	}

	void CurvesManager::snapBinding(const core::Vector2& p0, core::Vector2& p1)
	{
		//core::Vector2 d = p1 - p0;
		//float l = (p1 - p0).length() - CurvePointBinding::trim;
		//l = core::Math::round(l *0.1) * 10;
		//p1 = p0 + d.normalize() * (l + CurvePointBinding::trim);
	}

	void CurvesManager::load()
	{
		_pointBoxImage = ZomboEditor::Default.internalContent.getImage(ZomboEditorConstants::PointBoxImage.c_str());
		_pointRingImage = ZomboEditor::Default.internalContent.getImage(ZomboEditorConstants::PointRingImage.c_str());
	}

	void CurvesManager::regularPointRender(CurvesPoint* point) const
	{
		renderPoint(point->xy, _regularPointR, _pointsAlpha.get(), _pointsSnapAlpha.get(), 0);
	}

	void CurvesManager::animatePointRender(CurvesPointAnimator* animator) const
	{
		renderPoint(animator->point->xy, _regularPointR * animator->scale.get(), 1.0f, 1.0f, animator->rotation.get());
	}

	void CurvesManager::update()
	{
		_pointsAlpha.update();
		_pointsSnapAlpha.update();
		_extraPointsAlpha.update();
		_pointsScale.update();
		_extraPointsScale.update();
		_pointsSnapAlpha.update();
		scale.update();/*
		for (uint i = 0; i < _points.size(); i++)
		{
			_points[i]->updateCoords();
		}
		for (uint i = 0; i < _segments.size(); i++)
		{
			_segments[i]->updateCoords();
		}*/

		mousePos = ZomboEditorCamera::Default.getMouseProjection(0);
		_visibleItems.clear();
		queryVisibleItems(_visibleItems);
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

		for (uint i = 0; i < _visibleItems.segments.size(); i++)
		{
			_visibleItems.segments[i]->update();
		}

		_regularPointR = CurvesPoint::getR() * _pointsScale.get();
		if (_pointsAnimationMap.empty())
		{
			for (uint i = 0; i < _visibleItems.points.size(); i++)
			{
				regularPointRender(_visibleItems.points[i]);
			}
		}
		else
		{
			for (uint i = 0; i < _visibleItems.points.size(); i++)
			{
				PointsAnimationMap::iterator it = _pointsAnimationMap.find(_visibleItems.points[i]);
				if (it == _pointsAnimationMap.end())
				{
					regularPointRender(_visibleItems.points[i]);
				}
			}
			for (PointsAnimationMap::iterator j = _pointsAnimationMap.begin(); j != _pointsAnimationMap.end(); ++j)
			{
				j->second->update();
				if (isRegular(j->second))
				{
					_removeAnimationList.push_back(j->first);
				}
				animatePointRender(j->second);
			}
		}
		for (uint i = 0; i < _removeAnimationList.size(); i++)
		{
			PointsAnimationMap::iterator it = _pointsAnimationMap.find(_removeAnimationList[i]);
			if (it != _pointsAnimationMap.end())
			{
				delete it->second;
				_pointsAnimationMap.erase(it);
			}
		}
		_removeAnimationList.clear();
		/*
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
		*/
		
		for (uint i = 0; i < _visibleItems.pointsBindings.size(); i++)
		{
			_visibleItems.pointsBindings[i]->update(_extraPointsScale.get(), 1.0f);
			/*
			if (_visibleItems.pointsBindings[i] == lastSelection.binding)
			{
				_visibleItems.pointsBindings[i]->update(_extraPointsScale.get(), 1.0f);
			}
			else
			{				
				_visibleItems.pointsBindings[i]->update(_extraPointsScale.get(), _pointsAlpha.get());
			}
			*/
		}
		BindingSnapping::Default.update();
		PointSnapping::Default.update();
	}

	void CurvesManager::addCurve(core::Vector2 p0, core::Vector2 p3)
	{
		CurvesPoint *cp0 = new CurvesPoint(p0);
		CurvesPoint *cp1 = new CurvesPoint(p3);
		core::Vector2 d0 = core::Vector2::axisX*0.25f + core::Vector2::axisY*0.25f;
		core::Vector2 d1 = core::Vector2::axisX*0.25f - core::Vector2::axisY*0.25f;
		CurveSegment *seg = new CurveSegment();
		seg->p0->p = cp0;
		seg->p1->p = cp1;
		seg->p0->d = d0;
		seg->p1->d = d1;
		cp0->bindings.push_back(seg->p0);
		cp1->bindings.push_back(seg->p1);
		_points.push_back(cp0);
		_points.push_back(cp1);
		_segments.push_back(seg);
	}

	void CurvesManager::addCurve(const std::vector<core::Vector2> &p, bool close)
	{
		if (p.size() == 0) return;
		CurvesPoint *cp0;
		int start;
		CurvesPoint *last = new CurvesPoint(p.back());
		if (close)
		{
			cp0 = last;
			start = 0;
		}
		else
		{
			cp0 = new CurvesPoint(p.front());
			start = 1;
		}
		for (int i = start; i < p.size(); i++)
		{
			CurvesPoint *cp1;
			if (i == p.size() - 1)
			{
				cp1 = last;
			}
			else
			{
				cp1 = new CurvesPoint(p[i]);
			}
			core::Vector2 d0 = core::Vector2::axisX *0.25f;
			core::Vector2 d1 = core::Vector2::axisX *-0.25f;
			CurveSegment *seg = new CurveSegment();
			seg->p0->p = cp0;
			seg->p1->p = cp1;
			seg->p0->d = d0;
			seg->p1->d = d1;
			cp0->bindings.push_back(seg->p0);
			cp1->bindings.push_back(seg->p1);
			_points.push_back(cp0);
			_points.push_back(cp1);
			_segments.push_back(seg);
			cp0 = cp1;
		}
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

	ZomboContentImage* CurvesManager::getPointRingImage() const
	{
		return _pointRingImage;
	}

	ZomboContentImage* CurvesManager::getPointBoxImage() const
	{
		return _pointBoxImage;
	}

	bool CurvesManager::isRegular(CurvesPointAnimator* animator) const
	{
		if (animator->isAnimating()) return false;
		return
			core::Math::equals(animator->scale.getTarget(), CurvesPointState::Regular.scale) &&
			core::Math::equals(animator->rotation.getTarget(), CurvesPointState::Regular.rotation);
	}

	void CurvesManager::animateToState(CurvesPoint* point, CurvesPointState* state)
	{
		CurvesPointAnimator* animator = findAnimator(point);
		animator->scale.setTarget(state->scale);
		animator->rotation.setTarget(state->rotation);
	}

	void CurvesManager::animateToRegular(CurvesPoint* point)
	{
		PointsAnimationMap::iterator it = _pointsAnimationMap.find(point);
		if (it == _pointsAnimationMap.end()) return;
		CurvesPointAnimator *animator = it->second;
		animator->scale.setTarget(CurvesPointState::Regular.scale);
		animator->rotation.setTarget(CurvesPointState::Regular.rotation);
	}

	void CurvesManager::animateToHover(CurvesPoint* point)
	{
		animateToState(point, &CurvesPointState::Hovered);
	}

	void CurvesManager::animateToSelect(CurvesPoint* point)
	{
		animateToState(point, &CurvesPointState::Selected);
	}

	CurvesPointAnimator* CurvesManager::findAnimator(CurvesPoint* point)
	{
		PointsAnimationMap::iterator it = _pointsAnimationMap.find(point);
		CurvesPointAnimator *animator;
		if (it != _pointsAnimationMap.end())
		{
			animator = it->second;
		}
		else
		{
			_pointsAnimationMap[point] = animator = new CurvesPointAnimator(point);
		}
		return animator;
	}

	void CurvesManager::queryVisibleItems(CurvesVisibleItems& items)
	{
		// todo: use geometry tree to optimise query

		geometry::Frustum frustum = ZomboEditorCamera::Default.frustum;
		float scale = ZomboEditorViewport::Default.h * 0.5f / ZomboEditorCamera::Default.getInterpoaltedScale();
		segnemtLen = 8 / scale;
		// todo:: optimize;
		
		float pxMin = 12;
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
