#ifndef ZOMBO_EDITOR_CURVES_CURVES_MANAGER_H
#define ZOMBO_EDITOR_CURVES_CURVES_MANAGER_H

#include "..\..\zombo_classes.h"
#include "../../common/ZomboInterpolatedValue.h"

namespace zombo
{
	class CurvesPoint;
	class CurveSegment;

	class CurvesPoint
	{
	public:
		core::Vector2 xy;
		std::vector<CurveSegment*> segments;
		bool isHovered;
		bool isSelected;
		CurvesPoint();
		CurvesPoint(core::Vector2 p);
		float getR();
		void updateInput();
		void update();
		void setScale(float scale);
		void setAlpha(float a);
		ZomboInterpolatedValue _scale;
		ZomboInterpolatedValue _alpha;
	private:
	};

	class CurveSegment
	{
	public:
		CurvesPoint *p0;
		core::Vector2 d0;
		core::Vector2 d1;
		CurvesPoint *p1;
		void updateInput();
		void update();
	};

	class CurvesManager
	{
	public:
		static CurvesManager Default;
		core::Vector2 mousePos;
		CurvesManager();
		void update();
		void addCurve(core::Vector2 p0, core::Vector2 p1, core::Vector2 p2, core::Vector2 p3);
		ZomboInterpolatedValue scale;
	private:
		std::vector<CurvesPoint *> _points;
		std::vector<CurveSegment *> _segments;
	};
}

#endif /*ZOMBO_EDITOR_CURVES_CURVES_MANAGER_H*/
