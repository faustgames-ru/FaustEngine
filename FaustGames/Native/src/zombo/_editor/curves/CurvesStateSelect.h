#ifndef ZOMBO_EDITOR_CURVES_CURVES_STATE_SELECT_H
#define ZOMBO_EDITOR_CURVES_CURVES_STATE_SELECT_H

#include "../../zombo_classes.h"
#include "CurvesManager.h"

namespace zombo
{
	class CurvesStateSelect : public CurvesState
	{
	public:	
		static CurvesStateSelect Default;
		static void updatePointState(CurvesPoint* point, const CurvesSelection &selection);
		static void updateSegmentState(CurveSegment* segment, const CurvesSelection &selection);
		static void updateBindingState(CurvePointBinding* binding, const CurvesSelection &selection);
		static void updatePoints(const std::vector<CurvesPoint *> &points, const CurvesSelection &selection);
		static void updateSegments(const std::vector<CurveSegment*> &segment, const CurvesSelection &selection);
		static void updateBinding(const std::vector<CurvePointBinding*> &bindings, const CurvesSelection &selection);
		virtual void update() OVERRIDE;
	};
}

#endif /*ZOMBO_EDITOR_CURVES_CURVES_STATE_SELECT_H*/