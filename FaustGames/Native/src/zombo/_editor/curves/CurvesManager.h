#ifndef ZOMBO_EDITOR_CURVES_CURVES_MANAGER_H
#define ZOMBO_EDITOR_CURVES_CURVES_MANAGER_H

#include "../../zombo_classes.h"
#include "../../common/ZomboInterpolatedValue.h"
#include "../commands/ZomboEditorCommand.h"
#include "../../content/ZomboAtlas.h"

namespace zombo
{
	class CurvesPoint;
	class CurveSegment;
	class CurvePointBinding;

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

	class ZomboCommandMoveCurveBinding : public ZomboEditorCommand
	{
	public:
		ZomboCommandMoveCurveBinding(CurvePointBinding *binding, const core::Vector2 prevPosition, const core::Vector2 newPosition);
		void invalidate(const core::Vector2 newPosition);

		virtual bool isExecutionAvaliable() OVERRIDE;
		virtual bool isUndoAvaliable() OVERRIDE;
		virtual void execute() OVERRIDE;
		virtual void undo() OVERRIDE;
	private:
		core::Vector2 _prevPosition;
		core::Vector2 _targetPosition;
		CurvePointBinding* _binding;
	};

	struct CurvesPointLock
	{
		enum e
		{
			None,
			Direction,
			All
		};
	};

	class CurvesPoint
	{
	public:
		CurvesPointLock::e lock;
		core::Vector2 xy;
		std::vector<CurvePointBinding*> bindings;
		core::Vector2 getXY() const;
		core::Vector2 getTargetXY() const;
		CurvesPoint();
		CurvesPoint(core::Vector2 p);
		void removeBinding(CurvePointBinding* b);
		void invalidateBindings(CurvePointBinding* b);
		void invalidateBindingsLen(float l);
		static float getR();
		bool isUnderMouse() const;
		float distanceToMouse() const;
		geometry::Aabb2d getAabb() const;
		static float r;
		static core::Vector2 size;
	private:
	};

	struct BindingSnap
	{
		enum e
		{
			None,
			Edge,
		};
	};

	class CurvePointBinding
	{
	public:
		static float r;
		static core::Vector2 size;
		static float trim;

		CurvesPoint *p;
		core::Vector2 d;
		CurveSegment *segment;
		BindingSnap::e snap;
		CurvePointBinding();
		CurvePointBinding(CurvesPoint *point, const core::Vector2 &direction);
		core::Vector2 getWorldP() const;
		core::Vector2 getWorldD() const;
		void setWorldP(core::Vector2 p);
		core::Vector2 getLocalP(core::Vector2 p) const;
		bool isUnderMouse() const;
		float distanceToMouse() const;
		/*
		void updateRegularState();
		void updateHoverState();
		core::Vector2 getTargetXY() const;
		core::Vector2 getXY() const;
		core::Vector2 calcD(const core::Vector2 &p) const;
		void setTargetXY(const core::Vector2 &p);
		void setXY(const core::Vector2 &p);
		void updateSelectedState();
		static float getR();
		*/
		void update(float scale, float alpha);
		core::Vector2 calcTangent() const;
		core::Vector2 getTangent() const;
		/*
						void setScale(float a);
						void setRot(float rot);
						geometry::Aabb2d getAabb() const;
						core::Vector2 getCurveP() const;
						core::Vector2 getCurveTargetP() const;
						core::Vector2 getArrowP() const;
						SFloat _scale;
						SFloat _rot;
						*/
	};
	
	class CurveSegment;

	class CurveSegmentRenderer
	{
	public:
		static CurveSegmentRenderer Default;
		CurveSegmentRenderer();
		ZomboContentImage* image;
		void renderTile(CurveSegment *segment);
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
		~CurveSegment();
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

	struct CurvesPointAnimator
	{
		CurvesPoint *point;
		SFloat scale;
		SFloat rotation;
		CurvesPointAnimator(CurvesPoint *point);
		bool isAnimating() const;
		void update();
	};

	struct CurvesPointState
	{
		static CurvesPointState Regular;
		static CurvesPointState Hovered;
		static CurvesPointState Selected;
		float scale;
		float rotation;
		CurvesPointState();
		CurvesPointState(float scale, float rotation);
	};

	class CurvesManager
	{
	public:
		static CurvesManager Default;
		core::Vector2 mousePos;
		CurvesManager();
		CurvesPoint *snap(core::Vector2 &p, CurvesPoint *);
		bool hasImages();
		static void snapBinding(const core::Vector2 &p0, core::Vector2 &p1);
		void load();
		void regularPointRender(CurvesPoint* point) const;
		void animatePointRender(CurvesPointAnimator* animator) const;
		void update();
		void addCurve(core::Vector2 p0, core::Vector2 p3);
		void addCurve(const std::vector<core::Vector2> &p, bool close);
		static CurvesSelection findSelection(CurvesVisibleItems &items);
		CurvesVisibleItems& getVisibleItems();
		void setState(ICurvesState* state);
		SFloat scale;
		CurvesSelection selection;
		CurvesSelection lastSelection;
		ZomboContentImage* getPointRingImage() const;
		ZomboContentImage* getPointBoxImage() const;
		bool isRegular(CurvesPointAnimator* animator) const;
		void animateToRegular(CurvesPoint* point);
		void animateToHover(CurvesPoint* point);
		void animateToSelect(CurvesPoint* point);
		CurvesPointAnimator* findAnimator(CurvesPoint* point);
		//void animateAlpha(CurvesPoint* point, float scale);
	private:
		void animateToState(CurvesPoint* point, CurvesPointState* state);
		typedef std::map<CurvesPoint*, CurvesPointAnimator *> PointsAnimationMap;
		void queryVisibleItems(CurvesVisibleItems &items);
		std::vector<CurvesPoint *> _points;
		std::vector<CurveSegment *> _segments;
		float _snappingRange;
		ICurvesState* _actualState;
		CurvesVisibleItems _visibleItems;
		SFloat _pointsAlpha;
		SFloat _pointsSnapAlpha;
		SFloat _extraPointsAlpha;
		SFloat _pointsScale;
		SFloat _extraPointsScale;
		ZomboContentImage* _pointBoxImage;
		ZomboContentImage* _pointRingImage;
		PointsAnimationMap _pointsAnimationMap;
		std::vector<CurvesPoint *> _removeAnimationList;
		float _regularPointR;
	};
}

#endif /*ZOMBO_EDITOR_CURVES_CURVES_MANAGER_H*/
