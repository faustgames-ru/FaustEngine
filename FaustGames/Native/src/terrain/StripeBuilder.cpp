#include "StripeBuilder.h"
#include "../core/DebugRender.h"
#include "../core/Spline.h"

namespace terrain
{
	StripePoint StripePoint::offset(float dx, float dy)
	{
		StripePoint result = *this;
		result.x += dx;
		result.y += dy;
		return result;
	}

	StripePoint StripePoint::offset(core::Vector2 d)
	{
		StripePoint result = *this;
		result.x += d.getX();
		result.y += d.getY();
		return result;
	}

	StripePoint StripePoint::copy(core::Vector2 p)
	{
		StripePoint result = *this;
		result.x = p.getX();
		result.y = p.getY();
		return result;
	}

	core::Vector2 StripePoint::position()
	{
		return core::Vector2(x, y);
	}

	StripeWidth::StripeWidth(): out(1.0f), in(1.0f), width(1.0f)
	{
	}

	StripeWidth::StripeWidth(float o, float i, float w) : out(o), in(i), width(w)
	{
	}

	StripeConfig::StripeConfig(): 
		AllowedTiles(StripeType::Platform), 
		PhysicsMode(llge::PhysicsAll),
		DetailX(2),
		flipReversed(true)
	{
		left = StripeWidth(16, 32 + 16, 128);
		right = StripeWidth(16, 32 + 16, 128);
		top = StripeWidth(16, 32 + 16, 128);
		bottom = StripeWidth(16, 32 + 16, 128);
	}

	StripeWidth StripeConfig::getWidth(core::Direction2::e d)
	{
		if ((AllowedTiles & StripeType::TileAngular) == 0)
		{
			return top;
		}
		switch (d)
		{
		case core::Direction2::Left: 
			return bottom;
		case core::Direction2::Right: 
			return top;
		case core::Direction2::Top: 
			return left;
		case core::Direction2::Bottom:
			return right;
		default: 
			return top;
		}
	}

	TileStripeType::e StripeConfig::getType(core::Direction2::e d)
	{
		if ((AllowedTiles & StripeType::TileAngular) == 0)
		{
			return TileStripeType::TileTop;
		}
		switch (d)
		{
		case core::Direction2::Left:
			return TileStripeType::TileBottom;
		case core::Direction2::Right:
			return TileStripeType::TileTop;
		case core::Direction2::Top:
			return TileStripeType::TileLeft;
		case core::Direction2::Bottom:
			return TileStripeType::TileRight;
		default:
			return TileStripeType::TileTop;
		}
	}

	void StripeConfig::set(llge::TerrainStripeConfigType type, float inWidth, float outWidth, float width)
	{
		StripeWidth *w;
		switch (type)
		{
		case llge::Left: 
			w = &left;
			break;
		case llge::Right: 
			w = &right;
			break;
		case llge::Top: 
			w = &top;
			break;
		case llge::Bottom: 
			w = &bottom;
			break;
		default: 
			return;
		}
		w->out = outWidth;
		w->in = inWidth;
		w->width = width;
	}

	void StripeConfig::setPhysicsMode(llge::TerrainStripePhysicsType mode)
	{
		PhysicsMode = mode;
	}

	void StripeConfig::setFlipReversed(bool value)
	{
		flipReversed = value;
	}

	void StripeConfig::setTilesConfig(uint value)
	{
		AllowedTiles = static_cast<StripeType::e>(value);
	}

	class Stripe
	{
	public:
		StripeWidth w;
		TileStripeType::e type;
		TileStripeType::e prevType;
		TileStripeType::e nextType;
		core::Vector2 prevD;
		core::Vector2 nextD;
		StripeListPoint* start;
		StripeListPoint* finish;
		float lt;
		float rt;
		float lb;
		float rb;
		core::MacroSpline2* t;
		core::MacroSpline2* b;
	};


