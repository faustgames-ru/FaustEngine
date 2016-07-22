#ifndef GAME_INPUT_MOUSE_H
#define GAME_INPUT_MOUSE_H

#include "../game_classes.h"

namespace game
{	
	struct MouseButtons
	{
		enum e
		{
			None = 0x0,
			Left = 0x1,
			Right = 0x2,
			Mid = 0x4,
		};
	};

	class Mouse
	{
	public:
		static Mouse Default;
		float sensivity;
		float dx;
		float dy;
		int windowX;
		int windowY;
		MouseButtons::e buttons;
		bool leftPresed();
		bool rightPresed();
		Mouse();
		void update(int rawX, int rawY, int windowX, int windowY, uint buttons);
	};
}

#endif /*GAME_INPUT_MOUSE_H*/