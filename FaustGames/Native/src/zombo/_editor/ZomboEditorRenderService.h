#ifndef ZOMBO_EDITOR_RENDER_SERVICE_H
#define ZOMBO_EDITOR_RENDER_SERVICE_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "../../fonts/IFontRenderer.h"
#include "../../fonts/BitmapFont.h"

namespace zombo
{
	class ZomboEditorRenderService;
	class ZomboEditorRenderBuffer;
	class ZomboEditorRenderBuffers;
	class ZomboEditorRenderEntry;

	struct ZomboEditorPrimitivesType
	{
		enum e
		{
			None,
			Edges,
			Triangles,
			TrianglesTextured
		};
	};


	class ZomboEditorBitmapFontRenderer : public fonts::IBitmapFontRenderer
	{
	public:
		ZomboEditorRenderService* renderService;
		ushort indices[6];
		RenderVertex vertices[4];
		virtual void drawQuad(uint color, graphics::Texture *texture, const fonts::BitmapFontVertex &min, const fonts::BitmapFontVertex &max, float z, bool rotate) OVERRIDE;
	};
	
	class ZomboEditorFontRenderer : public fonts::IFontRenderer
	{
	public:
		ZomboEditorRenderService* renderService;
		ushort indices[6];
		ColorVertex vertices[4];
		virtual void drawEdge(uint color, const core::Vector3 a, const core::Vector3 b) OVERRIDE;
		virtual void drawTriangle(uint color, core::Vector3 a, core::Vector3 b, core::Vector3 c) OVERRIDE;
		virtual void drawTriangle(uint ca, uint cb, uint cc, core::Vector3 a, core::Vector3 b, core::Vector3 c) OVERRIDE;
		virtual void drawQuad(uint ca, uint cb, uint cc, uint cd, core::Vector3 a, core::Vector3 b, core::Vector3 c, core::Vector3 d) OVERRIDE;
	};

	class ZomboEditorRenderService
	{
	public:
		ZomboEditorFontRenderer VectorFontRenderer;
		ZomboEditorBitmapFontRenderer FontRenderer;
		ZomboEditorRenderService();
		~ZomboEditorRenderService();
		static ZomboEditorRenderService Default;
		static ZomboEditorRenderService Gui;
		void draw(ZomboEditorPrimitivesType::e type, uint actualTexture, graphics::EffectBase *effect, void* vertices, int vertexSize, int verticesCount, ushort* indices, int primitivesCount);
		void drawLines(ColorVertex* vertices, int verticesCount, ushort* indices, int primitivesCount);
		void drawTriangles(ColorVertex* vertices, int verticesCount, ushort* indices, int primitivesCount);
		void drawTrianglesTextured(graphics::Texture* texture, RenderVertex* vertices, int verticesCount, ushort* indices, int primitivesCount);

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
		uint texture;
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