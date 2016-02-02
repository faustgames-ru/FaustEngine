#include "CurvesStateMoveBinding.h"
#include "../ZomboEditorInput.h"
#include "CurvesStateSelect.h"

namespace zombo
{
	CurvesStateMoveBinding CurvesStateMoveBinding::Default;

	CurvesStateMoveBinding::CurvesStateMoveBinding(): selectedBinding(nullptr)
	{
	}

	void CurvesStateMoveBinding::start()
	{
		_downMousePos = CurvesManager::Default.mousePos;
		_prevSelectedPosition = selectedBinding->getXY();
	}

	void CurvesStateMoveBinding::update()
	{
		bool isLeftPressed = ZomboEditorInput::Default.mouse.isLeftPressed();
		core::Vector2 mousePos = CurvesManager::Default.mousePos;
		if (isLeftPressed)
		{
			core::Vector2 md = mousePos - _downMousePos;
			core::Vector2 sp = _prevSelectedPosition + md;
			
			selectedBinding->setXY(sp);
			
			// todo: snapping
			selectedBinding->updateSelectedState();
		}
		else
		{	
			// todo: undo action
			CurvesManager::Default.setState(&CurvesStateSelect::Default);
		}
	}

	void CurvesStateMoveBinding::setSelection(CurvePointBinding* binding)
	{
		selectedBinding = binding;
	}
}
