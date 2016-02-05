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

	void ZomboEditorMouse::handle(void* handler)
	{
		_handler = handler;
	}

	ZomboEditorMouse* ZomboEditorMouse::query(void* handler)
	{
		if (_handler == nullptr) return this;
		if (handler == _handler) return this;
		return nullptr;
	}

	ZomboEditorInput ZomboEditorInput::Default;


	ZomboEditorMouse* ZomboEditorInput::queryMouse(void* handler)
	{
		return _mouse.query(handler);
	}

	ZomboEditorMouse* ZomboEditorInput::mouse()
	{
		return &_mouse;
	}

	IZomboEditorMouse* ZomboEditorInput::getEditorMouse()
	{
		return &_mouse;
	}

	extern "C" DLLEXPORT IZomboEditorInput* API_CALL getZomboEditorInput()
	{
		return &ZomboEditorInput::Default;
	}

}
