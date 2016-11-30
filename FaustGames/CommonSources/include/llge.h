#ifndef LLGE_H
#define LLGE_H

#ifdef __ANDROID__
#include <jni.h>
#include <math.h>
#define API_CALL JNICALL
#define DLLEXPORT JNIEXPORT
#define OVERRIDE override
#define nullptr 0
#else 
#ifdef __APPLE__
#define API_CALL
#define DLLEXPORT
#define OVERRIDE override
#else
#define API_CALL __stdcall
#define DLLEXPORT __declspec( dllexport )
#define OVERRIDE override
#endif
#endif

#define CONFIG_MAX_SIZE 2

#include "BaseObjects.h"

typedef unsigned long long ulong;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char byte;
typedef const char * String;
typedef void * IntPtr;

namespace llge
{
	/// graphics enums
		
	enum TextureFilterMode
	{
		Nearest = 0x0,
		Linear = 0x1,
		NearestMipmapNearest = 0x2,
		LinearMipmapNearest = 0x3,
		NearestMipmapLinear = 0x4,
		LinearMipmapLinear = 0x5,
	};

	enum BlendMode
	{
		None = 0x0,
		Alpha = 0x1,
		Additive = 0x2,
	};

	enum GraphicsEffects
	{
		EffectTextureColor = 0x0,
		EffectTextureLightmapColor = 0x1,
		EffectWater = 0x2,
		EffectSolid = 0x3,
		EffectRenderDepth = 0x4,
		EffectSolidColor = 0x5,
		EffectBloomFilter = 0x6,
		EffectTextureColorHighlight = 0x7,
		EffectTextureAlphamaskColor = 0x8,
		EffectTexturePaint = 0x9,
	};

	enum GraphicsVertexFormats
	{
		FormatPositionTextureColor = 0x0,
		FormatPositionNormal = 0x1,
		FormatPositionTexture = 0x2,
		FormatPositionColor = 0x3,
	};

	enum CubemapPlane
	{
		PositiveX = 0x0,
		NegativeX = 0x1,
		PositiveY = 0x2,
		NegativeY = 0x3,
		PositiveZ = 0x4,
		NegativeZ = 0x5,
	};

	enum TextureQueryFormat
	{
		TQFNone = 0x0,
		TQFRgba8888 = 0x1,
		TQFRgba4444 = 0x2,
		TQFPlatformCompressed = 0x3,
	};
	
	enum TextureImage2dFormat
	{
		TFRgba8888 = 0x0,
		TFRgb888 = 0x1,
		TFRgba4444 = 0x2,
		TFPvrtc12 = 0x3,
		TFPvrtc14 = 0x4,
		TFAtc = 0x5,
		TFEtc2 = 0x6,
		TFAstc = 0x7,
		TFEnumSize = 0x8
	};
	
	/// entities enums
	enum ComponentsTypes
	{
		Aadd2d = 0x1,
		Transform2d = 0x2,
		Render2d = 0x4,
		MatrixTransform = 0x8,
	};

	/// physics enums
	enum PhysicalBodyType
	{
		Static = 0x1,
		Dynamic = 0x2,
		Kinematic = 0x3,
	};

	/// terrain enums
	enum TerrainStripeConfigType
	{
		Left = 0x1,
		Right = 0x2,
		Top = 0x3,
		Bottom = 0x4,
	};

	enum TerrainStripePhysicsType
	{
		PhysicsNone = 0x0,
		PhysicsTop = 0x1,
		PhysicsBottom = 0x2,
		PhysicsLeft = 0x4,
		PhysicsRight = 0x8,
		PhysicsAll = 0xff,
	};

	enum BoneFx
	{
		BoneFxNone = 0x0,
		BoneFxIgnoreLight = 0x1,
		BoneFxBlur = 0x2,
	};

	/// physics structs
	struct PhysicsFixtureConfig
	{
		float density;
		float friction;
		float bonce;
		uint collidesWith;
		uint collisionGroup;
		uint isSensor;
	};
	
	struct RayCastResult
	{
		float resultPositionX;
		float resultPositionY;
		float resultNormalX;
		float resultNormalY;
		uint resultColisionGroup;
		uint resultRaycastGroup;
	};


	/// graphics structs
	struct GraphicsConfig
	{
		bool generateMipmaps;
		bool earlyDepthPath;
		bool enableFog;
		bool earlyFragmentTestsShaderCode;
		uint texturesFilter;
		int mipmapsLevel;
		int bloomDownsample;
		bool refraction;
		float refractionScale;
		float refractionVelocityX;
		float refractionVelocityY;
		bool vignetting;
		float vignettingR0;
		float vignettingR1;
		float vignettingR2;
		uint vignettingColor0;
		uint vignettingColor1;
		uint vignettingColor2;
		float ellapsedTime;
		float postEffectsScale;
	};
	
