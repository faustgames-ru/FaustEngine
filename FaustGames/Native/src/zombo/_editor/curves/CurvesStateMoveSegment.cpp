#include "CurvesStateMoveSegment.h"
#include "../ZomboEditorInput.h"
#include "CurvesStateSelect.h"

namespace zombo
{
	CurvesStateMoveSegment CurvesStateMoveSegment::Default;


	CurvesStateMoveSegment::CurvesStateMoveSegment(): selectedSegment(nullptr)
	{
	}

	void CurvesStateMoveSegment::start()
	{
	}

	void CurvesStateMoveSegment::update()
	{
		bool isLeftPressed = ZomboEditorInput::Default.mouse.isLeftPressed();
		core::Vector2 mousePos = CurvesManager::Default.mousePos;
		if (isLeftPressed)
		{
		}
		else
		{
			CurvesManager::Default.setState(&CurvesStateSelect::Default);
		}
	}

	void CurvesStateMoveSegment::setSelection(CurveSegment* segment)
	{
		selectedSegment = segment;
	}
}
