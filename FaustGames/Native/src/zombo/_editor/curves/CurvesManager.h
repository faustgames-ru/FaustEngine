#ifndef ZOMBO_EDITOR_CURVES_CURVES_MANAGER_H
#define ZOMBO_EDITOR_CURVES_CURVES_MANAGER_H

#include "../../zombo_classes.h"
#include "../../common/ZomboInterpolatedValue.h"
#include "../commands/ZomboEditorCommand.h"

namespace zombo
{
	class CurvesPoint;
	class CurveSegment;

	class ICurvesState: public IBaseObject
	{
	public:
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void finish() = 0;
	};

	class CurvesState : public ICurvesState
	{
	public:
		virtual void start() OVERRIDE;
		virtual void update() OVERRIDE;
		virtual void finish() OVERRIDE;
	};

	class ZomboCommandReplaceCurvePoint : public ZomboEditorCommand
	{
	public:
		//ZomboCommandReplaceCurvePoint(CurveSegment *segment, uint pointIndex, CurvesPoint *newPoint);
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
		SVector2 xy;
		std::vector<CurveSegment*> segments;
		core::Vector2 getXY() const;
		core::Vector2 getTargetXY() const;
		CurvesPoint();
		CurvesPoint(core::Vector2 p);
		static float getR();
		bool isUnderMouse() const;
		float distanceToMouse() const;
		void updateCoords();
		void update(float scale, float alpha);
		void setScale(float scale);
		void setScaleEx(float scale);
		void setScaleFullEx(float scale);
		void setAlpha(float a);
		void setRot(float rot);
		void updateSelectedState();
		void updateHoverState();
		void updateRegularState();
		void updateHidenState();
		geometry::Aabb2d getAabb();
		float getScale() const;
		float getScaleEx() const;
		SFloat _scale;
		SFloat _scaleEx;
		SFloat _alpha;
		SFloat _rot;
		static float r;
		static core::Vector2 size;
	private:
	};

	class CurvePointBinding
	{
	public:
		static float r;
		static core::Vector2 size;

		CurvesPoint *p;
		SVector2 d;
		CurvePointBinding();
		CurvePointBinding(CurvesPoint *point, const core::Vector2 &direction);
		bool isUnderMouse() const;
		float distanceToMouse() const;
		void updateRegularState();
		void updateHoverState();
		core::Vector2 getXY() const;
		void setXY(const core::Vector2 &p);
		void updateSelectedState();
		static float getR();
		void updateCoords();
		void update(float scale, float alpha);
		void setScale(float a);
		void setRot(float rot);
		geometry::Aabb2d getAabb() const;
		SFloat _scale;
		SFloat _rot;
	};

	class CurveSegment
	{
	public:
		CurvePointBinding *p0;
		CurvePointBinding *p1;
		core::Vector2 get0() const;
		core::Vector2 get1() const;
		core::Vector2 get2() const;
		core::Vector2 get3() const;
		CurveSegment();
		void updateInput();
		void updateCoords();
		void update();
		uint calcDetail() const;
		float getBaseLen() const;
		float getR() const;
		bool isUnderMouse() const;
		float distanceToMouse() const;
		void updateRegularState();
		void updateHoverState();
		geometry::Aabb2d getAabb() const;
		uint color;
	private:
		SFloat _scale;
	};

	class CurvesVisibleItems
	{
	public:
		std::vector<CurvesPoint *> points;
		std::vector<CurveSegment *> segments;
		std::vector<CurvePointBinding  *> pointsBindings;
		void clear();
	};
	class CurvesSelection
	{
	public:
		CurvesPoint* point;
		CurveSegment* segment;
		CurvePointBinding* binding;
		bool isEmpty() const;
		CurvesSelection();
		CurvesSelection(CurvesPoint* p);
		CurvesSelection(CurveSegment* s);
		CurvesSelection(CurvePointBinding* b);
	};

	class CurvesManager
	{
	public:
		static CurvesManager Default;
		core::Vector2 mousePos;
		CurvesManager();
		CurvesPoint *snap(core::Vector2 &p, CurvesPoint *);
		void update();
		void addCurve(core::Vector2 p0, core::Vector2 p1, core::Vector2 p2, core::Vector2 p3);
		static CurvesSelection findSelection(CurvesVisibleItems &items);
		CurvesVisibleItems& getVisibleItems();
		void setState(ICurvesState* state);
		SFloat scale;
		CurvesSelection selection;
	private:
		void queryVisibleItems(CurvesVisibleItems &items);
		std::vector<CurvesPoint *> _points;
		std::vector<CurveSegment *> _segments;
		float _snappingRange;
		ICurvesState* _actualState;
		CurvesVisibleItems _visibleItems;
		SFloat _pointsAlpha;
		SFloat _extraPointsAlpha;
		SFloat _pointsScale;
		SFloat _extraPointsScale;
	};
}

#endif /*ZOMBO_EDITOR_CURVES_CURVES_MANAGER_H*/