	struct EffectConfig
	{
		uint texture;
	};

	struct LightingConfig
	{
		uint texture;
		uint lightmap;
	};

	struct HighlightConfig
	{
		uint texture;
		uint highlightColor;
	};
	
	struct BatcherConfig
	{
		int effect;
		int blendMode;
		IntPtr vertices;
		int verticesCount;
		IntPtr indices;
		int indicesCount;
		int colorScale;
		IntPtr texture;
	};

	class ITexture : IBaseObject
	{
	public:
		virtual uint API_CALL getId() = 0;
		virtual IntPtr API_CALL getTextureInstance() = 0;
	};

	class ITextureImage2d : IBaseObject
	{
	public:
		virtual IntPtr API_CALL getTextureImageInstance() = 0;
		virtual ITexture* API_CALL getTexture() = 0;
		virtual void API_CALL LoadPixels(int width, int height, TextureImage2dFormat format, void *pixels) = 0;
		virtual void API_CALL create() = 0;
		virtual void API_CALL cleanup() = 0;
		virtual void API_CALL dispose() = 0;

		virtual int API_CALL getVerticesCount() = 0;
		virtual IntPtr API_CALL getVertices() = 0;
		virtual int API_CALL getIndicesCount() = 0;
		virtual IntPtr API_CALL getIndices() = 0;
	};

