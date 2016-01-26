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
		virtual void unpdate() OVERRIDE;
	};
}

#endif /**/