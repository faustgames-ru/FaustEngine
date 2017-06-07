#ifndef BATCHER_H
#define BATCHER_H

#include "drawing_classes.h"
#include "PostProcess.h"
#include "../core/DebugRender.h"
#include <set>
#include "DynamicVertexLightMap.h"

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
		int ColorTransformIndex;
		int RenderTargetIndex;
		graphics::BlendState::e Blend;
		graphics::EffectBase *Effect;
	};

	typedef Mesh2dVertex TVertex;

	class ColorCorrection
	{
	public:
		float saturation;
		float hue;
		float minL;
		float maxL;
		float midL;
	};

	class ColorTrasform
	{
	public:
		uint trasform(core::Vector3 v, uint color);
	};

	class BatchBuffer
	{
	public:
		const int VerticesLimit = 32768;
		const int IndicesLimit = 32768;
		graphics::TextureTransform textureTransform;
		ILightMap* lightMap;
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
	typedef std::vector<core::Matrix3Container> TBatchColorTransforms;
	typedef std::vector<BatchBuffer *> TBatchBuffers;
	typedef std::vector<BatchEntry> TBatchEntries;
	
	class EntriesGroup
	{
	public:
		TBatchEntries Entries;
		int transformIndex;
		EntriesGroup(int index);
	};
	
	typedef std::vector<EntriesGroup *> TBatchEntriesGroup;

	class RenderBuffer
	{
	public:
		TBatchBuffers Buffers;
		TBatchEntriesGroup EntriesGroups;
		TBatchTransforms Transforms;
		TBatchColorTransforms ColorTransforms;
		TBatchRenderTargets RenderTargets;
		RenderBuffer();
		~RenderBuffer();
		void addEntry(const BatchEntry& entry);
		void clearEntries();
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
		graphics::Texture* texture;
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

	struct BatcherRenderArgs
	{
		bool usePostProcess;
		RenderBuffer *renderBuffer;
		BatcherRenderArgs();
	};

	class BatcherRenderController;

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
		void setupUVTransform(llge::ITexture* texture);
		void cleanupUVTransform();

		void executeRenderCommands(bool usePostProcess);
		void internalExecuteRenderCommands(BatcherRenderArgs e);
		void addColorTransform(const core::Matrix3 &value);

		void mainRender(BatcherRenderArgs e);
		void guiRender(BatcherRenderArgs e);
		void renderTranformGroup(int groupIndex, BatcherRenderArgs e);

		virtual IntPtr API_CALL getNativeInstance() OVERRIDE;
		void applyEntry();
		virtual void API_CALL setLightingMode(llge::BatcherLightingMode mode) override;
		virtual void API_CALL addProjection(void* floatMatrix) OVERRIDE;
		virtual void API_CALL addRenderTarget(IntPtr renderTargetInstance) OVERRIDE;
		virtual void API_CALL setupLighting(IntPtr lightingConfig) OVERRIDE;
		virtual void API_CALL startBatch() OVERRIDE;
		virtual void API_CALL finishBatch() OVERRIDE;
		virtual void API_CALL draw(IntPtr batcherConfig, IntPtr texturesConfig) OVERRIDE;
		virtual void API_CALL drawSolid(int z, llge::ITexture* textureId, uint lightmapId, void *vertices, int verticesCount, void *indices, int indicesCount, byte colorScale) OVERRIDE;
		virtual void API_CALL execute(bool usePostProcess) OVERRIDE;
		virtual void API_CALL setToneMap(uint tonemapId) OVERRIDE;
		virtual int API_CALL getRenderedVerticesCount() OVERRIDE;
		virtual int API_CALL getRenderedPrimitivesCount() OVERRIDE;
		virtual void API_CALL setBatcherMode(llge::BatcherMode mode) OVERRIDE;

		virtual void drawEdge(uint color, const core::Vector3 &a, const core::Vector3 &b) OVERRIDE;
		bool usedPostProcess();
		graphics::Texture* getBlurMap();
	private:
		llge::BatcherLightingMode _lightingMode;
		ILightMap* _lightingModes[32];
		DynamicVertexLightMap _lightMap;
		EmptyLightMap _lightMapEmpty;
		BatcherDebugRender _debugRender;
		RenderBuffer *_buffer;
		uint _tonemapId;
		BatchEntry _currentEntry;
		int _batchBufferIndex;

		llge::LightingConfig _lightingConfig; // todo: remove
		core::MatrixContainer _projection;
		core::Matrix3Container _colorTransform;
		graphics::BlendState::e _blend;
		graphics::EffectBase *_effect;
		uint _config[CONFIG_MAX_SIZE];
		graphics::GraphicsDevice * _graphicsDevice;
		graphics::VertexFormat * _format;
		graphics::RenderConverter _converter;
		PostProcessBloom _bloom;
		graphics::TextureTransform _textureTransform;
		//TVertex* _localBuffer;

		int _primitivesCount;
		int _verticesCount;
		int _verticesCounter;
		ZBatcher* _zButcher;
		bool _usedPostProcess;
		core::Matrix3Container _emptyColorTransformContainer;
		BatcherRenderController* _controller;
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
		graphics::TextureTransform textureTransform;
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
		graphics::TextureTransform textureTransform;
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
		graphics::TextureTransform textureTransform;
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

	class BatcherRenderState;
	class BatcherStateDefault;
	class BatcherStateBlurSnapShot;
	class BatcherStateBlur;
	class BatcherStateBlurHide;


	class BatcherRenderController
	{
	public:
		llge::BatcherMode Mode;
		float lighting;
		BatcherRenderArgs* args();
		Batcher* batcher();
		void setState(BatcherRenderState *state);
		void update(BatcherRenderArgs e);
		void drawSnapShot(graphics::IPostProcessTarget *snapShot);
		void drawSnapShot(graphics::IPostProcessTarget *snapShot, int color);

		explicit BatcherRenderController(Batcher* batcher);

		template<typename T>
		T* CreateState();
		BatcherStateDefault* Default;
		BatcherStateBlurSnapShot* BlurSnapShot;
		BatcherStateBlur* Blur;
		BatcherStateBlurHide* Hide;

		graphics::IPostProcessTarget *popSnapShot();
		graphics::IPostProcessTarget *popBlurShot();
		void swapShots();
		void pushSnapShot();
		void pushBlurShot();
	private:
		void constructState(BatcherRenderState* state);
		BatcherRenderArgs _args;
		BatcherRenderState *_state;
		Batcher* _batcher;
		graphics::IPostProcessTarget *_snapShot;
		graphics::IPostProcessTarget *_blurShot;
		core::MatrixContainer identity;
	};

	template <typename T>
	T* BatcherRenderController::CreateState()
	{
		T* state = new T();
		constructState(state);
		return state;
	}

	class BatcherRenderState
	{
	public:
		BatcherRenderState();
		virtual ~BatcherRenderState();
		BatcherRenderController *controller();
		BatcherRenderArgs* args();
		Batcher* batcher();
		void setState(BatcherRenderState *state);
		virtual void update();
		virtual void activated();
		virtual void deactivated();
		void construct(BatcherRenderController* controller);
	private:
		BatcherRenderController* _controller;
	};

	class BatcherStateDefault : public BatcherRenderState
	{
	public:
		virtual void activated() override;
		virtual void update() override;
		int _frameCounter;
	};

	class BatcherStateBlurSnapShot: public BatcherRenderState
	{
	public:
		virtual void activated() override;
		virtual void update() override;
	private:
	};

	class BatcherStateBlur : public BatcherRenderState
	{
	public:
		virtual void activated() override;
		virtual void update() override;
		int _iterationIndex;
		int _iterationsMax;
		static core::Vector2 _pixelOffset[2];
	};

	class BatcherStateBlurHide : public BatcherRenderState
	{
	public:
		virtual void activated() override;
		virtual void update() override;
		int _iterationsMax;
		int _frameCounter;
	};
}

#endif /*BATCHER*/