#include "Mouse.h"

namespace game
{
	Mouse Mouse::Default;

	bool Mouse::leftPresed()
	{
		return (buttons & MouseButtons::e::Left) != MouseButtons::e::None;
	}

	bool Mouse::rightPresed()
	{
		return (buttons & MouseButtons::e::Right) != MouseButtons::e::None;
	}

	Mouse::Mouse(): sensivity(1.0f), dx(0), dy(0), windowX(0), windowY(0), buttons(MouseButtons::e::None)
	{
	}

	void Mouse::update(int rawX, int rawY, int wX, int wY, uint b)
	{
		buttons = static_cast<MouseButtons::e>(b);
		windowX = wX;
		windowY = wY;
		dx = static_cast<float>(rawX) * sensivity;
		dy = static_cast<float>(rawY) * sensivity;
	}
}