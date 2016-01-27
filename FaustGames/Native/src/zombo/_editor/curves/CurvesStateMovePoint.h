#ifndef ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_POINT_H
#define ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_POINT_H

#include "../../zombo_classes.h"
#include "CurvesManager.h"

namespace zombo
{
	class CurvesStateMovePoint : public CurvesState
	{
	public:
		static CurvesStateMovePoint Default;
		
		CurvesPoint* selectedPoint;
		CurvesStateMovePoint();
		virtual void start() OVERRIDE;
		virtual void update() OVERRIDE;
		void setSelection(CurvesPoint* point);
	private:
		core::Vector2 _downMousePos;
		core::Vector2 _prevSelectedPosition;
		CurvesPoint* _replacePoint;
	};
}

#endif /*ZOMBO_EDITOR_CURVES_CURVES_STATE_MOVE_POINT_H*/