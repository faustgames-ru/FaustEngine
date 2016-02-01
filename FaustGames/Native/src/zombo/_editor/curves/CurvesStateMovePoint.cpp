#include "CurvesStateMovePoint.h"
#include "../ZomboEditorInput.h"
#include "CurvesStateSelect.h"
#include "../commands/ZomboEditorCommands.h"

namespace zombo
{
	CurvesStateMovePoint CurvesStateMovePoint::Default;

	CurvesStateMovePoint::CurvesStateMovePoint(): selectedPoint(nullptr), _replacePoint(nullptr)
	{
	}

	void CurvesStateMovePoint::start()
	{
		_downMousePos = CurvesManager::Default.mousePos;
		_prevSelectedPosition = selectedPoint->getXY();
		_replacePoint = nullptr;
	}

	void CurvesStateMovePoint::update()
	{
		bool isLeftPressed = ZomboEditorInput::Default.mouse.isLeftPressed();
		core::Vector2 mousePos = CurvesManager::Default.mousePos;
		if (isLeftPressed)
		{
			core::Vector2 md = mousePos - _downMousePos;
			core::Vector2 sp = _prevSelectedPosition + md;

			// todo: snapping
			CurvesPoint *replacePoint = CurvesManager::Default.snap(sp, selectedPoint);
			if (replacePoint != _replacePoint)
			{
				selectedPoint->xy.setTarget(sp);
			}
			else
			{
				selectedPoint->xy.setAll(sp);
			}
			_replacePoint = replacePoint;
			selectedPoint->updateSelectedState();

			CurvesVisibleItems &visible = CurvesManager::Default.getVisibleItems();
			for (uint i = 0; i < visible.points.size(); i++)
			{
				if (visible.points[i] != selectedPoint)
				{
					visible.points[i]->updateHidenState();
				}
				else
				{
					visible.points[i]->updateSelectedState();
				}
			}
		}
		else
		{
			// todo: replace command support
			ZomboCommandMoveCurvePoint *actualMovePointCommand = new ZomboCommandMoveCurvePoint(selectedPoint, _prevSelectedPosition, selectedPoint->getTargetXY());
			if (ZomboEditorCommands::Default.doCommand(actualMovePointCommand) != CommandExecutonStatus::CommandExecuted)
			{
				delete actualMovePointCommand;
			}
			CurvesManager::Default.setState(&CurvesStateSelect::Default);
		}
	}

	void CurvesStateMovePoint::setSelection(CurvesPoint* point)
	{
		selectedPoint = point;
	}
}
