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

#define ushort unsigned short
#define uint unsigned int

namespace llge
{
	struct Mesh2dVertex
	{
		float x;
		float y;
		float z;
		ushort u;
		ushort v;
		uint color;
	};
	
	class IStaticRenderLayer
	{
	public:
	};

	class IImage
	{
	public:
	};

	class IAabb2dComponent
	{
	public:
		virtual void API_CALL upateAabb2d(float minX, float minY, float maxX, float maxY) = 0;
	};

	class ITransform2dComponent
	{
	public:
		virtual void API_CALL upateTransform(float x, float y, float z, float rotation, float scale) = 0;
		virtual void API_CALL upatePivot(float x, float y) = 0;
	};

	class IRenderLayerComponent
	{
	public:
		virtual void API_CALL upateRenderLayer(IStaticRenderLayer *value) = 0;
	};
	
	class IMesh2dComponent
	{
	public:
		virtual void API_CALL updateData(void * mesh2dVertices, void * ushortIndices) = 0;
	};
		
	class IMesh2dEntity
	{
	public:
		virtual ITransform2dComponent * API_CALL getTransform() = 0;
		virtual IAabb2dComponent * API_CALL getAabbComponent() = 0;
		virtual IMesh2dComponent * API_CALL getMesh() = 0;
	};

	class IEntitiesFactory
	{
	public:
		virtual IMesh2dEntity * API_CALL createMeshEntity(int verticesCount, int indicesCount) = 0;
		virtual void API_CALL disposeMeshEntities() = 0;
		virtual void API_CALL dispose() = 0;
	};
	
	class IRenderSystem
	{
	public:
		virtual IStaticRenderLayer * API_CALL createStaticLayer(float z) = 0;
		virtual IImage * API_CALL createImage() = 0;
	};

	class ITestRenderSystem
	{
	public:		
		virtual void API_CALL viewport(int width, int height) = 0;
		virtual void API_CALL create() = 0;
		virtual void API_CALL render() = 0;
		virtual void API_CALL cleanup() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IFactory
	{
	public:
		virtual ITestRenderSystem * API_CALL createRenderSystem() = 0;
		virtual IEntitiesFactory * API_CALL createEntitiesFactory() = 0;
		virtual void API_CALL dispose() = 0;
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

	class ITextureUniform
	{
	public:
		virtual void API_CALL setTexture(ITexture *texture) = 0;
	};

	class IProjectionUniform
	{
	public:
		virtual void API_CALL setProjection(void *floatMatrix) = 0;
	};

	class IVertexFormatsFacade
	{
	public:
		virtual int API_CALL getPositionTextureColorFormat() = 0;
	};

	class IEffectsFacade
	{
	public:
		virtual int API_CALL getTextureColorEffect() = 0;
	};

	class IUniformsFacade
	{
	public:
		virtual ITextureUniform * API_CALL getTextureUniform() = 0;
		virtual IProjectionUniform * API_CALL getProjectionUniformm() = 0;
	};

	class IGraphicsFacade
	{
	public:
		virtual IUniformsFacade * API_CALL getUniforms() = 0;
		virtual IVertexFormatsFacade * API_CALL getVertexFormatsFacade() = 0;
		virtual IEffectsFacade * API_CALL getEffectsFacade() = 0;
		virtual ITexture * API_CALL createTexture() = 0;

		virtual void API_CALL viewport(int width, int height) = 0;
		virtual void API_CALL setClearState(uint color, float depth) = 0;
		virtual void API_CALL clear() = 0;
		virtual void API_CALL draw(int effect, int vertexFormat, void *vertices, void *indices, int primitivesCount) = 0;

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

	extern "C" DLLEXPORT IFactory * API_CALL createFactory();
	extern "C" DLLEXPORT IGraphicsFactory * API_CALL createGraphicsFactory();
}

#endif /*LLGE_H*/