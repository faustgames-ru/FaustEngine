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
		ZomboEditorMouse* mouse = ZomboEditorInput::Default.queryMouse(this);
		if (mouse == nullptr) return;
		bool isLeftPressed = mouse->isLeftPressed();
		core::Vector2 mousePos = CurvesManager::Default.mousePos;
		if (isLeftPressed)
		{
			mouse->handle(this);
		}
		else
		{
			CurvesManager::Default.setState(&CurvesStateSelect::Default);
			mouse->handle(nullptr);
		}
	}

	void CurvesStateMoveSegment::setSelection(CurveSegment* segment)
	{
		selectedSegment = segment;
	}
}
