#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "llge.h"

namespace asteroids
{
	class IAsteroidsNativeController
	{
	public:
		virtual void API_CALL load() = 0;
		virtual void API_CALL update(float delta) = 0;
		virtual void API_CALL render(float w, float h) = 0;
		virtual void API_CALL dispose() = 0;
	};

	extern "C" DLLEXPORT IAsteroidsNativeController * API_CALL createAsteroidsNativeController();
}

#endif /*ASTEROIDS_H*/