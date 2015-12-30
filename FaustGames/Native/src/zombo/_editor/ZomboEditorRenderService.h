#ifndef ZOMBO_EDITOR_RENDER_SERVICE_H
#define ZOMBO_EDITOR_RENDER_SERVICE_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"

namespace zombo
{
	class ZomboEditorRenderService
	{
	public:
		static ZomboEditorRenderService Default;
		void drawLines(ColorVertex* vertices, int verticesCount, ushort* indices, int primitivesCount);
		void applyRenderCommands();
	};

	class ZomboEditorRenderBuffer;

	class ZomboEditorRenderEntry
	{
	public:
		ZomboEditorRenderBuffer* buffer;
	};

	class ZomboEditorRenderBuffer
	{
	public:
		const int VerticesLimit = 32768;
		const int IndicesLimit = 32768;
	private:
		int _vertexSize;
		void *_buffer;
		int _bufferSize;
		ushort *_indices;
		int _indicesCount;
	};

}

#endif /* ZOMBO_EDITOR_RENDER_SERVICE_H */