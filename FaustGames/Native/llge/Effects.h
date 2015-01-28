#ifndef EFFECTS_H
#define EFFECTS_H

#include "graphics.h"

namespace graphics
{
	class Effects
	{
	private:
		static Effects *_instance;
		Effects();
	public:
		static Effects *instance();
	};
}

#endif /*EFFECTS_H*/