	void StripeBuilder::build(const StripePoint* vertices, float tension, int count, bool closed)
	{
		_dedugRender.clear();
		_polyline.clear();
		_edges.clear();
		_tiles.clear();
		_indices.clear();
		_vertices.clear();

		if (count < 2) return;
		initQuad1Indices();
		initQuadIndices(config.DetailX);
		initFanIndices(config.DetailX);
		initFan2Indices(config.DetailX * 2);

		StripeListPoint *points = createStripeList(vertices, count, closed);

		StripePoint p0, p1, p2, p3, tp0, tp1, tp2, tp3, bp0, bp1, bp2, bp3;
		StripeListPoint *current = points;
		int edgesCount = count;
		if (!closed)
		{
			edgesCount--;
		}

		std::vector<Stripe*> stripes;
		Stripe* first = nullptr;
		Stripe* s = nullptr;
		TileStripeType::e _prevType = TileStripeType::None;
		if (!closed)
		{
			_polyline.push_back(current->point.position());
		}
		for (int i = 0; i < edgesCount; i++)
		{
			StripeListPoint *lp0 = current->prev;
			StripeListPoint *lp1 = current;
			StripeListPoint *lp2 = current->next;
			StripeListPoint *lp3 = current->next->next;
			p0 = lp0->point;
			p1 = lp1->point;
			p2 = lp2->point;
			p3 = lp3->point;
			
			float d1x = p2.x - p1.x;
			float d1y = p2.y - p1.y;
			float l1 = core::Math::sqrt(d1x*d1x + d1y*d1y);

			core::Vector2 d(d1x, d1y);
			StripeWidth w = config.getWidth(d.direction());
			TileStripeType::e type = config.getType(d.direction());

			tp0 = lp0->pt;
			tp1 = lp1->pt;
			tp2 = lp2->pt;
			tp3 = lp3->pt;

			bp0 = lp0->pb;
			bp1 = lp1->pb;
			bp2 = lp2->pb;
			bp3 = lp3->pb;
			int tilesCount = static_cast<int>(core::Math::round(l1 / w.width)) * config.DetailX;
			
			core::Spline2 spline(
				p0.x, p0.y, p0.t * tension,
				p1.x, p1.y, p1.t * tension,
				p2.x, p2.y, p2.t * tension,
				p3.x, p3.y, p3.t * tension, tilesCount * 2);
			for (uint j = 0; j < tilesCount; j++)
			{
				float l = static_cast<float>(j + 1)*spline.length() / static_cast<float>(tilesCount);
				float u = spline.getU(l);
				_polyline.push_back(spline.get(u));
			}

			if(_prevType != type)
			{
				if (s != nullptr)
				{
					s->nextType = type;
				}
				s = new Stripe();
				if (first == nullptr)
					first = s;
				s->lt = lp1->tl;
				s->lb = lp1->bl;
				s->type = type;
				s->prevType = _prevType;
				s->w = w;
				s->t = new core::MacroSpline2();
				s->b = new core::MacroSpline2();
				s->nextD = lp1->nextD;
				s->start = lp1;
				stripes.push_back(s);
				_prevType = type;
			}

			s->t->addSpline(
				tp0.x, tp0.y, p0.t * tension,
				tp1.x, tp1.y, p1.t * tension,
				tp2.x, tp2.y, p2.t * tension,
				tp3.x, tp3.y, p3.t * tension, tilesCount * 2);
			s->b->addSpline(
				bp0.x, bp0.y, p0.t * tension,
				bp1.x, bp1.y, p1.t * tension,
				bp2.x, bp2.y, p2.t * tension,
				bp3.x, bp3.y, p3.t * tension, tilesCount * 2);
			
			s->rt = lp2->tr;
			s->rb = lp2->br;
			s->prevD = lp2->prevD;
			s->finish = lp2;

			current = current->next;
		}

		if (s != first && closed)
		{
			s->nextType = first->type;
			first->prevType = s->type;
		}

		for (uint i = 0; i < stripes.size(); i++)
		{
			s = stripes[i];
			core::Vector2 pt0 = s->t->get(s->t->getU(s->lt));
			core::Vector2 pb0 = s->b->get(s->b->getU(s->lb));

			if (s->lt > 0.0001f && s->lb > 0.0001f)
			{
				// build cap				
				core::Vector2 dir = s->nextD.normalize();
				core::MSplineU ut = s->t->getU(s->lt);
				core::MSplineU ub = s->b->getU(s->lb);
				pt0 = s->t->get(ut);
				pb0 = s->b->get(ub);

				addQuad(TileStripeType::CapLeft,
					s->start->pb.position() - dir*s->start->br,
					s->start->pt.position() - dir*s->start->tr,
					pt0, pb0);
			}
			else if (s->lt > 0.0001f)
			{
				SegmentConfig seg;
				seg.centerPosition = s->start->pb.position();
				seg.startPosition = s->start->pt.position();
				float len = s->lt;
				seg.statLen = 0;
				seg.finishLen = len;
				seg.spline = s->t;
				if (s->prevType == TileStripeType::TileTop)
				{
					if (s->type == TileStripeType::TileRight)
					{
						seg.type = TileStripeType::AngularTopToRight;
						seg.centerTexture = core::Vector2(0.0f, 1.0f);
						seg.startTexture = core::Vector2(1.0f, 0.0f);
						seg.finishTexture = core::Vector2(1.0f, 1.0f);
						pt0 = buildSegment(seg, false);
					}
					else if (s->type == TileStripeType::TileBottom)
					{
						seg.type = TileStripeType::AngularRightToBottom;
						seg.centerTexture = core::Vector2(0.0f, 0.0f);
						seg.startTexture = core::Vector2(1.0f, 0.0f);
						seg.finishTexture = core::Vector2(1.0f, 1.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, false);
						seg.startTexture = core::Vector2(1.0f, 1.0f);
						seg.finishTexture = core::Vector2(0.0f, 1.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						pt0 = buildSegment(seg, false);
					}
				}
				else if (s->prevType == TileStripeType::TileLeft)
				{
					if (s->type == TileStripeType::TileTop)
					{
						seg.type = TileStripeType::AngularLeftToTop;
						seg.centerTexture = core::Vector2(1.0f, 1.0f);
						seg.startTexture = core::Vector2(0.0f, 0.0f);
						seg.finishTexture = core::Vector2(1.0f, 0.0f);
						pt0 = buildSegment(seg, false);
					}
					else if (s->type == TileStripeType::TileRight)
					{
						seg.type = TileStripeType::AngularTopToRight;
						seg.centerTexture = core::Vector2(0.0f, 1.0f);
						seg.startTexture = core::Vector2(0.0f, 0.0f);
						seg.finishTexture = core::Vector2(1.0f, 0.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, false);
						seg.startTexture = core::Vector2(1.0f, 0.0f);
						seg.finishTexture = core::Vector2(1.0f, 1.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						pt0 = buildSegment(seg, false);
					}
				}
				else if (s->prevType == TileStripeType::TileRight)
				{
					if (s->type == TileStripeType::TileBottom)
					{
						seg.type = TileStripeType::AngularRightToBottom;
						seg.centerTexture = core::Vector2(0.0f, 0.0f);
						seg.startTexture = core::Vector2(1.0f, 1.0f);
						seg.finishTexture = core::Vector2(0.0f, 1.0f);
						pt0 = buildSegment(seg, false);
					}
					else if (s->type == TileStripeType::TileLeft)
					{
						seg.type = TileStripeType::AngularBottomToLeft;
						seg.centerTexture = core::Vector2(1.0f, 0.0f);
						seg.startTexture = core::Vector2(1.0f, 1.0f);
						seg.finishTexture = core::Vector2(0.0f, 1.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, false);
						seg.startTexture = core::Vector2(0.0f, 1.0f);
						seg.finishTexture = core::Vector2(0.0f, 0.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						pt0 = buildSegment(seg, false);
					}
				}
				else if (s->prevType == TileStripeType::TileBottom)
				{
					if (s->type == TileStripeType::TileLeft)
					{
						seg.type = TileStripeType::AngularBottomToLeft;
						seg.centerTexture = core::Vector2(1.0f, 0.0f);
						seg.startTexture = core::Vector2(0.0f, 1.0f);
						seg.finishTexture = core::Vector2(0.0f, 0.0f);
						pt0 = buildSegment(seg, false);
					}
					else if (s->type == TileStripeType::TileTop)
					{
						seg.type = TileStripeType::AngularLeftToTop;
						seg.centerTexture = core::Vector2(1.0f, 1.0f);
						seg.startTexture = core::Vector2(0.0f, 1.0f);
						seg.finishTexture = core::Vector2(0.0f, 0.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, false);
						seg.startTexture = core::Vector2(0.0f, 0.0f);
						seg.finishTexture = core::Vector2(1.0f, 0.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						pt0 = buildSegment(seg, false);
					}
				}
				//pt0 = buildSegment(t, 0, lp1->tl, pt0, pb0, lp1->large, false);
			}
			else if (s->lb > 0.0001f)
			{
				SegmentConfig seg;
				seg.centerPosition = s->start->pt.position();
				seg.startPosition = s->start->pb.position();
				seg.statLen = 0;
				float len = s->lb;
				seg.finishLen = len;
				seg.spline = s->b;
				if (s->prevType == TileStripeType::TileTop)
				{
					if (s->type == TileStripeType::TileLeft)
					{					
						seg.type = TileStripeType::AngularTopToLeft;
						seg.centerTexture = core::Vector2(0.0f, 0.0f);
						seg.startTexture = core::Vector2(1.0f, 1.0f);
						seg.finishTexture = core::Vector2(1.0f, 0.0f);
						pb0 = buildSegment(seg, true);
					}
					else if (s->type == TileStripeType::TileBottom)
					{
						seg.type = TileStripeType::AngularLeftToBottom;
						seg.centerTexture = core::Vector2(0.0f, 1.0f);
						seg.startTexture = core::Vector2(1.0f, 1.0f);
						seg.finishTexture = core::Vector2(1.0f, 0.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, true);
						seg.startTexture = core::Vector2(1.0f, 0.0f);
						seg.finishTexture = core::Vector2(0.0f, 0.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						pb0 = buildSegment(seg, true);
					}
				}
				else if (s->prevType == TileStripeType::TileRight)
				{
					if (s->type == TileStripeType::TileTop)
					{
						seg.type = TileStripeType::AngularRightToTop;
						seg.centerTexture = core::Vector2(1.0f, 0.0f);
						seg.startTexture = core::Vector2(0.0f, 1.0f);
						seg.finishTexture = core::Vector2(1.0f, 1.0f);
						pb0 = buildSegment(seg, true);
					}
					else if (s->type == TileStripeType::TileLeft)
					{
						seg.type = TileStripeType::AngularTopToLeft;
						seg.centerTexture = core::Vector2(0.0f, 0.0f);
						seg.startTexture = core::Vector2(0.0f, 1.0f);
						seg.finishTexture = core::Vector2(1.0f, 1.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, true);
						seg.startTexture = core::Vector2(1.0f, 1.0f);
						seg.finishTexture = core::Vector2(1.0f, 0.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						pb0 = buildSegment(seg, true);
					}
				}
				else if (s->prevType == TileStripeType::TileBottom)
				{
					if (s->type == TileStripeType::TileRight)
					{
						seg.type = TileStripeType::AngularBottomToRight;
						seg.centerTexture = core::Vector2(1.0f, 1.0f);
						seg.startTexture = core::Vector2(0.0f, 0.0f);
						seg.finishTexture = core::Vector2(0.0f, 1.0f);
						pb0 = buildSegment(seg, true);
					}
					else if (s->type == TileStripeType::TileTop)
					{						
						seg.type = TileStripeType::AngularRightToTop;
						seg.centerTexture = core::Vector2(1.0f, 0.0f);
						seg.startTexture = core::Vector2(0.0f, 0.0f);
						seg.finishTexture = core::Vector2(0.0f, 1.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, true);
						seg.startTexture = core::Vector2(0.0f, 1.0f);
						seg.finishTexture = core::Vector2(1.0f, 1.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						pb0 = buildSegment(seg, true);
					}
				}
				else if (s->prevType == TileStripeType::TileLeft)
				{
					if (s->type == TileStripeType::TileBottom)
					{
						seg.type = TileStripeType::AngularLeftToBottom;
						seg.centerTexture = core::Vector2(0.0f, 1.0f);
						seg.startTexture = core::Vector2(1.0f, 0.0f);
						seg.finishTexture = core::Vector2(0.0f, 0.0f);
						pb0 = buildSegment(seg, true);
					}
					else if (s->type == TileStripeType::TileRight)
					{
						seg.type = TileStripeType::AngularBottomToRight;
						seg.centerTexture = core::Vector2(1.0f, 1.0f);
						seg.startTexture = core::Vector2(1.0f, 0.0f);
						seg.finishTexture = core::Vector2(0.0f, 0.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, true);
						seg.startTexture = core::Vector2(0.0f, 0.0f);
						seg.finishTexture = core::Vector2(0.0f, 1.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						pb0 = buildSegment(seg, true);
					}
				}
				
				//pb0 = buildSegment(b, 0, lp1->bl, pb0, pt0, lp1->large, true);
				
			}

			float lt = s->t->length() - s->lt - s->rt;
			float lb = s->b->length() - s->lb - s->rb;
			float l = core::Math::max(0, (lt + lb)*0.5f);
			float lt0 = s->lt;
			float lb0 = s->lb;


			float tilesCount = static_cast<int>(core::Math::round(l / s->w.width));
			if (tilesCount <= 0)
			{
				tilesCount = 1;
			}
			for (int j = 0; j < tilesCount; j++)
			{
				float u1 = static_cast<float>(j + 1) / static_cast<float>(tilesCount);
				float lt1 = core::Math::lerp(s->lt, s->t->length() - s->rt, u1);
				float lb1 = core::Math::lerp(s->lb, s->b->length() - s->rb, u1);
				addQuad(s->type, s->t, s->b, lt0, lb0, lt1, lb1, pt0, pb0);

				lt0 = lt1;
				lb0 = lb1;
			}

			
			if (s->rt > 0.0001f && s->rb > 0.0001f)
			{
				core::Vector2 dir = s->prevD.normalize();
				addQuad(TileStripeType::CapRight,
					pb0, pt0,
					s->finish->pt.position() + dir*s->finish->tl,
					s->finish->pb.position() + dir*s->finish->bl);
			}
			else if (s->rt > 0.0001f)
			{
				SegmentConfig seg;
				seg.centerPosition = pb0;
				seg.startPosition = pt0;
				float len = s->finish->tr;
				seg.statLen = s->t->length() - len;
				seg.finishLen = s->t->length();
				seg.spline = s->t;
				if (s->type == TileStripeType::TileTop)
				{
					if (s->nextType == TileStripeType::TileRight)
					{
						seg.type = TileStripeType::AngularTopToRight;
						seg.centerTexture = core::Vector2(0.0f, 1.0f);
						seg.startTexture = core::Vector2(0.0f, 0.0f);
						seg.finishTexture = core::Vector2(1.0f, 0.0f);
						buildSegment(seg, false);
					} 
					else if (s->nextType == TileStripeType::TileBottom)
					{
						seg.type = TileStripeType::AngularTopToRight;
						seg.centerTexture = core::Vector2(0.0f, 1.0f);
						seg.startTexture = core::Vector2(0.0f, 0.0f);
						seg.finishTexture = core::Vector2(1.0f, 0.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, false);						
						seg.startTexture = core::Vector2(1.0f, 0.0f);
						seg.finishTexture = core::Vector2(1.0f, 1.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						buildSegment(seg, false);						
					}
				}
				else if (s->type == TileStripeType::TileLeft)
				{
					if (s->nextType == TileStripeType::TileTop)
					{
						seg.type = TileStripeType::AngularLeftToTop;
						seg.centerTexture = core::Vector2(1.0f, 1.0f);
						seg.startTexture = core::Vector2(0.0f, 1.0f);
						seg.finishTexture = core::Vector2(0.0f, 0.0f);
						buildSegment(seg, false);
					}
					else if (s->nextType == TileStripeType::TileRight)
					{
						seg.type = TileStripeType::AngularLeftToTop;
						seg.centerTexture = core::Vector2(1.0f, 1.0f);
						seg.startTexture = core::Vector2(0.0f, 1.0f);
						seg.finishTexture = core::Vector2(0.0f, 0.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, false);
						seg.startTexture = core::Vector2(0.0f, 0.0f);
						seg.finishTexture = core::Vector2(1.0f, 0.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						buildSegment(seg, false);
					}
				}
				else if (s->type == TileStripeType::TileRight)
				{
					if (s->nextType == TileStripeType::TileBottom)
					{
						seg.type = TileStripeType::AngularRightToBottom;
						seg.centerTexture = core::Vector2(0.0f, 0.0f);
						seg.startTexture = core::Vector2(1.0f, 0.0f);
						seg.finishTexture = core::Vector2(1.0f, 1.0f);
						buildSegment(seg, false);
					}
					else if (s->nextType == TileStripeType::TileLeft)
					{
						seg.type = TileStripeType::AngularRightToBottom;
						seg.centerTexture = core::Vector2(0.0f, 0.0f);
						seg.startTexture = core::Vector2(1.0f, 0.0f);
						seg.finishTexture = core::Vector2(1.0f, 1.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, false);
						seg.startTexture = core::Vector2(1.0f, 1.0f);
						seg.finishTexture = core::Vector2(0.0f, 1.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						buildSegment(seg, false);
					}
				}
				else if (s->type == TileStripeType::TileBottom)
				{
					if (s->nextType == TileStripeType::TileLeft)
					{
						seg.type = TileStripeType::AngularBottomToLeft;
						seg.centerTexture = core::Vector2(1.0f, 0.0f);
						seg.startTexture = core::Vector2(1.0f, 1.0f);
						seg.finishTexture = core::Vector2(0.0f, 1.0f);
						buildSegment(seg, false);
					}
					else if (s->nextType == TileStripeType::TileTop)
					{
						seg.type = TileStripeType::AngularBottomToLeft;
						seg.centerTexture = core::Vector2(1.0f, 0.0f);
						seg.startTexture = core::Vector2(1.0f, 1.0f);
						seg.finishTexture = core::Vector2(0.0f, 1.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, false);
						seg.startTexture = core::Vector2(0.0f, 1.0f);
						seg.finishTexture = core::Vector2(0.0f, 0.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						buildSegment(seg, false);
					}
				}
			}
			else if (s->rb > 0.0001f)
			{
				SegmentConfig seg;
				seg.centerPosition = pt0;
				seg.startPosition = pb0;
				float len = s->finish->br;
				seg.statLen = s->b->length() - len;
				seg.finishLen = s->b->length();
				seg.spline = s->b;
				if (s->type == TileStripeType::TileTop)
				{
					if (s->nextType == TileStripeType::TileLeft)
					{
						seg.type = TileStripeType::AngularTopToLeft;
						seg.centerTexture = core::Vector2(0.0f, 0.0f);
						seg.startTexture = core::Vector2(0.0f, 1.0f);
						seg.finishTexture = core::Vector2(1.0f, 1.0f);
						buildSegment(seg, true);
					}
					else if (s->nextType == TileStripeType::TileBottom) 
					{
						seg.type = TileStripeType::AngularTopToLeft;
						seg.centerTexture = core::Vector2(0.0f, 0.0f);
						seg.startTexture = core::Vector2(0.0f, 1.0f);
						seg.finishTexture = core::Vector2(1.0f, 1.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, false);
						seg.startTexture = core::Vector2(1.0f, 1.0f);
						seg.finishTexture = core::Vector2(1.0f, 0.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						buildSegment(seg, true);
					}
				}
				else if (s->type == TileStripeType::TileRight)
				{
					if (s->nextType == TileStripeType::TileTop)
					{
						seg.type = TileStripeType::AngularRightToTop;
						seg.centerTexture = core::Vector2(1.0f, 0.0f);
						seg.startTexture = core::Vector2(0.0f, 0.0f);
						seg.finishTexture = core::Vector2(0.0f, 1.0f);
						buildSegment(seg, true);
					}
					else if (s->nextType == TileStripeType::TileLeft)
					{
						seg.type = TileStripeType::AngularRightToTop;
						seg.centerTexture = core::Vector2(1.0f, 0.0f);
						seg.startTexture = core::Vector2(0.0f, 0.0f);
						seg.finishTexture = core::Vector2(0.0f, 1.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, false);
						seg.startTexture = core::Vector2(0.0f, 1.0f);
						seg.finishTexture = core::Vector2(1.0f, 1.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						buildSegment(seg, true);
					}
				}
				else if (s->type == TileStripeType::TileBottom)
				{
					if (s->nextType == TileStripeType::TileRight)
					{
						seg.type = TileStripeType::AngularBottomToRight;
						seg.centerTexture = core::Vector2(1.0f, 1.0f);
						seg.startTexture = core::Vector2(1.0f, 0.0f);
						seg.finishTexture = core::Vector2(0.0f, 0.0f);
						buildSegment(seg, true);
					}
					else if (s->nextType == TileStripeType::TileTop)
					{
						seg.type = TileStripeType::AngularBottomToRight;
						seg.centerTexture = core::Vector2(1.0f, 1.0f);
						seg.startTexture = core::Vector2(1.0f, 0.0f);
						seg.finishTexture = core::Vector2(0.0f, 0.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, false);
						seg.startTexture = core::Vector2(0.0f, 0.0f);
						seg.finishTexture = core::Vector2(0.0f, 1.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						buildSegment(seg, true);
					}
				}
				else if (s->type == TileStripeType::TileLeft)
				{
					if (s->nextType == TileStripeType::TileBottom)
					{
						seg.type = TileStripeType::AngularLeftToBottom;
						seg.centerTexture = core::Vector2(0.0f, 1.0f);
						seg.startTexture = core::Vector2(1.0f, 1.0f);
						seg.finishTexture = core::Vector2(1.0f, 0.0f);
						buildSegment(seg, true);
					}
					else if (s->nextType == TileStripeType::TileRight)
					{
						seg.type = TileStripeType::AngularLeftToBottom;
						seg.centerTexture = core::Vector2(0.0f, 1.0f);
						seg.startTexture = core::Vector2(1.0f, 1.0f);
						seg.finishTexture = core::Vector2(1.0f, 0.0f);
						seg.finishLen -= len*0.5f;
						seg.startPosition = buildSegment(seg, false);
						seg.startTexture = core::Vector2(1.0f, 0.0f);
						seg.finishTexture = core::Vector2(0.0f, 0.0f);
						seg.statLen = seg.finishLen;
						seg.finishLen += len*0.5f;
						buildSegment(seg, true);
					}
				}
				//buildSegment(b, b.length() - lp2->br, b.length(), pb0, lp2->pt.position(), lp2->large, true);
			}
			

		}


		for (int i = 0; i < stripes.size(); i++)
		{
			delete stripes[i]->t;
			delete stripes[i]->b;
			delete stripes[i];
		}
		delete [] points;

		_result.init(&_vertices, &_indices);
		for (uint i =0; i < _tiles.size(); i++)
		{
			_result.addMesh(_tiles[i].indexer);
		}

		if (_polyline.size() > 0)
		{
			ListPoint *polyline = new ListPoint[_polyline.size()];
			int polylineSize = _polyline.size();
			for (int i = 0; i < _polyline.size(); i++)
			{
				polyline[i].p = _polyline[i];
				polyline[i].prev = polyline + (i - 1);
				polyline[i].next = polyline + (i + 1);
			}

			if (closed)
			{
				polyline[0].prev = polyline + (polylineSize - 1);
				polyline[polylineSize - 1].next = polyline;
			}
			else
			{
				polyline[0].prev = polyline;
				polyline[polylineSize - 1].next = polyline + (polylineSize - 1);
				polylineSize--;
			}
			ListPoint *p = polyline;
			for (uint i = 0; i < polylineSize; i++)
			{
				StripeEdge edge;
				edge.p[0] = p->prev->p;
				edge.p[1] = p->p;
				edge.p[2] = p->next->p;
				edge.p[3] = p->next->next->p;
				core::Vector2 d = edge.p[2] - edge.p[1];
				switch (d.direction())
				{
				case core::Direction2::Left: 
					if ((config.PhysicsMode & llge::PhysicsBottom) != 0)
					{
						_edges.push_back(edge);
					}
					break;
				case core::Direction2::Right: 
					if ((config.PhysicsMode & llge::PhysicsTop) != 0)
					{
						_edges.push_back(edge);
					}
					break;
				case core::Direction2::Top: 
					if ((config.PhysicsMode & llge::PhysicsLeft) != 0)
					{
						_edges.push_back(edge);
					}
					break;
				case core::Direction2::Bottom: 
					if ((config.PhysicsMode & llge::PhysicsRight) != 0)
					{
						_edges.push_back(edge);
					}
					break;
				default: 
					break;
				}				

				p = p->next;
			}
			delete[] polyline;
		}
	}

	llge::ITerrainConfig* StripeBuilder::getConfig()
	{
		return &config;
	}

	void StripeBuilder::buildStripe(IntPtr terrainStripePoints, float tension, int count, bool closed)
	{
		StripePoint* vertices = static_cast<StripePoint*>(terrainStripePoints);
		build(vertices, tension, count, closed);
	}

	void StripeBuilder::dispose()
	{
		delete this;
	}

	int StripeBuilder::getDebugRenderVerticesCount()
	{
		return _dedugRender.size();
	}

	void StripeBuilder::getDebugRenderVertices(IntPtr vertices2f)
	{
		core::Vector2 *res = static_cast<core::Vector2 *>(vertices2f);
		for (uint i = 0; i < _dedugRender.size(); i++)
		{
			res[i] = _dedugRender[i];
		}
	}

	llge::IMeshesResult* StripeBuilder::getResult()
	{
		return &_result;
	}

	int StripeBuilder::getEdgesCount()
	{
		return _edges.size();
	}

	void StripeBuilder::getEdge(int i, IntPtr vertices2f)
	{
		StripeEdge e = _edges[i];
		core::Vector2* vertices = static_cast<core::Vector2*>(vertices2f);
		for (int j = 0; j < 4; j++)
		{
			vertices[j] = _edges[i].p[j];
		}
	}

	void StripeBuilder::initQuad1Indices()
	{
		_quad1IndicesStart = _indices.size();
		_quad1IndicesCount = 6;
		_indices.push_back(0);
		_indices.push_back(1);
		_indices.push_back(2);
		_indices.push_back(0);
		_indices.push_back(2);
		_indices.push_back(3);
	}

	void StripeBuilder::initQuadIndices(int count)
	{
		_quadIndicesStart = _indices.size();
		_quadIndicesCount = count * 6;
		for (uint i = 0; i < count; i++)
		{
			ushort o = i * 2;
			_indices.push_back(o + 0);
			_indices.push_back(o + 1);
			_indices.push_back(o + 3);
			_indices.push_back(o + 0);
			_indices.push_back(o + 3);
			_indices.push_back(o + 2);
		}
	}

	void StripeBuilder::initFanIndices(int count)
	{
		_fanIndicesStart = _indices.size();
		_fanIndicesCount = count*3;
		for (uint i = 0; i < count; i++)
		{
			_indices.push_back(0);
			_indices.push_back(i + 1);
			_indices.push_back(i + 2);
		}
		_inverseFanIndicesStart = _indices.size();
		_inverseFanIndicesCount = count * 3;
		for (uint i = 0; i < count; i++)
		{
			_indices.push_back(0);
			_indices.push_back(i + 2);
			_indices.push_back(i + 1);
		}
	}

	void StripeBuilder::initFan2Indices(int count)
	{
		_fan2IndicesStart = _indices.size();
		_fan2IndicesCount = count*3;
		for (uint i = 0; i < count; i++)
		{
			_indices.push_back(0);
			_indices.push_back(i + 1);
			_indices.push_back(i + 2);
		}
	}
	
	core::Vector2 StripeBuilder::buildSegment(TileStripeType::e type, core::Spline2& spline, float l0, float l1, core::Vector2 start, core::Vector2 center, bool large, bool inverse)
	{
		int agulatTilesCount = static_cast<int>(config.DetailX);
		if (large)
			agulatTilesCount *= 2;


		core::Vector2 pt0 = start;
		core::Vector2 pt1;
		for (int j = 0; j < agulatTilesCount; j++)
		{
			float u1 = static_cast<float>(j + 1) / static_cast<float>(agulatTilesCount);
			float lt1 = core::Math::lerp(l0, l1, u1);
			float ut1 = spline.getU(lt1);
			pt1 = spline.get(ut1);

			_dedugRender.push_back(pt0);
			_dedugRender.push_back(pt1);			
			_dedugRender.push_back(pt1);
			_dedugRender.push_back(center);

			pt0 = pt1;
		}
		return pt0;
	}

	core::Vector2 StripeBuilder::buildSegment(const SegmentConfig &seg, bool inverse)
	{
		int tilesCount = static_cast<int>(config.DetailX);
		StripeTile tile;
		tile.indexer.VerticesFirst = _vertices.size();
		tile.indexer.VerticesCount = tilesCount + 2;
		tile.indexer.IndicesFirst = inverse ? _inverseFanIndicesStart : _fanIndicesStart;
		tile.indexer.IndicesCount = inverse ? _inverseFanIndicesCount : _fanIndicesCount;
		//tile.indexer.IndicesFirst = _fanIndicesStart;
		//tile.indexer.IndicesCount = _fanIndicesCount;
		tile.indexer.Type = seg.type;
		tile.type = seg.type;

		core::Vector2 p0 = seg.startPosition;
		core::Vector2 t0 = seg.startTexture;
		core::Vector2 p1;
		core::Vector2 t1;

		_vertices.push_back(core::MeshVertex(seg.centerPosition, seg.centerTexture));
		_vertices.push_back(core::MeshVertex(seg.startPosition, seg.startTexture));
		for (int j = 0; j < tilesCount; j++)
		{
			float u1 = static_cast<float>(j + 1) / static_cast<float>(tilesCount);
			float lt1 = core::Math::lerp(seg.statLen, seg.finishLen, u1);
			core::MSplineU ut1 = seg.spline->getU(lt1);
			p1 = seg.spline->get(ut1);
			t1 = core::Vector2::lerp(seg.startTexture, seg.finishTexture, u1);
			
			if (t1.getY() < 0.01)
				t1.setY(0.0f);
			if (t1.getY() > 0.99)
				t1.setY(1.0f);

			if (t1.getX() < 0.01)
				t1.setX(0.0f);
			if (t1.getX() > 0.99)
				t1.setX(1.0f);


			_vertices.push_back(core::MeshVertex(p1, t1));
			p0 = p1;
			t0 = t1;
		}
		_tiles.push_back(tile);
		return p1;
	}

	bool line2Cross(core::Vector2 a, core::Vector2 b, core::Vector2 c, core::Vector2 d, core::Vector2 &r)
	{
		float det = (d.getY() - c.getY())*(b.getX() - a.getX()) - (d.getX() - c.getX())*(b.getY() - a.getY());
		if (core::Math::abs(det) < core::Math::Epsilon)
		{
			r = core::Vector2::empty;
			return false;
		}

		float det1 = (d.getX() - c.getX())*(a.getY() - c.getY()) - (d.getY() - c.getY())*(a.getX() - c.getX());
		float u1 = det1 / det;
		r = a*(1 - u1) + b*u1;
		return true;
	}

	bool stripeCross(core::Vector2 p, core::Vector2 d1, core::Vector2 d2, float w1, float w2, core::Vector2 &r, bool reversed)
	{
		core::Vector2 n1 = reversed ? d1.rotate90cw() : d1.rotate90ccw();
		core::Vector2 n2 = reversed ? d2.rotate90cw() : d2.rotate90ccw();

		core::Vector2 a = p + n1*w1;
		core::Vector2 b = a + d1;
		core::Vector2 c = p + n2*w2;
		core::Vector2 d = c + d2;
		return line2Cross(a, b, c, d, r);
	}
	
	StripeListPoint* StripeBuilder::createStripeList(const StripePoint* vertices, int count, bool closed)
	{
		StripeListPoint *points = new StripeListPoint[count];
		for (int i = 0; i < count; i++)
		{
			points[i].point = vertices[i];
			points[i].prev = points + (i - 1);
			points[i].next = points + (i + 1);
		}

		if (closed)
		{
			points[0].prev = points + (count - 1);
			points[count - 1].next = points;
		}
		else
		{
			points[0].prev = points;
			points[count - 1].next = points + (count - 1);
		}
		StripeListPoint *current = points;
		bool reversed = false;
		if (!closed && config.flipReversed)
		{
			if (vertices[0].x > vertices[count - 1].x)
			{
				reversed = true;
			}
		}
		for (int i = 0; i < count; i++)
		{
			StripeListPoint *prev = current->prev;
			StripeListPoint *next = current->next;
			core::Vector2 d0(current->point.x - prev->point.x, current->point.y - prev->point.y);
			core::Vector2 d1(next->point.x - current->point.x, next->point.y - current->point.y);
			current->prevD = d0;
			current->nextD = d1;
			d0 = d0.normalize();
			d1 = d1.normalize();
			current->d = (d0 + d1).normalize();
			current->n = reversed? current->d.rotate90cw(): current->d.rotate90ccw();
			current->tl = 0;
			current->bl = 0;
			current->tr = 0;
			current->br = 0;
			current->tilesCount = 0;
			core::Direction2::e dir0 = d0.direction();
			core::Direction2::e dir1 = d1.direction();
			StripeWidth w;
			current->large = false;

			if (!d0.isEmpty() && !d1.isEmpty() && (config.AllowedTiles & StripeType::TileAngular) != 0)
			{
				float dot = core::Vector2::dotProduct(d0*-1.0f, d1);
				current->scale = 1.0f / core::Math::sqrt((1 - dot) / 2.0f);
				float lfactor = 0.707f * current->scale;
				if (dir0 != dir1)
				{
					StripeWidth w0 = config.getWidth(dir0);
					StripeWidth w1 = config.getWidth(dir1);

					w0.out *= current->point.wt;
					w0.in *= current->point.wb;
					w1.out *= current->point.wt;
					w1.in *= current->point.wb;

					core::Vector2 pt;
					core::Vector2 pb;

					stripeCross(current->point.position(), d0, d1, w0.out, w1.out, pt, reversed);
					stripeCross(current->point.position(), d0, d1, -w0.in, -w1.in, pb, reversed);

					current->pt = current->point.copy(pt);
					current->pb = current->point.copy(pb);

					//float gl = (pt - pb).lengthSqr();
					//float k0 = w0.out + w0.in;
					//float k1 = w1.out + w1.in;
					
					float l;// = (w1.out + w1.in)*lfactor;// core::Math::sqrt(gl - k0*k0);
					float r;// = (w0.out + w0.in)*lfactor;// core::Math::sqrt(gl - k1*k1);

					float cross = core::Vector2::crossProduct(d0, d1);
					if ((dir0 & dir1) == 0)
					{
						l = (w1.out + w1.in)*lfactor;// core::Math::sqrt(gl - k0*k0);
						r = (w0.out + w0.in)*lfactor;// core::Math::sqrt(gl - k1*k1);						
					}
					else
					{
						l = 1.5f*(w1.out + w1.in)*lfactor;// core::Math::sqrt(gl - k0*k0);
						r = 1.5f*(w0.out + w0.in)*lfactor;// core::Math::sqrt(gl - k1*k1);
						current->large = true;

					}
					if (cross < 0)
					{
						current->tl = l;
						current->tr = r;
					}
					else
					{
						current->bl = l;
						current->br = r;
					}
				}
				else
				{
					w = config.getWidth(dir0);
					w.out *= current->point.wt;
					w.in *= current->point.wb;
					current->pt = current->point.offset(current->n * w.out*current->scale);
					current->pb = current->point.offset(current->n * -w.in *current->scale);
				}
			}
			else
			{
				if ((config.AllowedTiles & StripeType::TileAngular) == 0)
				{
					w = config.top;
					w.out *= current->point.wt;
					w.in *= current->point.wb;
					if (!closed)
					{
						if (i == 0)
						{
							StripeWidth lw = config.left;
							current->tl = lw.in;
							current->bl = lw.in;
							current->tr = lw.out;
							current->br = lw.out;
						}
						else if (i == count - 1)
						{
							StripeWidth rw = config.right;
							current->tr = rw.in;
							current->br = rw.in;
							current->tl = rw.out;
							current->bl = rw.out;
						}
					}
				}
				else
				{
					if (!d0.isEmpty())
					{
						w = config.getWidth(dir0);
						w.out *= current->point.wt;
						w.in *= current->point.wb;
					}
					else if (!d1.isEmpty())
					{
						w = config.getWidth(dir1);
						w.out *= current->point.wt;
						w.in *= current->point.wb;
					}
				}
				
				current->scale = 1.0f;				
				current->pt = current->point.offset(current->n * w.out*current->scale);
				current->pb = current->point.offset(current->n * -w.in *current->scale);
			}

			current++;
		}
		return points;
	}

	void StripeBuilder::addQuad(TileStripeType::e type, core::MacroSpline2* t, core::MacroSpline2* b, float tl0, float bl0, float tl1, float bl1, core::Vector2& startT, core::Vector2& startB)
	{
		int tilesCount = static_cast<int>(config.DetailX);
		core::Vector2 pt0 = startT;
		core::Vector2 pt1;
		core::Vector2 pb0 = startB;
		core::Vector2 pb1;
		StripeTile tile;
		tile.indexer.VerticesFirst = _vertices.size();
		tile.indexer.VerticesCount = (tilesCount + 1) * 2;
		tile.indexer.IndicesFirst = _quadIndicesStart;
		tile.indexer.IndicesCount = _quadIndicesCount;
		tile.indexer.Type = type;
		tile.type = type;
		_vertices.push_back(core::MeshVertex(pb0, 0.0f, 1.0f));
		_vertices.push_back(core::MeshVertex(pt0, 0.0f, 0.0f));
		for (int j = 0; j < tilesCount; j++)
		{
			float u1 = static_cast<float>(j + 1) / static_cast<float>(tilesCount);
			float lt1 = core::Math::lerp(tl0, tl1, u1);
			float lb1 = core::Math::lerp(bl0, bl1, u1);
			core::MSplineU ut1 = t->getU(lt1);
			core::MSplineU ub1 = b->getU(lb1);
			pt1 = t->get(ut1);
			pb1 = b->get(ub1);

			if (u1 < 0.01)
				u1 = 0.0f;
			if (u1 > 0.99)
				u1 = 1.0f;

			_vertices.push_back(core::MeshVertex(pb1, u1, 1.0f));
			_vertices.push_back(core::MeshVertex(pt1, u1, 0.0f));

			pt0 = pt1;
			pb0 = pb1;
		}
		_tiles.push_back(tile);
		startT = pt0;
		startB = pb0;
	}

	void StripeBuilder::addQuad(TileStripeType::e type, core::Spline2& t, core::Spline2& b, float tl0, float bl0, float tl1, float bl1, core::Vector2 &startT, core::Vector2 &startB)
	{
		int tilesCount = static_cast<int>(config.DetailX);
		core::Vector2 pt0 = startT;
		core::Vector2 pt1;
		core::Vector2 pb0 = startB;
		core::Vector2 pb1;
		StripeTile tile;
		tile.indexer.VerticesFirst = _vertices.size();
		tile.indexer.VerticesCount = (tilesCount + 1) * 2;
		tile.indexer.IndicesFirst = _quadIndicesStart;
		tile.indexer.IndicesCount = _quadIndicesCount;
		tile.indexer.Type = type;
		tile.type = type;
		_vertices.push_back(core::MeshVertex(pb0, 0.0f, 1.0f));
		_vertices.push_back(core::MeshVertex(pt0, 0.0f, 0.0f));

		for (int j = 0; j < tilesCount; j++)
		{
			float u1 = static_cast<float>(j + 1) / static_cast<float>(tilesCount);
			float lt1 = core::Math::lerp(tl0, tl1, u1);
			float lb1 = core::Math::lerp(bl0, bl1, u1);
			float ut1 = t.getU(lt1);
			float ub1 = b.getU(lb1);
			pt1 = t.get(ut1);
			pb1 = b.get(ub1);

			if (u1 < 0.01)
				u1 = 0.0f;
			if (u1 > 0.99)
				u1 = 1.0f;

			_vertices.push_back(core::MeshVertex(pb1, u1, 1.0f));
			_vertices.push_back(core::MeshVertex(pt1, u1, 0.0f));
			
			pt0 = pt1;
			pb0 = pb1;
		}
		_tiles.push_back(tile);
		startT = pt0;
		startB = pb0;
	}
	
	void StripeBuilder::addQuad(TileStripeType::e type, core::Vector2 lb, core::Vector2 lt, core::Vector2 rt, core::Vector2 rb)
	{
		StripeTile tile;
		tile.indexer.VerticesFirst = _vertices.size();
		tile.indexer.VerticesCount = 4;
		tile.indexer.IndicesFirst = _quad1IndicesStart;
		tile.indexer.IndicesCount = _quad1IndicesCount;
		tile.indexer.Type = type;
		tile.type = type;

		_vertices.push_back(core::MeshVertex(lb, 0.0f, 1.0f));
		_vertices.push_back(core::MeshVertex(lt, 0.0f, 0.0f));
		_vertices.push_back(core::MeshVertex(rt, 1.0f, 0.0f));
		_vertices.push_back(core::MeshVertex(rb, 1.0f, 1.0f));
		_tiles.push_back(tile);
	}
}