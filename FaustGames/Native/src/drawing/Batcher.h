#ifndef BATCHER_H
#define BATCHER_H

#include "drawing_classes.h"
#include "PostProcess.h"
#include "../core/DebugRender.h"
#include <set>

namespace drawing
{
	class BatchBuffer;
	class ZBatcher;
		
	struct BatchEntry
	{
		ushort * IndicesStart;
		uint Config[CONFIG_MAX_SIZE];
		int IndicesCount;
		int BatchBufferIndex;
		int TransformIndex;
		int RenderTargetIndex;
		graphics::BlendState::e Blend;
		graphics::EffectBase *Effect;
	};

	typedef Mesh2dVertex TVertex;

	class BatchBuffer
	{
	public:
		const int VerticesLimit = 32768;
		const int IndicesLimit = 32768;
		float _x;
		float _y;
		float _w;
		float _h;
		BatchBuffer();

		~BatchBuffer();

		void reset();

		bool canAdd(int verticesCount, int indicesCount) const;
		void addMesh(uint color, float z, float* vertices, float* uvs, int verticesCount, ushort* indices, int indicesCount, bool additive, byte colorScale);
		void addMesh(uint color, float z, float* vertices, float* uvs, int verticesCount, ushort* indices, int indicesCount, bool additive, core::Matrix viewTransform, byte colorScale);
		void addMesh(TVertex* vertices, int verticesCount, ushort* indices, int indicesCount, bool additive, unsigned char colorScale);
		TVertex* getVertices();
		int getVerticesCount() const;
		ushort* getCurrentIndices() const;
		int getIndicesCount();
	private:
		TVertex *_vertices;
		ushort *_indices;
		int _indicesCount;
		int _verticesCount;
	};

	typedef std::vector<graphics::IRenderTarget *> TBatchRenderTargets;
	typedef std::vector<core::MatrixContainer> TBatchTransforms;
	typedef std::vector<BatchBuffer *> TBatchBuffers;
	typedef std::vector<BatchEntry> TBatchEntries;

	class RenderBuffer
	{
	public:
		TBatchBuffers Buffers;
		TBatchEntries Entries;
		TBatchTransforms Transforms;
		TBatchRenderTargets RenderTargets;
		RenderBuffer();
		~RenderBuffer();
	};

	struct BatcherState
	{
		graphics::EffectBase *Effect;
		graphics::BlendState::e Blend;
		void *config;
		bool isConfigEqual(void *otherConfig) const;
	};

	struct BatcherSpineMesh
	{
		BatcherState State;
		uint Color;
		float *Vertices;
		float *Uvs;
		float Z;
		int VerticesCount;
		unsigned short *Indices;
		int IndicesCount;
		BatcherSpineMesh();
		BatcherSpineMesh(int verticesLimit);
	};

	struct BatcherMesh
	{
		BatcherState State;
		TVertex *Vertices;
		int VerticesCount;
		ushort *Indices;
		int IndicesCount;
	};

	class BatcherDebugRender : public core::IDebugRender
	{
	public:
		virtual void drawEdge(uint color, const core::Vector3 &a, const core::Vector3 &b) OVERRIDE;
		void apply(graphics::GraphicsDevice *graphicsDevice, core::MatrixContainer transform);
	private:
		std::vector<SolidVertex> _edges;
	};

	class Batcher : public llge::IBatch2d, public core::IDebugRender
	{
	public:
		Batcher();
		~Batcher();
		void start();
		void finish();
		void drawMesh(graphics::EffectBase *effect, graphics::BlendState::e blend, llge::ITexture * texture, uint lightmapId, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount, float colorScale);
		void drawMesh(graphics::EffectBase *effect, graphics::BlendState::e blend, uint textureId, uint lightmapId, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount, unsigned char colorScale);
		void drawMesh(graphics::EffectBase *effect, graphics::BlendState::e blend, void* config, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount, unsigned char colorScale);
		void drawSpineMesh(const BatcherSpineMesh &mesh, byte colorScale);
		void drawSpineMesh0(const BatcherSpineMesh &mesh, byte colorScale);

		void executeRenderCommands(bool usePostProcess);

