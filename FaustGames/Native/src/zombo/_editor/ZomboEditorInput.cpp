#include "ZomboEditorInput.h"

namespace zombo
{
#define ZOMBO_MOUSE_LEFT_BUTTON 0x1
#define ZOMBO_MOUSE_RIGHT_BUTTON 0x1
#define ZOMBO_MOUSE_MIDDLE_BUTTON 0x2

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

	void ZomboEditorMouse::update(int mouseX, int mouseY, uint mouseButtons)
	{
		x = mouseX;
		y = mouseY;
		buttons = mouseButtons;
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
