#ifndef ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_BINDING_H
#define ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_BINDING_H

#include "../../zombo_classes.h"
#include "CurvesManager.h"

#define ZOMBO_BINDING_SNAPPING_AXIS_COUNT 8

namespace zombo
{
	class BindingSnapping
	{
	public:
		static BindingSnapping Default;
		BindingSnapping();
		void show();
		void setCenter(CurvesPoint *p);
		bool snap(core::Vector2 &p);
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
		core::Vector2 _downMousePos;
		core::Vector2 _prevSelectedPosition;
	};
}

#endif /*ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_BINDING_H*/