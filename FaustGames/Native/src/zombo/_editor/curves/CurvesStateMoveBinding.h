#ifndef ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_BINDING_H
#define ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_BINDING_H

#include "../../zombo_classes.h"
#include "CurvesManager.h"

namespace zombo
{
	class CurvesStateMoveBinding : public CurvesState
	{
	public:
		static CurvesStateMoveBinding Default;

		CurvePointBinding* selectedBinding;
		CurvesStateMoveBinding();
		virtual void start() OVERRIDE;
		virtual void update() OVERRIDE;
		void setSelection(CurvePointBinding* binding);
	private:
		core::Vector2 _downMousePos;
		core::Vector2 _prevSelectedPosition;
	};
}

#endif /*ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_BINDING_H*/