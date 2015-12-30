#include "ZomboEditorRenderService.h"
#include "../../graphics/VertexFormats.h"

namespace zombo
{
	ZomboEditorRenderService ZomboEditorRenderService::Default;

	void ZomboEditorRenderService::applyRenderCommands()
	{
		//graphics::GraphicsDevice::Default.drawEdgesPrimitives(graphics::VertexFormats::positionColor(), )
	}

	bool ZomboEditorRenderBuffer::canAdd(int verticesBytes, int indicesCount) const
	{
		return ((_indicesCount + indicesCount) < IndicesLimit) && ((_bufferSize + verticesBytes) < VerticesLimit);
	}
}
