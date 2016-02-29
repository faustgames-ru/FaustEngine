#ifndef ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_BINDING_H
#define ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_BINDING_H

#include "../../zombo_classes.h"
#include "CurvesManager.h"

#define ZOMBO_BINDING_SNAPPING_AXIS_COUNT 8

namespace zombo
{	

	struct BindingSnapType
	{
		enum e
		{
			None,
			Point,
			Line,
		};
	};

	class BindingSnapping
	{
	public:
		static BindingSnapping Default;
		BindingSnapping();
		void show();
		void setCenter(CurvesPoint *p);
		BindingSnapType::e snap(core::Vector2 &p);
		void update();
		void hide();
		core::Vector2 origin;
	public:
		SFloat _scale;
		CurvesPoint *_center;
		core::Vector2 _axis[ZOMBO_BINDING_SNAPPING_AXIS_COUNT];
		float _step;
		int _axisIndex;
		SFloat _snapScale;
	};

	class CurvesStateMoveBinding : public CurvesState
	{
	public:
		static CurvesStateMoveBinding Default;

		CurvePointBinding* selectedBinding;
		CurvesStateMoveBinding();
		virtual void start() OVERRIDE;
		virtual void update() OVERRIDE;
		virtual void finish() OVERRIDE;
		void setSelection(CurvePointBinding* binding);
	private:
		BindingSnapType::e _snap;
		core::Vector2 _downMousePos;
		core::Vector2 _prevSelectedPosition;
		core::Vector2 _prevPosition;
	};
	
}

#endif /*ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_BINDING_H*/