#ifndef CORE_DEBUG_RENDER_H
#define CORE_DEBUG_RENDER_H

#include "core_classes.h"
#include "Vector.h"
#include "Matrix.h"

namespace core
{
	class IDebugRender
	{
	public:
		virtual ~IDebugRender() { }
		virtual void drawEdge(uint color, const Vector3 &a, const Vector3 &b) = 0;
	};

	class DebugRenderDummy : public IDebugRender
	{
	public:
		static DebugRenderDummy Default;
		virtual void drawEdge(uint color, const Vector3 &a, const Vector3 &b) OVERRIDE;
	};
		
	class DebugDraw
	{
	public:
		static DebugDraw Default;
		IDebugRender* Render;
		DebugDraw();
		void circle(uint color, const Vector3 center, float r);
		void edge(uint color, const Vector3 a, const Vector3 b);
		void polyline(uint color, const std::vector<Vector2> &p, bool closed = false, float z = 0.0f);
		void polyline(uint color, const Vector2* p, uint count, bool closed = false, float z = 0.0f);
		void polyline(uint color, Vector3 center, float scale, const Vector2* p, uint count, bool closed = false);
		void polyline(uint color, const std::vector<Vector3> &p, bool closed = false);
		void polyline(uint color, const Vector3* p, uint count, bool closed = false);
		void polyline(uint color, Vector3 center, const Vector3* p, uint count, bool closed = false);
	private:
		static const int circleDetail = 32;
		Vector2 _circlePoints[circleDetail];
	};
}

#endif /*CORE_IDEBUG_RENDER_H*/