		virtual IntPtr API_CALL getNativeInstance() OVERRIDE;
		void applyEntry();
		virtual void API_CALL addProjection(void* floatMatrix) OVERRIDE;
		virtual void API_CALL addRenderTarget(IntPtr renderTargetInstance) OVERRIDE;
		virtual void API_CALL startBatch() OVERRIDE;
		virtual void API_CALL finishBatch() OVERRIDE;
		virtual void API_CALL drawEx(llge::GraphicsEffects effect, llge::BlendMode blendMode, IntPtr config, void* vertices, int verticesCount, void* indices, int indicesCount, unsigned char colorScale) OVERRIDE;
		virtual void API_CALL draw(llge::GraphicsEffects effect, llge::BlendMode blendMode, llge::ITexture* texture, uint lightmapId, void* vertices, int verticesCount, void* indices, int indicesCount, unsigned char colorScale) OVERRIDE;
		virtual void API_CALL drawSolid(int z, llge::ITexture* textureId, uint lightmapId, void *vertices, int verticesCount, void *indices, int indicesCount, byte colorScale) OVERRIDE;
		virtual void API_CALL execute(bool usePostProcess) OVERRIDE;
		virtual void API_CALL setToneMap(uint tonemapId) OVERRIDE;
		virtual int API_CALL getRenderedVerticesCount() OVERRIDE;
		virtual int API_CALL getRenderedPrimitivesCount() OVERRIDE;
		virtual void drawEdge(uint color, const core::Vector3 &a, const core::Vector3 &b) OVERRIDE;
	private:
		BatcherDebugRender _debugRender;
		RenderBuffer *_buffer;
		uint _tonemapId;
		BatchEntry _currentEntry;
		int _batchBufferIndex;

		llge::LightingConfig _lightingConfig; // todo: remove
		core::MatrixContainer _projection;
		graphics::BlendState::e _blend;
		graphics::EffectBase *_effect;
		uint _config[CONFIG_MAX_SIZE];
		graphics::GraphicsDevice * _graphicsDevice;
		graphics::VertexFormat * _format;
		graphics::RenderConverter _converter;
		PostProcessBloom _bloom;
		float _x;
		float _y;
		float _w;
		float _h;
		//TVertex* _localBuffer;

		int _primitivesCount;
		int _verticesCount;
		int _verticesCounter;
		ZBatcher* _zButcher;
	};
	
	struct ZBatchEntry
	{
		TVertex* originVertices;
		uint Config[CONFIG_MAX_SIZE];
		int indicesStart;
		int indicesCount;
		int verticesStart;
		int verticesCount;
		bool equals(const ZBatchEntry &entry);
	};
	
	class ZBatchBuffer
	{
	public:
		ZBatchBuffer();
		~ZBatchBuffer();
		void reset();
		void add(TVertex *vertices, int verticesCount, ushort *indices, int indicesCount, byte colorScale, ZBatchEntry& result);
		ushort *allIndices();
	private:
		std::vector<TVertex*> _vertices;
		std::vector<ushort> _indices;
		int _blockSize;
		int _verticesIndex;
		int _verticesBufferIndex;
	};

	class ZBlock
	{
	public:
		int z;
		ZBatchBuffer Buffer;
		std::vector<ZBatchEntry> Entries;
		void reconstruct(int z);
		void addMesh(llge::ITexture * texture, uint lightmapId, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount, byte colorScale);
		void applyRender(graphics::EffectBase *effect);
	private:
	};


	class ZBlocksPool
	{
	public:
		ZBlocksPool();
		~ZBlocksPool();
		void reset();
		ZBlock * queryBlock();
	private:
		int _blockSize;
		int _blocksIndex;
		int _blockIndex;
		std::vector<ZBlock*> _blocks;
	};

	class ZBatcher
	{
	public:
		void configure(
			graphics::BlendState::e blend,
			graphics::EffectBase *effect,
			core::MatrixContainer transform);
		void reset();
		void applyRender();
		void drawMesh(int z, llge::ITexture * texture, uint lightmapId, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount, byte colorScale);
	private:
		core::MatrixContainer _transform;
		graphics::BlendState::e _blend;
		graphics::EffectBase *_effect;
		float _colorScale;
		uint _config[CONFIG_MAX_SIZE];
		typedef std::map<int, ZBlock*> BlocksMap;
		std::vector<ZBlock*> _blocksList;
		ZBlock* queryBlock(int z);
		ZBlocksPool _blocksPool;
		BlocksMap _blocks;
		int _verticesCounter;
		int _indicesCounter;
	};
}

#endif /*BATCHER*/