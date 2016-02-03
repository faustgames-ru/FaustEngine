#ifndef ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_POINT_H
#define ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_POINT_H

#include "../../zombo_classes.h"
#include "CurvesManager.h"

namespace zombo
{
	class PointSnapping
	{
	public:
		static PointSnapping Default;
		PointSnapping();
		void show();
		void snap(core::Vector2 &p);
		void update();
		void hide();
	public:
		SFloat _scale;
		SFloat _snapScaleX;
		SFloat _snapScaleY;
		float _step;
		float _size;
		float _snapX;
		float _snapY;

	};

	class CurvesStateMovePoint : public CurvesState
	{
	public:
		static CurvesStateMovePoint Default;
		
		CurvesPoint* selectedPoint;
		CurvesStateMovePoint();
		virtual void start() OVERRIDE;
		virtual void update() OVERRIDE;
		virtual void finish() OVERRIDE;
		void setSelection(CurvesPoint* point);
	private:
		core::Vector2 _downMousePos;
		core::Vector2 _prevSelectedPosition;
		CurvesPoint* _replacePoint;
	};
}

#endif /*ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_POINT_H*/