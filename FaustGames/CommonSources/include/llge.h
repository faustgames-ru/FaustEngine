#ifndef LLGE_H
#define LLGE_H

#ifdef __ANDROID__
#include <jni.h>
#define API_CALL JNICALL
#define DLLEXPORT JNIEXPORT
#else
#define API_CALL __stdcall
#define DLLEXPORT __declspec( dllexport )
#endif

typedef unsigned short ushort;
typedef unsigned int uint;
typedef void * IntPtr;

namespace llge
{
	/// graphics enums

	enum GraphicsEffects
	{
		EffectTextureColor = 0x0,
		EffectTextureLightmapColor = 0x1,
		EffectWater = 0x2,
		EffectSolid = 0x3,
	};

	enum GraphicsVertexFormats
	{
		FormatPositionTextureColor = 0x0,
		FormatPositionNormal = 0x1,
		FormatPositionTexture = 0x2,
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

	/// entities enums
	enum ComponentsTypes
	{
		Aadd2d = 0x1,
		Transform2d = 0x2,
		Render2d = 0x4,
	};

	/// graphics
	class ITexture
	{
	public:
		virtual uint API_CALL getId() = 0;
	};

	class ITextureImage2d
	{
	public:
		virtual ITexture* API_CALL getTexture() = 0;
		virtual void API_CALL LoadPixels(int width, int height, void *pixels) = 0;

		virtual void API_CALL create() = 0;
		virtual void API_CALL cleanup() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IRenderTarget2d
	{
	public:
		virtual ITexture* API_CALL getTexture() = 0;
		virtual void API_CALL create(int width, int height) = 0;
		virtual void API_CALL cleanup() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IUniformsFacade
	{
	public:
		virtual void API_CALL setTime(float value) = 0;
		virtual void API_CALL setNormalmap(ITexture *texture) = 0;
		virtual void API_CALL setEnvironment(ITexture *texture) = 0;
		virtual void API_CALL setTexture(ITexture *texture) = 0;
		virtual void API_CALL setLightMap(ITexture *texture) = 0;
		virtual void API_CALL setProjection(void *floatMatrix) = 0;
	};

	class IVertexBuffer
	{
	public:
		virtual int API_CALL getId() = 0;
		virtual void API_CALL create() = 0;
		virtual void API_CALL setData(void *data, int count) = 0;
		virtual void API_CALL cleanup() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IGraphicsFacade
	{
	public:
		virtual IUniformsFacade * API_CALL getUniforms() = 0;
		virtual ITextureImage2d * API_CALL createTextureImage2d() = 0;
		virtual IRenderTarget2d * API_CALL createRenderTarget2d() = 0;
		virtual IVertexBuffer * API_CALL createVertexBuffer() = 0;

		virtual void API_CALL viewport(int width, int height) = 0;
		virtual void API_CALL setClearState(uint color, float depth) = 0;
		virtual void API_CALL clear() = 0;
		virtual void API_CALL draw(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void *vertices, void *indices, int primitivesCount) = 0;
		virtual void API_CALL drawVertexBuffer(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IVertexBuffer *vertexBuffer, void *indices, int primitivesCount) = 0;
		virtual void API_CALL setEffectConstantFloat(GraphicsEffects effect, char *name, float value) = 0;
		virtual void API_CALL setEffectConstantColor(GraphicsEffects effect, char *name, uint value) = 0;

		virtual void API_CALL create() = 0;
		virtual void API_CALL cleanup() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IGraphicsFactory
	{
	public:
		virtual IGraphicsFacade * API_CALL createGraphicsFacade() = 0;
		virtual void API_CALL dispose() = 0;
	};

	/// geometry

	class IQuadTree
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

	class IGeometryFactory
	{
	public:
		virtual IQuadTree * API_CALL createQuadTree() = 0;
		virtual void API_CALL dispose() = 0;
	};

	/// entities

	class IAabb2d
	{
	public:
		virtual void API_CALL update(float minX, float minY, float maxX, float maxY, float zOrder) = 0;
	};

	class IRender2d
	{
	public:
		virtual void API_CALL setMesh(ITexture *texture, void* vertices, int verticesCount, void* indices, int indicesCount) = 0;
	};


	class ITransform2d
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

	class IEntity
	{
	public:
		virtual IntPtr getSelfInstance() = 0;		
		virtual void setComponents(ComponentsTypes types) = 0;
		virtual IAabb2d* API_CALL getAabb2d() = 0;
		virtual IRender2d* API_CALL getRender2d() = 0;
		virtual ITransform2d* API_CALL getTransform2d() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class ICamera
	{
	public:
		virtual void API_CALL setPosition(float x, float y, float z) = 0;
		virtual void API_CALL setFov(float fov) = 0;
		virtual void API_CALL setAspect(float aspect) = 0;
		virtual void API_CALL setRotation(float rotationZ) = 0;
		virtual void API_CALL setPlanes(float zn, float zf) = 0;
	};

	class IEntitiesWorld
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
	};

	class IEntitiesFactory
	{
	public:
		virtual IEntitiesWorld * API_CALL createEntitiesWorld() = 0;
		virtual void API_CALL dispose() = 0;
	};

	/// content
	class IContentManager
	{
	public:
		virtual int API_CALL registerImage(char * name) = 0;
		virtual void API_CALL startLoad() = 0;
		virtual void API_CALL loadImage(int id, ITextureImage2d *textureImage) = 0;
		virtual void API_CALL finishLoad() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IContentFactory
	{
	public:
		virtual IContentManager * API_CALL createContentManager() = 0;
		virtual void API_CALL dispose() = 0;
	};

	extern "C" DLLEXPORT IEntitiesFactory * API_CALL createEntitiesFactory();
	extern "C" DLLEXPORT IGraphicsFactory * API_CALL createGraphicsFactory();
	extern "C" DLLEXPORT IGeometryFactory * API_CALL createGeometryFactory();
}

#endif /*LLGE_H*/