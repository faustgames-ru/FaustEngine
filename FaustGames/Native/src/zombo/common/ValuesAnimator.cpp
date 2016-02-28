#include "ValuesAnimator.h"

namespace zombo
{
	AnimatorFloat Animators::Float;
	AnimatorVector2 Animators::Vector2;

	void Animators::update()
	{
		Float.update();
		Vector2.update();
	}
}
