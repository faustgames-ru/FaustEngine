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

	/// graphics

	class ITexture
	{
	public:
		virtual int API_CALL getId() = 0;
		virtual void API_CALL LoadPixels(int width, int height, void *pixels) = 0;

		virtual void API_CALL create() = 0;
		virtual void API_CALL cleanup() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class ICubemap
	{
	public:
		virtual int API_CALL getId() = 0;
		virtual void API_CALL LoadPixels(CubemapPlane plane, int width, int height, void *pixels) = 0;

		virtual void API_CALL create() = 0;
		virtual void API_CALL cleanup() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IUniformsFacade
	{
	public:
		virtual void API_CALL setTime(float value) = 0;
		virtual void API_CALL setNormalmap(ITexture *texture) = 0;
		virtual void API_CALL setEnvironment(ITexture *cubemap) = 0;
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
		virtual ITexture * API_CALL createTexture() = 0;
		virtual ICubemap * API_CALL createCubemap() = 0;
		virtual IVertexBuffer * API_CALL createVertexBuffer() = 0;

		virtual void API_CALL viewport(int width, int height) = 0;
		virtual void API_CALL setClearState(uint color, float depth) = 0;
		virtual void API_CALL clear() = 0;
		virtual void API_CALL draw(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void *vertices, void *indices, int primitivesCount) = 0;
		virtual void API_CALL drawVertexBuffer(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IVertexBuffer *vertexBuffer, void *indices, int primitivesCount) = 0;
		virtual void API_CALL setEffectConstantFloat(GraphicsEffects effect, char *name, float value) = 0;

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
	class IEntity
	{
	public:
		virtual void API_CALL setBounds(float minX, float minY, float maxX, float maxY, float zOrder) = 0;
		virtual void API_CALL setMesh(ITexture *texture, void* vertices, int verticesCount, void* indices, int indicesCount) = 0;
		virtual void API_CALL setWorldPosition(float x, float y, float z) = 0;
		virtual void API_CALL setWorldRotation(float value) = 0;
		virtual void API_CALL setWorldScale(float value) = 0;
		virtual void API_CALL setLocalPivot(float x, float y, float z) = 0;
		virtual void API_CALL setLocalPosition(float x, float y, float z) = 0;
		virtual void API_CALL setLocalRotation(float value) = 0;
		virtual void API_CALL setLocalScale(float value) = 0;
		virtual void API_CALL addToWorld() = 0;
		virtual void API_CALL removeFromWorld() = 0;
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
		
		virtual IEntity * API_CALL createMesh2d() = 0;
				
		virtual int API_CALL update(float elapsed) = 0;

		virtual void API_CALL dispose() = 0;
	};

	class IEntitiesFactory
	{
	public:
		virtual IEntitiesWorld * API_CALL createEntitiesWorld() = 0;
		virtual void API_CALL dispose() = 0;
	};

	extern "C" DLLEXPORT IEntitiesFactory * API_CALL createEntitiesFactory();
	extern "C" DLLEXPORT IGraphicsFactory * API_CALL createGraphicsFactory();
	extern "C" DLLEXPORT IGeometryFactory * API_CALL createGeometryFactory();
}

#endif /*LLGE_H*/