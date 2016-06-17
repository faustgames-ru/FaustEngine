#include "DebugRender.h"

namespace core
{
	DebugRenderDummy DebugRenderDummy::Default;

	void DebugRenderDummy::drawEdge(uint color, const Vector3& a, const Vector3& b)
	{
	}

	DebugDraw DebugDraw::Default;

	DebugDraw::DebugDraw(): Render(&DebugRenderDummy::Default)
	{
		Vector2 p;
		float a = 0.0f;
		float da = Math::Pi * 2.0f / circleDetail;
		for (uint i =0; i < circleDetail; i++, a += da)
		{
			_circlePoints[i].setX(Math::cos(a));
			_circlePoints[i].setY(Math::sin(a));
		}
	}

	void DebugDraw::circle(uint color, const Vector3 center, float r)
	{
		polyline(color, center, r, _circlePoints, circleDetail, true);
	}

	void DebugDraw::edge(uint color, const Vector3 a, const Vector3 b)
	{
		Render->drawEdge(color, a, b);
	}

	void DebugDraw::polyline(uint color, const std::vector<Vector2>& p, bool closed, float z)
	{
		polyline(color, p.data(), p.size(), closed, z);
	}

	void DebugDraw::polyline(uint color, const Vector2* p, uint count, bool closed, float z)
	{
		uint j;
		uint s;
		if (closed)
		{
			j = 0;
			s = 1;
		}
		else
		{
			j = count - 1;
			s = 0;
		}
		for (uint i = s; i < count; i++)
		{
			Render->drawEdge(color, p[j].toVector3(z), p[i].toVector3(z));
			j = i;
		}
	}

	void DebugDraw::polyline(uint color, Vector3 center, float scale, const Vector2* p, uint count, bool closed)
	{
		uint j;
		uint s;
		if (closed)
		{
			j = count - 1;
			s = 0;
		}
		else
		{
			j = 0;
			s = 1;
		}
		for (uint i = s; i < count; i++)
		{
			Render->drawEdge(color, center + p[j].toVector3()*scale, center + p[i].toVector3()*scale);
			j = i;
		}
	}

	void DebugDraw::polyline(uint color, const std::vector<Vector3> &p, bool closed)
	{
		polyline(color, p.data(), p.size(), closed);
	}

	void DebugDraw::polyline(uint color, const Vector3* p, uint count, bool closed)
	{
		uint j;
		uint s;
		if (closed)
		{
			j = count - 1;
			s = 0;
		}
		else
		{
			j = 0;
			s = 1;
		}
		for (uint i = s; i < count; i++)
		{
			Render->drawEdge(color, p[j], p[i]);
			j = i;
		}
	}

	void DebugDraw::polyline(uint color, Vector3 center, const Vector3* p, uint count, bool closed)
	{
		uint j;
		uint s;
		if (closed)
		{
			j = count - 1;
			s = 0;
		}
		else
		{
			j = 0;
			s = 1;
		}
		for (uint i = s; i < count; i++)
		{
			Render->drawEdge(color, center + p[j], center + p[i]);
			j = i;
		}
	}
}