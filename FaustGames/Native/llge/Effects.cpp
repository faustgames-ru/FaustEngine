#include "Effects.h"
#include "Effect.h"

namespace graphics
{
	Effects::Effects()
	{
	}

	Effects * Effects::_instance(0);


	Effects * Effects::instance()
	{
		if (!_instance)
			_instance = new Effects();
		return _instance;
	}
}