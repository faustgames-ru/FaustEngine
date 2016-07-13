#include "RenderVertex.h"
#include "../graphics.h"

namespace graphics
{
	VertexFormat* SpriteVertex::format()
	{
		return VERTEX_FORMATS::positionColorTexture();
	}
}
