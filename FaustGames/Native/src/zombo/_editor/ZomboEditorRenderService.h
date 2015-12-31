#ifndef ZOMBO_EDITOR_RENDER_SERVICE_H
#define ZOMBO_EDITOR_RENDER_SERVICE_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"

namespace zombo
{
	class ZomboEditorRenderBuffer;
	class ZomboEditorRenderBuffers;
	class ZomboEditorRenderEntry;

	struct ZomboEditorPrimitivesType
	{
		enum e
		{
			None,
			Edges,
			Triangles
		};
	};


	class ZomboEditorRenderService
	{
	public:
		ZomboEditorRenderService();
		~ZomboEditorRenderService();
		static ZomboEditorRenderService Default;
		void draw(ZomboEditorPrimitivesType::e type, graphics::EffectBase *effect, void* vertices, int vertexSize, int verticesCount, ushort* indices, int primitivesCount);
		void drawLines(ColorVertex* vertices, int verticesCount, ushort* indices, int primitivesCount);
		
		void applyRenderCommands();
		void resetBuffers();
	private:
		ZomboEditorRenderBuffer *getBuffer(int vertexSize, int verticesCount, int indicesCount);
		ZomboEditorRenderBuffers *getBuffers(int vertexSize);

		std::vector<ZomboEditorRenderBuffers *> _buffers;
		std::vector<ZomboEditorRenderBuffers *> _existingBuffers;
		std::vector<ZomboEditorRenderEntry> _entries;
		int _entriesCount;
	};

	class ZomboEditorRenderEntry
	{
	public:
		ZomboEditorPrimitivesType::e type;
		graphics::EffectBase* effect;
		ZomboEditorRenderBuffer* buffer;
		ushort *indices;
		int indicesCount;
		ZomboEditorRenderEntry();
	};

	class ZomboEditorRenderBuffers
	{
	public:	
		ZomboEditorRenderBuffers(int vertexSize);
		~ZomboEditorRenderBuffers();
		ZomboEditorRenderBuffer *getBuffer(int verticesCount, int indicesCount);
		void reset();
	private:
		std::vector<ZomboEditorRenderBuffer *> _items;
		int _vertexSize;
		int _count;
	};
	
	class ZomboEditorRenderBuffer
	{
	public:
		const int VerticesLimit = 32768;
		const int IndicesLimit = 32768;
		ZomboEditorRenderBuffer(int vertexSize);
		~ZomboEditorRenderBuffer();
		bool canAdd(int verticesCount, int indicesCount) const;
		ushort *add(void* vertices, int verticesCount, ushort* indices, int indicesCount);
		void reset();
		void *getBuffer() const;
	private:
		int _vertexSize;
		void *_buffer;
		int _verticesCount;
		ushort *_indices;
		int _indicesCount;
	};

}

#endif /* ZOMBO_EDITOR_RENDER_SERVICE_H */