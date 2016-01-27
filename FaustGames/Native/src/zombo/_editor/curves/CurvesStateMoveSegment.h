#ifndef ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_CURVE_H
#define ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_CURVE_H

#include "../../zombo_classes.h"
#include "CurvesStateMovePoint.h"

namespace zombo
{
	class CurvesStateMoveSegment : public CurvesState
	{
	public:		
		static CurvesStateMoveSegment Default;
		CurveSegment* selectedSegment;
		CurvesStateMoveSegment();
		virtual void start() OVERRIDE;
		virtual void update() OVERRIDE;
		void setSelection(CurveSegment* segment);
	private:
	};
}

#endif /*ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_CURVE_H*/
