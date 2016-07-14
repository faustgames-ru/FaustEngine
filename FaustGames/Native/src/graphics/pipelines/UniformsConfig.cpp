#include "UniformsConfig.h"

namespace graphics
{
	void UniformConfigNone::apply()
	{
	}

	void UniformConfigTexture::apply()
	{
		graphics::UniformValues::texture()->setValue(testureId);
	}
}
