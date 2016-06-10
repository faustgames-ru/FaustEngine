#ifndef BATCHER_H
#define BATCHER_H

#include "drawing_classes.h"
#include "PostProcess.h"
#include "../core/DebugRender.h"

namespace drawing
{
	class BatchBuffer;
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
		void addMesh(uint color, float z, float* vertices, float* uvs, int verticesCount, int* indices, int indicesCount, bool additive);
		void addMesh(TVertex* vertices, int verticesCount, ushort* indices, int indicesCount, bool additive);
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
		int *Indices;
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
		void drawMesh(graphics::EffectBase *effect, graphics::BlendState::e blend, llge::ITexture * texture, uint lightmapId, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount);
		void drawMesh(graphics::EffectBase *effect, graphics::BlendState::e blend, uint textureId, uint lightmapId, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount);
		void drawMesh(graphics::EffectBase *effect, graphics::BlendState::e blend, void* config, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount);
		void drawSpineMesh(const BatcherSpineMesh &mesh);
		
		void executeRenderCommands(bool usePostProcess);

		virtual IntPtr API_CALL getNativeInstance() OVERRIDE;
		void applyEntry();
		virtual void API_CALL addProjection(void* floatMatrix) OVERRIDE;
		virtual void API_CALL addRenderTarget(IntPtr renderTargetInstance) OVERRIDE;
		virtual void API_CALL startBatch() OVERRIDE;
		virtual void API_CALL finishBatch() OVERRIDE;
		virtual void API_CALL drawEx(llge::GraphicsEffects effect, llge::BlendMode blendMode, IntPtr config, void* vertices, int verticesCount, void* indices, int indicesCount) OVERRIDE;
		virtual void API_CALL draw(llge::GraphicsEffects effect, llge::BlendMode blendMode, llge::ITexture* texture, uint lightmapId, void* vertices, int verticesCount, void* indices, int indicesCount) OVERRIDE;
		virtual void API_CALL execute(bool usePostProcess) OVERRIDE;
		virtual void API_CALL setToneMap(uint tonemapId) OVERRIDE;

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
		TVertex* _localBuffer;
	};
}

#endif /*BATCHER*/