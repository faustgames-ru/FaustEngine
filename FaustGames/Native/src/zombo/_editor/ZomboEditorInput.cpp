#include "ZomboEditorInput.h"

namespace zombo
{
#define ZOMBO_MOUSE_LEFT_BUTTON 0x1
#define ZOMBO_MOUSE_RIGHT_BUTTON 0x2
#define ZOMBO_MOUSE_MIDDLE_BUTTON 0x4

	bool ZomboEditorMouse::isLeftPressed() const
	{
		return (buttons & ZOMBO_MOUSE_LEFT_BUTTON) != 0;
	}

	bool ZomboEditorMouse::isRightPressed() const
	{
		return (buttons & ZOMBO_MOUSE_RIGHT_BUTTON) != 0;
	}

	bool ZomboEditorMouse::isMiddlePressed() const
	{
		return (buttons & ZOMBO_MOUSE_MIDDLE_BUTTON) != 0;
	}

	bool ZomboEditorMouse::isPrevStateLeftPressed() const
	{
		return (prevButtonsState & ZOMBO_MOUSE_LEFT_BUTTON) != 0;
	}

	bool ZomboEditorMouse::isPrevStateRightPressed() const
	{
		return (prevButtonsState & ZOMBO_MOUSE_RIGHT_BUTTON) != 0;
	}

	bool ZomboEditorMouse::isPrevStateMiddlePressed() const
	{
		return (prevButtonsState & ZOMBO_MOUSE_MIDDLE_BUTTON) != 0;
	}

	void ZomboEditorMouse::update(int mouseX, int mouseY, uint mouseButtons)
	{		
		internalPosition = core::Vector2(static_cast<float>(mouseX), static_cast<float>(mouseY));
		internalButtons = mouseButtons;
	}

	void ZomboEditorMouse::internalUpdate()
	{
		prevPosition = position;
		prevButtonsState = buttons;
		position = internalPosition;
		buttons = internalButtons;
	}

	void ZomboEditorMouse::reset()
	{
		internalButtons = 0;
	}

	ZomboEditorInput ZomboEditorInput::Default;

	IZomboEditorMouse* ZomboEditorInput::getEditorMouse()
	{
		return &mouse;
	}

	extern "C" DLLEXPORT IZomboEditorInput* API_CALL getZomboEditorInput()
	{
		return &ZomboEditorInput::Default;
	}

}
