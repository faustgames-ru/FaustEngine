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

	StripeConfig::StripeConfig(): AllowedTiles(StripeType::All), DetailX(16), DetailY(16)
	{
	}

	void StripeBuilder::build(const StripePoint* vertices, int count, bool closed)
	{
		_dedugRender.clear();

		if (count < 2) return;
		StripeListPoint *points = new StripeListPoint[count];
		for (int i = 0; i < count; i++)
		{
			points[i].point = vertices[i];
			points[i].prev = points + (i - 1);
			points[i].next = points + (i + 1);
		}

		int edgesCount = count;
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
			if (!d0.isEmpty() && !d1.isEmpty())
			{
				float dot = core::Vector2::dotProduct(d0, d1);
				current->scale = 1.0f / core::Math::sqrt((1 - dot) / 2.0f);
			}
			else
			{
				current->scale = 1.0f;
			}
			current->n = current->d.rotate90ccw();

			current++;
		}

		StripePoint p0, p1, p2, p3, tp0, tp1, tp2, tp3, bp0, bp1, bp2, bp3;
		current = points;
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
			int tilesCount = static_cast<int>(core::Math::round(l1 / config.DetailX));
			
			tp0 = p0.offset(lp0->n.getX() * p0.width*lp0->scale, lp0->n.getY() * p0.width*lp0->scale);
			tp1 = p1.offset(lp1->n.getX() * p1.width*lp1->scale, lp1->n.getY() * p1.width*lp1->scale);
			tp2 = p2.offset(lp2->n.getX() * p2.width*lp2->scale, lp2->n.getY() * p2.width*lp2->scale);
			tp3 = p3.offset(lp3->n.getX() * p3.width*lp3->scale, lp3->n.getY() * p3.width*lp3->scale);

			bp0 = p0.offset(lp0->n.getX() * -p0.width*lp0->scale, lp0->n.getY() * -p0.width*lp0->scale);
			bp1 = p1.offset(lp1->n.getX() * -p1.width*lp1->scale, lp1->n.getY() * -p1.width*lp1->scale);
			bp2 = p2.offset(lp2->n.getX() * -p2.width*lp2->scale, lp2->n.getY() * -p2.width*lp2->scale);
			bp3 = p3.offset(lp3->n.getX() * -p3.width*lp3->scale, lp3->n.getY() * -p3.width*lp3->scale);
			
			core::Spline2 t(
				tp0.x, tp0.y, p0.tension,
				tp1.x, tp1.y, p1.tension,
				tp2.x, tp2.y, p2.tension,
				tp3.x, tp3.y, p3.tension, tilesCount * 2);
			core::Spline2 b(
				bp0.x, bp0.y, p0.tension,
				bp1.x, bp1.y, p1.tension,
				bp2.x, bp2.y, p2.tension,
				bp3.x, bp3.y, p3.tension, tilesCount * 2);


			for (int j = 0; j < tilesCount; j++)
			{
				float lt0 = static_cast<float>(j) * t.length() / static_cast<float>(tilesCount);
				float lb0 = static_cast<float>(j) * b.length() / static_cast<float>(tilesCount);
				float lt1 = static_cast<float>(j+1) * t.length() / static_cast<float>(tilesCount);
				float lb1 = static_cast<float>(j+1) * b.length() / static_cast<float>(tilesCount);

				float ut0 = t.getU(lt0);
				float ub0 = b.getU(lb0);
				float ut1 = t.getU(lt1);
				float ub1 = b.getU(lb1);

				core::Vector2 pt0 = t.get(ut0);
				core::Vector2 pb0 = b.get(ub0);
				core::Vector2 pt1 = t.get(ut1);
				core::Vector2 pb1 = b.get(ub1);

				_dedugRender.push_back(pt0);
				_dedugRender.push_back(pb0);
				_dedugRender.push_back(pt0);
				_dedugRender.push_back(pt1);
				_dedugRender.push_back(pb0);
				_dedugRender.push_back(pb1);
			}

			current = current->next;
		}

		delete [] points;
	}

	void StripeBuilder::buildStripe(IntPtr terrainStripePoints, int count, bool closed)
	{
		StripePoint* vertices = static_cast<StripePoint*>(terrainStripePoints);
		build(vertices, count, closed);
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
}