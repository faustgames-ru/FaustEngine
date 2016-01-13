#ifndef ZOMBO_EDITOR_RENDER_SERVICE_H
#define ZOMBO_EDITOR_RENDER_SERVICE_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "../../fonts/IFontRenderer.h"

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


	class ZomboEditorFontRenderer : public fonts::IFontRenderer
	{
	public:
		static ZomboEditorFontRenderer Default;
		ushort indices[2];
		ColorVertex vertices[2];
		virtual void drawEdge(uint color, const core::Vector3 a, const core::Vector3 b) OVERRIDE;
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
		ZomboEditorRenderBuffer *getBuffer(uint vertexSize, uint verticesCount, uint indicesCount);
		ZomboEditorRenderBuffers *getBuffers(uint vertexSize);

		std::vector<ZomboEditorRenderBuffers *> _buffers;
		std::vector<ZomboEditorRenderBuffers *> _existingBuffers;
		std::vector<ZomboEditorRenderEntry> _entries;
		uint _entriesCount;
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
		ZomboEditorRenderBuffers(uint vertexSize);
		~ZomboEditorRenderBuffers();
		ZomboEditorRenderBuffer *getBuffer(uint verticesCount, uint indicesCount);
		void reset();
	private:
		std::vector<ZomboEditorRenderBuffer *> _items;
		uint _vertexSize;
		uint _count;
	};
	
	class ZomboEditorRenderBuffer
	{
	public:
		static uint VerticesLimit;
		static uint IndicesLimit;
		ZomboEditorRenderBuffer(uint vertexSize);
		~ZomboEditorRenderBuffer();
		bool canAdd(uint verticesCount, uint indicesCount) const;
		ushort *add(void* vertices, uint verticesCount, ushort* indices, uint indicesCount);
		void reset();
		void *getBuffer() const;
	private:
		uint _vertexSize;
		void *_buffer;
		uint _verticesCount;
		ushort *_indices;
		uint _indicesCount;
	};

}

#endif /* ZOMBO_EDITOR_RENDER_SERVICE_H */