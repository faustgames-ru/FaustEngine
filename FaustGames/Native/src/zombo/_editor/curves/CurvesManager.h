#ifndef ZOMBO_EDITOR_CURVES_CURVES_MANAGER_H
#define ZOMBO_EDITOR_CURVES_CURVES_MANAGER_H

#include "../../zombo_classes.h"
#include "../../common/ZomboInterpolatedValue.h"
#include "../commands/ZomboEditorCommand.h"

namespace zombo
{
	class CurvesPoint;
	class CurveSegment;


	class ZomboCommandReplaceCurvePoint : public ZomboEditorCommand
	{
	public:
		ZomboCommandReplaceCurvePoint(CurveSegment *segment, uint pointIndex, CurvesPoint *newPoint);
	};

	class ZomboCommandMoveCurvePoint : public ZomboEditorCommand
	{
	public:
		ZomboCommandMoveCurvePoint(CurvesPoint *point, const core::Vector2 prevPosition, const core::Vector2 newPosition);
		void invalidate(const core::Vector2 newPosition);

		virtual bool isExecutionAvaliable() OVERRIDE;
		virtual bool isUndoAvaliable() OVERRIDE;
		virtual void execute() OVERRIDE;
		virtual void undo() OVERRIDE;
	private:
		core::Vector2 _prevPosition;
		core::Vector2 _targetPosition;
		CurvesPoint* _point;
	};


	class CurvesPoint
	{
	public:
		ZomboInterpolatedValue x;
		ZomboInterpolatedValue y;
		std::vector<CurveSegment*> segments;
		core::Vector2 getXY() const;
		core::Vector2 getTargetXY() const;
		CurvesPoint();
		CurvesPoint(core::Vector2 p);
		float getR();
		bool isUnderMouse();
		float distanceToMouse() const;
		void update();
		void setScale(float scale);
		void setAlpha(float a);
		void setRot(float rot);
		void updateSelectedState();
		void updateHoverState();
		void updateRegularState();
		void updateHidenState();
		ZomboInterpolatedValue _scale;
		ZomboInterpolatedValue _alpha;
		ZomboInterpolatedValue _rot;
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
		core::Vector2 prevMousePos;
		CurvesManager();
		CurvesPoint *snap(core::Vector2 &p);
		void moveSelected();
		void update();
		void updateHover();
		void findSelected();
		void addCurve(core::Vector2 p0, core::Vector2 p1, core::Vector2 p2, core::Vector2 p3);
		ZomboInterpolatedValue scale;
	private:
		std::vector<CurvesPoint *> _points;
		std::vector<CurveSegment *> _segments;
		CurvesPoint *_selectedPoint;
		CurvesPoint *_replacePoint;
		core::Vector2 _prevSelectedPosition;
		core::Vector2 _downMousePos;
		ZomboCommandMoveCurvePoint* _actualMovePointCommand;
		float _snappingRange;
	};
}

#endif /*ZOMBO_EDITOR_CURVES_CURVES_MANAGER_H*/
