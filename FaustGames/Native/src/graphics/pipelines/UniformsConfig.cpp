#include "UniformsConfig.h"

namespace graphics
{
	void UniformConfigTexture::apply()
	{
		graphics::UniformValues::texture()->setValue(testureId);
	}
}