	class IRenderTarget2d : IBaseObject
	{
	public:
		virtual IntPtr API_CALL getRenderTargetInstance() = 0;
		virtual ITexture* API_CALL getTexture() = 0;
		virtual void API_CALL create(int width, int height) = 0;
		virtual void API_CALL cleanup() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IRenderTargetDepth2d : IBaseObject
	{
	public:
		virtual IntPtr API_CALL getRenderTargetInstance() = 0;
		virtual ITexture* API_CALL getTexture() = 0;
		virtual ITexture* API_CALL getDepthTexture() = 0;
		virtual void API_CALL create(int width, int height) = 0;
		virtual void API_CALL cleanup() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IUniformsFacade : IBaseObject
	{
	public:
		virtual void API_CALL resetSamplers() = 0;
		virtual void API_CALL setTime(float value) = 0;
		virtual void API_CALL setNormalmap(ITexture *texture) = 0;
		virtual void API_CALL setEnvironment(ITexture *texture) = 0;
		virtual void API_CALL setDepthmap(ITexture *texture) = 0;
		virtual void API_CALL setTexture(ITexture *texture) = 0;
		virtual void API_CALL setLightMap(ITexture *texture) = 0;
		virtual void API_CALL setProjection(void *floatMatrix) = 0;
		virtual void API_CALL setFogStart(float value) = 0;
		virtual void API_CALL setFogDensity(float value) = 0;
		virtual void API_CALL setFogScale(float value) = 0;
		virtual void API_CALL setFogColor(float r, float g, float b) = 0;
	};

	class IVertexBuffer : IBaseObject
	{
	public:
		virtual int API_CALL getId() = 0;
		virtual void API_CALL create() = 0;
		virtual void API_CALL setData(void *data, int count) = 0;
		virtual void API_CALL cleanup() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IGraphicsFacade : IBaseObject
	{
	public:
		virtual IUniformsFacade * API_CALL getUniforms() = 0;

		virtual void API_CALL viewport(int width, int height) = 0;
		virtual void API_CALL setRenderTarget(void *renderTargetInstance) = 0;
		virtual void API_CALL setClearState(uint color, float depth) = 0;
		virtual void API_CALL setBlendMode(BlendMode blendMode) = 0;
		virtual void API_CALL clear() = 0;
		virtual void API_CALL clearDepth() = 0;

		virtual void API_CALL resetDrawCallsCounter() = 0;
		virtual int API_CALL getDrawCallsCounterValue() = 0;

		virtual void API_CALL drawEdges(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void *vertices, int primitivesCount) = 0;
		virtual void API_CALL draw(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void *vertices, int primitivesCount) = 0;
		virtual void API_CALL drawElements(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void *vertices, void *indices, int primitivesCount) = 0;
		virtual void API_CALL drawVertexBuffer(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IVertexBuffer *vertexBuffer, void *indices, int primitivesCount) = 0;
		virtual void API_CALL setEffectConstantFloat(GraphicsEffects effect, char *name, float value) = 0;
		virtual void API_CALL setEffectConstantColor(GraphicsEffects effect, char *name, uint value) = 0;
		virtual int API_CALL getPixelsWidth() = 0;
		virtual int API_CALL getPixelsHeight() = 0;
		virtual void API_CALL getPixels(IntPtr target) = 0;

        virtual void API_CALL create() = 0;
		virtual void API_CALL grabDefaultRenderTarget() = 0;
		virtual void API_CALL cleanup() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IGraphicsFactory : IBaseObject
	{
	public:
		virtual IGraphicsFacade * API_CALL createGraphicsFacade() = 0;
		virtual ITextureImage2d * API_CALL createTextureImage2d(bool generateMipmaps, bool useFilter) = 0;
		virtual IRenderTarget2d * API_CALL createRenderTarget2d() = 0;
		virtual IRenderTargetDepth2d * API_CALL createRenderTargetDepth2d() = 0;
		virtual IVertexBuffer * API_CALL createVertexBuffer() = 0;
		virtual void API_CALL setupGraphics(IntPtr graphicsConfig) = 0;

		virtual void API_CALL dispose() = 0;
	};

	/// geometry

	class IMarchingSquares : IBaseObject
	{
	public:
		virtual void API_CALL build(IntPtr boolPoints, int w, int h) = 0;
		virtual void API_CALL buildSolid(IntPtr boolPoints, IntPtr boolInversePoints, int w, int h) = 0;
		virtual void API_CALL collectEdges() = 0;
		virtual void API_CALL simplifyPathes(int tolerance) = 0;
		virtual void API_CALL triangulatePathes() = 0;
		virtual IntPtr API_CALL getEdges() = 0;
		virtual int API_CALL getEdgesCount() = 0;
		virtual int API_CALL getPathesCount() = 0;
		virtual int API_CALL getPathCount(int pathIndex) = 0;
		virtual IntPtr API_CALL getPath(int pathIndex) = 0;
		virtual int API_CALL getVerticesCount() = 0;
		virtual IntPtr API_CALL getVertices() = 0;
		virtual int API_CALL getIndicesCount() = 0;
		virtual IntPtr API_CALL getIndices() = 0;
		virtual int API_CALL getSolidVerticesCount() = 0;
		virtual IntPtr API_CALL getSolidVertices() = 0;
		virtual int API_CALL getSolidIndicesCount() = 0;
		virtual IntPtr API_CALL getSolidIndices() = 0;
		virtual int API_CALL getBlendVerticesCount() = 0;
		virtual IntPtr API_CALL getBlendVertices() = 0;
		virtual int API_CALL getBlendIndicesCount() = 0;
		virtual IntPtr API_CALL getBlendIndices() = 0;

		virtual void API_CALL dispose() = 0;
	};

	class IQuadTree : IBaseObject
	{
	public:
		virtual int API_CALL insert(float minX, float minY, float maxX, float maxY, int userData) = 0;
		virtual void API_CALL remove(int id) = 0;
		virtual void API_CALL query(float minX, float minY, float maxX, float maxY) = 0;
		virtual void API_CALL getQueryResults(void *intBuffer) = 0;
		virtual int API_CALL getQueryResultsCount() = 0;
		virtual int API_CALL getIterationsCount() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IMeshesResult : IBaseObject
	{
	public:
		virtual int API_CALL getMeshesCount() = 0;
		virtual int API_CALL getMeshType(int meshIndex) = 0;
		virtual int API_CALL getVerticesCount(int meshIndex) = 0;
		virtual int API_CALL getIndicesCount(int meshIndex) = 0;
		virtual void API_CALL getVertices(int meshIndex, IntPtr vertices) = 0;
		virtual void API_CALL getIndices(int meshIndex, IntPtr indices) = 0;
	};

	class IP2t : IBaseObject
	{
	public:	
		virtual void API_CALL buildContour(IntPtr vertices2f, uint count) = 0;
		virtual int API_CALL getTrianglesCount() = 0;
		virtual void API_CALL getTriangles(IntPtr triangles) = 0;
	};
	
	class ITerrainClipper : IBaseObject
	{
	public:
		virtual void API_CALL clearClipper() = 0;
		virtual void API_CALL addClipperContour(IntPtr vertices2f, uint count) = 0;
		virtual void API_CALL buildClipper(int sizeX, int sizeY, int detailX, int detailY, bool createDifference) = 0;
		virtual IMeshesResult* API_CALL getIntersectionResult() = 0;
		virtual IMeshesResult* API_CALL getDifferenceResult() = 0;
		virtual void API_CALL dispose() = 0;
	};


	class IGeometryFactory : IBaseObject
	{
	public:
		virtual IQuadTree * API_CALL createQuadTree() = 0;
		virtual IMarchingSquares * API_CALL createMarchingSquares() = 0;
		virtual ITerrainClipper * API_CALL createTerrainClipper() = 0;
		virtual IP2t * API_CALL createP2t() = 0;
		virtual void API_CALL dispose() = 0;
	};

	/// entities

	class IAabb2d : IBaseObject
	{
	public:
		virtual void API_CALL update(float minX, float minY, float maxX, float maxY, float zOrder) = 0;
	};

	class IRender2d : IBaseObject
	{
	public:
		virtual void API_CALL setMeshesCount(int meshesCount) = 0;
		virtual void API_CALL setMesh(int meshIndex, ITexture *texture, void* vertices, int verticesCount, void* indices, int indicesCount) = 0;
	};

	class IMatrixTransform : IBaseObject
	{
	public:
		virtual void API_CALL setTransform(void *floatMatrix) = 0;
	};

	class ITransform2d : IBaseObject
	{
	public:
		virtual void API_CALL setWorldPosition(float x, float y, float z) = 0;
		virtual void API_CALL setWorldRotation(float value) = 0;
		virtual void API_CALL setWorldScale(float value) = 0;
		virtual void API_CALL setLocalPivot(float x, float y, float z) = 0;
		virtual void API_CALL setLocalPosition(float x, float y, float z) = 0;
		virtual void API_CALL setLocalRotation(float value) = 0;
		virtual void API_CALL setLocalScale(float value) = 0;
	};

	class IEntity : IBaseObject
	{
	public:
		virtual IntPtr getSelfInstance() = 0;		
		virtual void setComponents(ComponentsTypes types) = 0;
		virtual IAabb2d* API_CALL getAabb2d() = 0;
		virtual IRender2d* API_CALL getRender2d() = 0;
		virtual ITransform2d* API_CALL getTransform2d() = 0;
		virtual IMatrixTransform* API_CALL getMatrixTransform() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class ICamera : IBaseObject
	{
	public:
		virtual void API_CALL setPosition(float x, float y, float z) = 0;
		virtual void API_CALL setFov(float fov) = 0;
		virtual void API_CALL setAspect(float aspect) = 0;
		virtual void API_CALL setRotation(float rotationZ) = 0;
		virtual void API_CALL setPlanes(float zn, float zf) = 0;
	};

	class IEntitiesWorld : IBaseObject
	{
	public:
		virtual ICamera * API_CALL getCamera() = 0;
		virtual void API_CALL setUnpdateBounds(float minX, float minY, float maxX, float maxY) = 0;
		virtual void API_CALL setRenderBounds(float minX, float minY, float maxX, float maxY) = 0;
		
		virtual IEntity * API_CALL createEntity() = 0;
				
		virtual int API_CALL update(float elapsed) = 0;

		virtual void API_CALL updateEntity(IEntity *entity, ComponentsTypes types) = 0;
		virtual void API_CALL addEntity(IEntity *entity) = 0;
		virtual void API_CALL removeEntity(IEntity *entity) = 0;

		virtual void API_CALL dispose() = 0;
		virtual void API_CALL clear() = 0;
	};

	class IEntitiesFactory : IBaseObject
	{
	public:
		virtual IEntitiesWorld * API_CALL createEntitiesWorld() = 0;
		virtual void API_CALL dispose() = 0;
	};

	/// drawing
	class IBatch2d : IBaseObject
	{
	public:
		virtual IntPtr API_CALL getNativeInstance() = 0;
		virtual void API_CALL addProjection(void *floatMatrix) = 0;
		virtual void API_CALL addRenderTarget(IntPtr renderTargetInstance) = 0;
		virtual void API_CALL startBatch() = 0;
		virtual void API_CALL finishBatch() = 0;
		virtual void API_CALL setToneMap(uint tonemapId) = 0;
		virtual void API_CALL draw(IntPtr batcherConfig, IntPtr texturesConfig) = 0;
		virtual void API_CALL drawSolid(int z, ITexture* textureId, uint lightmapId, void *vertices, int verticesCount, void *indices, int indicesCount, byte colorScale) = 0;
		virtual void API_CALL execute(bool usePostProcess) = 0;

		virtual int API_CALL getRenderedVerticesCount() = 0;
		virtual int API_CALL getRenderedPrimitivesCount() = 0;
	};

	/// spine
	class ISpineSkeletonBone : IBaseObject
	{
	public:
		virtual IntPtr API_CALL getName() = 0;
		virtual float API_CALL getX() = 0;
		virtual float API_CALL getY() = 0;
		virtual void API_CALL setBoneFx(BoneFx fx) = 0;
		virtual void API_CALL setBoneRgbTransfomrIndex(int index) = 0;
	};

	class ISpineSkeleton : IBaseObject
	{
	public:
		virtual float API_CALL getBoundsVertexX(int boundsIndex, int vertexIndex) = 0;
		virtual float API_CALL getBoundsVertexY(int boundsIndex, int vertexIndex) = 0;
		virtual int API_CALL getBoundsVerticesCount(int boundsIndex) = 0;
		virtual int API_CALL getBoundsCount() = 0;
		virtual ISpineSkeletonBone* API_CALL getBone(int index) = 0;
		virtual int API_CALL getBonesCount() = 0;
		virtual void API_CALL setTransform(void *floatMatrix) = 0;
		virtual void API_CALL setColor(uint color) = 0;
		virtual float API_CALL getMinX() = 0;
		virtual float API_CALL getMinY() = 0;
		virtual float API_CALL getMaxX() = 0;
		virtual float API_CALL getMaxY() = 0;
		virtual float API_CALL getZ() = 0;

		virtual void API_CALL renderEx(IBatch2d * batch, IntPtr effectConfig, GraphicsEffects effect, byte colorScale) = 0;
		virtual void API_CALL render(IBatch2d * batch, int lightmapId, GraphicsEffects effect, byte colorScale) = 0;
		virtual int API_CALL getGeometry(void *vertices, int verticeLimit, void *indices, int indicesLimit) = 0;
		virtual IntPtr API_CALL getNativeInstance() = 0;
		virtual void API_CALL updateWorldTransform() = 0;
		virtual void API_CALL setBonesToSetupPose() = 0;
		virtual void API_CALL setSlotsToSetupPose() = 0;
		virtual void API_CALL applySkin(IntPtr spineSkinNativeInstance) = 0;
		virtual void API_CALL setRgbTransform(int index, void *floatMatrix3) = 0;
		virtual void API_CALL setDefaultRgbTransform(int index) = 0;

		virtual void API_CALL dispose() = 0;
	};

	
	class ISpineSkin : IBaseObject
	{
	public:
		virtual IntPtr API_CALL getNativeInstance() = 0;
		virtual IntPtr API_CALL getName() = 0;
	};

	class ISpineDynamicSkin : IBaseObject
	{
	public:
		virtual ISpineSkin* API_CALL skinValue() = 0;
		virtual void API_CALL addSkin(ISpineSkin* skin) = 0;
	};

	class ISpineAnimation : IBaseObject
	{
	public:
		virtual IntPtr API_CALL getNativeInstance() = 0;
		virtual IntPtr API_CALL getName() = 0;
		virtual float API_CALL getDuration() = 0;
	};

	class ISpineAnimationState : IBaseObject
	{
	public:
		virtual float API_CALL getTime() = 0;
		virtual void API_CALL setTime(float time) = 0;
		virtual void API_CALL update(float delta) = 0;
		virtual void API_CALL apply(ISpineSkeleton *skeleton) = 0;
		virtual void API_CALL setAnimation(ISpineAnimation* animation, bool loop, bool normalize) = 0;
		virtual void API_CALL addAnimation(ISpineAnimation* animation, bool loop, float delay) = 0;
		virtual int API_CALL getSpineEventIndices(IntPtr indices, int limit) = 0;
		virtual int API_CALL getSpineEventsLimit() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class ISpineAnimationStateData : IBaseObject
	{
	public:
		virtual void API_CALL setMix(ISpineAnimation *from, ISpineAnimation *to, float time) = 0;
		virtual ISpineAnimationState* API_CALL createState() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class ISpineEvent : IBaseObject
	{
	public:
		virtual IntPtr API_CALL getName() = 0;
	};

	class ISpineResource : IBaseObject
	{
	public:
		virtual void API_CALL load(String atlasText, String jsonText, String dir, TextureQueryFormat format) = 0;
		virtual void API_CALL unLoad() = 0;
		virtual ISpineAnimation* API_CALL getSpineAnimation(int i) = 0;
		virtual int API_CALL getSpineAnimationsCount() = 0;
		virtual ISpineSkin* API_CALL getSpineSkin(int i) = 0;
		virtual int API_CALL getSpineSkinsCount() = 0;
		virtual ISpineEvent* API_CALL getSpineEvent(int i) = 0;
		virtual int API_CALL getSpineEventsCount() = 0;
		virtual ISpineSkeleton* API_CALL createSkeleton(void *floatMatrix) = 0;
		virtual ISpineAnimationStateData* API_CALL createStateData() = 0;
		virtual IntPtr API_CALL errorMessage() = 0;
		virtual bool API_CALL isValid() = 0;
		virtual ISpineDynamicSkin* API_CALL createDynamicSkin() = 0;
		virtual ISpineSkin* API_CALL getDefaultSkin() = 0;
		virtual void API_CALL dispose() = 0;
	};

	/// profile

	class INativeMemoryProfiler : IBaseObject
	{
	public:
		virtual int API_CALL getTexturesSize() = 0;
		virtual int API_CALL getAllocationsSize() = 0;
		virtual int API_CALL getHeapSize() = 0;
	};

	/// content
	class ITextureBuffer2d : IBaseObject
	{
	public:
		virtual TextureImage2dFormat API_CALL getFormat() = 0;
		virtual int API_CALL getWidth() = 0;
		virtual int API_CALL getHeight() = 0;
		virtual IntPtr API_CALL getPixels() = 0;
	};

	class IContentAtlasMap : IBaseObject
	{
	public:
		virtual void API_CALL resetMap() = 0;
		virtual void API_CALL addRect(char *name, int pageIndex, int x, int y, int width, int height) = 0;
		virtual void API_CALL loadTextures() = 0;
	};

	class IContentManager : IBaseObject
	{
	public:
		virtual IContentAtlasMap * API_CALL getContentAtlasMap() = 0;
		virtual void API_CALL useCompression(TextureImage2dFormat format) = 0;
		virtual void API_CALL replaceSeparator(bool value) = 0;
		virtual int API_CALL registerImage(char * name) = 0;
		virtual void API_CALL startLoad() = 0;
		virtual void API_CALL startAtlasBuild() = 0;
		virtual void API_CALL finishAtlasBuild() = 0;
		virtual void API_CALL loadImage(int id, ITextureImage2d *textureImage, int w, int h, TextureQueryFormat queryFormat) = 0;
		virtual ITextureBuffer2d * API_CALL loadBuffer(int id) = 0;
		virtual void API_CALL finishLoad() = 0;
		virtual bool API_CALL update() = 0;
		virtual void API_CALL dispose() = 0;
	};
		
	class IObbContentProvider : IBaseObject
	{
	public:
		virtual void API_CALL refreshAssetsManager(void *jniEnv, void *assetsManager) = 0;
		virtual void API_CALL openAssets(void *jniEnv, void *assetsManager) = 0;
		virtual void API_CALL openObbFile(const char *obbFile) = 0;
		virtual void API_CALL closeObbFile() = 0;
		virtual bool API_CALL existsContent(const char *name) = 0;
		virtual void API_CALL openContent(const char *name) = 0;
		virtual int API_CALL read(void *buffer, int bytesLimit) = 0;
		virtual int API_CALL getContentSize() = 0;
		virtual void API_CALL closeContent() = 0;
	};

	class INavMeshConfig : IBaseObject
	{
	public:
		virtual void API_CALL clear() = 0;
		virtual void API_CALL addPolygon(IntPtr vertices2f, uint verticesCount) = 0;
		virtual IntPtr API_CALL getNativeInstance() = 0;
		virtual void API_CALL dispose() = 0;
	};
	
	class INavPath : IBaseObject
	{
	public:
		virtual int API_CALL getVerticesCount() = 0;
		virtual void API_CALL getVertices(IntPtr vertices2f) = 0;
		virtual IntPtr API_CALL getNativeInstance() = 0;
	};

	class IPathMesh : IBaseObject
	{
	public:
		virtual void API_CALL fillNavPath(INavPath *path) = 0;
		virtual int API_CALL getTriagnlesCount() = 0;
		virtual void API_CALL getTriagnles(IntPtr triangles3f) = 0;
		virtual IntPtr API_CALL getNativeInstance() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class INavMesh : IBaseObject
	{
	public:
		virtual void API_CALL buildNavMesh(INavMeshConfig* config) = 0;
		virtual int API_CALL getTriagnlesCount() = 0;
		virtual void API_CALL getTriagnles(IntPtr triangles3f) = 0;
		virtual void API_CALL fillPathMesh(float x0, float y0, float x1, float y1, IPathMesh *mesh) = 0;
		virtual void API_CALL dispose() = 0;
	};


	/// physics
	class IPhysicalShape : IBaseObject
	{
	public:
		virtual IntPtr API_CALL getNativeInstance() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IPhysicalFixture : IBaseObject
	{
	public:
		virtual IntPtr API_CALL getNativeInstance() = 0;
		virtual bool API_CALL testPolygonOverlap(float x, float y, IntPtr polygon2f, uint count) = 0;
		virtual bool API_CALL testOverlap(IPhysicalFixture* fixture) = 0;

		virtual bool API_CALL testPolygonOverlapEx(float x, float y, IntPtr polygon2f, uint count, IntPtr contactPoint) = 0;
		virtual bool API_CALL testOverlapEx(IPhysicalFixture* fixture, IntPtr contactPoint) = 0;

		virtual void API_CALL pauseCollisions(uint group) = 0;
		virtual void API_CALL resumeCollisions() = 0;
		virtual int API_CALL getContactsCounter() = 0;
		virtual float API_CALL getDensity() = 0;
		virtual float API_CALL getFriction() = 0;
		virtual float API_CALL getBonce() = 0;
		virtual bool API_CALL isSensor() = 0;
		virtual uint API_CALL getCollidesWith() = 0;
		virtual uint API_CALL getCollisionGroup() = 0;
		virtual void API_CALL setDensity(float value) = 0;
		virtual void API_CALL setFriction(float value) = 0;
		virtual void API_CALL setBonce(float value) = 0;
		virtual void API_CALL setSensor(bool value) = 0;
		virtual void API_CALL setCollidesWith(uint value) = 0;
		virtual void API_CALL setCollisionGroup(uint value) = 0;
		virtual void API_CALL setRaycastGroup(ushort value) = 0;
	};
	
	class IPhysicalContactIterator : IBaseObject
	{
	public:
		virtual void API_CALL CalcWorldManifold() = 0;
		virtual float API_CALL getWorldNormalX() = 0;
		virtual float API_CALL getWorldNormalY() = 0;

		virtual bool API_CALL isEnabled() = 0;
		virtual bool API_CALL isSensorA() = 0;
		virtual uint API_CALL getCollisionGroupA() = 0;
		virtual IntPtr API_CALL getNativeBodyA() = 0;
		virtual float API_CALL getNativeBodyAPositionX() = 0;
		virtual float API_CALL getNativeBodyAPositionY() = 0;
		virtual IntPtr API_CALL getNativeFixtureA() = 0;
		virtual bool API_CALL isSensorB() = 0;
		virtual uint API_CALL getCollisionGroupB() = 0;
		virtual IntPtr API_CALL getNativeBodyB() = 0;
		virtual float API_CALL getNativeBodyBPositionX() = 0;
		virtual float API_CALL getNativeBodyBPositionY() = 0;
		virtual IntPtr API_CALL getNativeFixtureB() = 0;
		virtual void API_CALL reset() = 0;
		virtual bool API_CALL isEnd() = 0;
		virtual void API_CALL next() = 0;
	};

	class IPhysicalBody : IBaseObject
	{
	public:
		virtual IntPtr API_CALL getNativeInstance() = 0;

		virtual float API_CALL getVelocityX() = 0;
		virtual float API_CALL getVelocityY() = 0;
		virtual void API_CALL setVelocityX(float x) = 0;
		virtual void API_CALL setVelocityY(float y) = 0;
		virtual float API_CALL getX() = 0;
		virtual float API_CALL getY() = 0;
		virtual void API_CALL setX(float x) = 0;
		virtual void API_CALL setY(float y) = 0;
		virtual float API_CALL getRotation() = 0;
		virtual void API_CALL setRotation(float value) = 0;
		virtual IPhysicalContactIterator* API_CALL getContactIterator() = 0;
		virtual IPhysicalFixture* API_CALL createEdgeFixture(float x0, float y0, float x1, float y1, PhysicsFixtureConfig config) = 0;
		virtual IPhysicalFixture* API_CALL createCircleFixture(float x, float y, float r, PhysicsFixtureConfig config) = 0;
		virtual IPhysicalFixture* API_CALL createBoxFixture(float x, float y, float rx, float ry, float rotation, PhysicsFixtureConfig config) = 0;
		virtual IPhysicalFixture* API_CALL createPolygonFixture(IntPtr vertices2f, int count, PhysicsFixtureConfig config) = 0;
	};

	class IPhysicalFixedJoint : IBaseObject
	{		
	public:
		virtual IntPtr API_CALL getNativeInstance() = 0;

		virtual void API_CALL setJointTarget(float x, float y) = 0;
		virtual void API_CALL setJointMaxForce(float value) = 0;
		virtual void API_CALL setJointFrequency(float value) = 0;
		virtual void API_CALL setJointDampingRatio(float value) = 0;
	};

	class IPhysicalWorld : IBaseObject
	{
	public:
		virtual void API_CALL debugRender(float x, float y, float rx, float ry) = 0;
		virtual bool API_CALL makeRayCastFirstEx(float x0, float y0, float x1, float y1, uint raycastMask, uint mask, bool ignoreSensors, IntPtr result) = 0;
		virtual bool API_CALL makeRayCastFirst(float x0, float y0, float x1, float y1, uint raycastMask, uint mask, bool ignoreSensors, IntPtr resultPoint, IntPtr resultNormal) = 0;
		virtual IPhysicalBody* API_CALL createPhysicalBody(PhysicalBodyType type, float x, float y, float rotation, bool fixedRotation) = 0;
		virtual void API_CALL disposePhysicalBody(IPhysicalBody* body) = 0;
		virtual IPhysicalFixedJoint* API_CALL createPhysicalFixedJoint(IPhysicalBody* ground, IPhysicalBody* body, float x, float y, float maxForce) = 0;
		virtual void API_CALL disposePhysicalJoint(IPhysicalFixedJoint* joint) = 0;
		virtual void API_CALL step(float dt, int velocityIterations, int positionIterations) = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IPhysicalFactory : IBaseObject
	{
	public:
		virtual IPhysicalWorld* API_CALL createPhysicalWorld() = 0;
		virtual IPhysicalShape* API_CALL createCircleShape(float x, float y, float r) = 0;
		virtual IPhysicalShape* API_CALL createBoxShape(float x, float y, float rx, float ry, float rotation) = 0;
		virtual IPhysicalShape* API_CALL createPolygonShape(IntPtr vertices2f, int count) = 0;
		virtual void API_CALL dispose() = 0;
	};

	// terrain
	class ITerrainConfig : IBaseObject
	{
	public:
		virtual void API_CALL set(TerrainStripeConfigType type, float inWidth, float outWidth, float width) = 0;
		virtual void API_CALL setPhysicsMode(TerrainStripePhysicsType mode) = 0;
		virtual void API_CALL setFlipReversed(bool value) = 0;
		virtual void API_CALL setTilesConfig(uint value) = 0;
	};
	
	class ITerrainStripeBuilder : IBaseObject
	{
	public:
		virtual ITerrainConfig* API_CALL getConfig() = 0;
		virtual void API_CALL buildStripe(IntPtr terrainStripePoints, float tension, int count, bool closed) = 0;
		virtual int API_CALL getDebugRenderVerticesCount() = 0;
		virtual void API_CALL getDebugRenderVertices(IntPtr vertices2f) = 0;
		virtual IMeshesResult* API_CALL getResult() = 0;
		virtual int API_CALL getEdgesCount() = 0;
		virtual void API_CALL getEdge(int i, IntPtr vertices2f) = 0;
		virtual void API_CALL dispose() = 0;
	};

	class ITerrainFactory : IBaseObject
	{
	public:
		virtual ITerrainStripeBuilder * API_CALL createStripeBuilder() = 0;
		virtual void API_CALL dispose() = 0;
	};
	
	class ITestGame : IBaseObject
	{
	public:
		virtual void API_CALL setRootPath(String rootPath) = 0;
		virtual void API_CALL updateColors(IntPtr colors) = 0;
		virtual void API_CALL updateMouse(int rawX, int rawY, int windowX, int windowY, uint buttons) = 0;
		virtual void API_CALL updateEnvironment(int w, int h, float ellapsedTime) = 0;
		virtual void API_CALL load() = 0;
		virtual void API_CALL update() = 0;
		virtual void API_CALL render() = 0;
		virtual void API_CALL release() = 0;
	};

	extern "C" DLLEXPORT IBatch2d * API_CALL createBatch2d();
    
	extern "C" DLLEXPORT ITexture * API_CALL createTextureByID(uint id);
	extern "C" DLLEXPORT IContentManager * API_CALL createContentManager();
	extern "C" DLLEXPORT IObbContentProvider * API_CALL createContentProvider();
	extern "C" DLLEXPORT IEntitiesFactory * API_CALL createEntitiesFactory();
	extern "C" DLLEXPORT IGraphicsFactory * API_CALL createGraphicsFactory();
	extern "C" DLLEXPORT IGeometryFactory * API_CALL createGeometryFactory();

	extern "C" DLLEXPORT void API_CALL initRenderContext();
	extern "C" DLLEXPORT INativeMemoryProfiler * API_CALL createNativeMemoryProfiler();
	extern "C" DLLEXPORT ISpineResource * API_CALL createSpineResource();
	extern "C" DLLEXPORT IPhysicalFactory * API_CALL createPhysicalFactory(float scaleDimensions, float scaleVelocity);
	extern "C" DLLEXPORT ITerrainFactory * API_CALL createTerrainFactory();

	//extern "C" DLLEXPORT IPathMesh * API_CALL createPathMesh();
	//extern "C" DLLEXPORT INavMesh * API_CALL createNavMesh();
	//extern "C" DLLEXPORT INavMeshConfig * API_CALL createNavMeshConfig();
	//extern "C" DLLEXPORT INavPath * API_CALL createNavPath();
}

#endif /*LLGE_H*/