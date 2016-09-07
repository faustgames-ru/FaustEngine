#include "CurvesStateSelect.h"
#include "../ZomboEditorInput.h"
#include "CurvesStateMovePoint.h"
#include "CurvesStateMoveSegment.h"
#include "CurvesStateMoveBinding.h"
#include "../menu/ContextMenu.h"
#include "../ZomboEditorCamera.h"

namespace zombo
{
	CurvesStateSelect CurvesStateSelect::Default;

	CurvesStateSelect::CurvesStateSelect(): _isRightPressed(false)
	{
	}

	void CurvesStateSelect::updatePointState(CurvesPoint *point, const CurvesSelection &selection)
	{
		if (point != selection.point)
		{
			CurvesManager::Default.animateToRegular(point);
		}
		else
		{
			CurvesManager::Default.animateToHover(point);
		}
	}

	void CurvesStateSelect::updateSegmentState(CurveSegment* segment, const CurvesSelection& selection)
	{
		if (segment != selection.segment)
		{
			segment->updateRegularState();
		}
		else
		{
			segment->updateHoverState();
		}
	}

	void CurvesStateSelect::updateBindingState(CurvePointBinding* binding, const CurvesSelection& selection)
	{
		/*
		if (binding != selection.binding)
		{
			binding->updateRegularState();
		}
		else
		{
			binding->updateHoverState();
		}
		*/
	}

	void CurvesStateSelect::updatePoints(const std::vector<CurvesPoint*> &points, const CurvesSelection& selection)
	{
		for (uint i = 0; i < points.size(); i++)
		{
			updatePointState(points[i], selection);
		}
	}

	void CurvesStateSelect::updateSegments(const std::vector<CurveSegment*> &segment, const CurvesSelection& selection)
	{
		for (uint i = 0; i < segment.size(); i++)
		{
			updateSegmentState(segment[i], selection);
		}
	}

	void CurvesStateSelect::updateBinding(const std::vector<CurvePointBinding*>& bindings, const CurvesSelection& selection)
	{
		for (uint i = 0; i < bindings.size(); i++)
		{
			updateBindingState(bindings[i], selection);
		}
	}

	void CurvesStateSelect::update()
	{
		ZomboEditorMouse *mouse = ZomboEditorInput::Default.queryMouse(this);
		if (mouse == nullptr) return;
		CurvesVisibleItems &visible = CurvesManager::Default.getVisibleItems();
		CurvesSelection selection = CurvesManager::Default.findSelection(visible);
		updatePoints(visible.points, selection);
		updateBinding(visible.pointsBindings, selection);
		updateSegments(visible.segments, selection);
		bool isLeftPressed = mouse->isLeftPressed();
		bool isRightPressed = mouse->isRightPressed();
		/*
		if (!isRightPressed)
		{
			ContextMenu::Default.hide();
		}
		*/
		if (isRightPressed && !_isRightPressed)
		{
			if (ContextMenu::Default.isVisible())
			{
				ContextMenu::Default.hide();
			}
			else
			{
				if (selection.point != nullptr)
				{
					core::Vector3 v = ZomboEditorCamera::Default.getViewPosition(selection.point->getXY().toVector3());
					ContextMenu::Default.show(v.toVector2());
				}
			}
			/*
			else
			{
				ContextMenu::Default.hide();
			}
			*/
		}
		else
		{
			if (isLeftPressed)
			{
				ContextMenu::Default.hide();
				CurvesManager::Default.selection = CurvesManager::Default.lastSelection = selection;
				if (selection.point != nullptr)
				{
					CurvesStateMovePoint::Default.setSelection(selection.point);
					CurvesManager::Default.setState(&CurvesStateMovePoint::Default);
				}
				else if (selection.binding != nullptr)
				{
					CurvesStateMoveBinding::Default.setSelection(selection.binding);
					CurvesManager::Default.setState(&CurvesStateMoveBinding::Default);
				}
				else if (selection.segment != nullptr)
				{
					CurvesStateMoveSegment::Default.setSelection(selection.segment);
					CurvesManager::Default.setState(&CurvesStateMoveSegment::Default);
					// set move segment state
				}
			}
			else
			{
				CurvesManager::Default.selection = CurvesSelection();
			}
		}
		_isRightPressed = isRightPressed;
	}
}
