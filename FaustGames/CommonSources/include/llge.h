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
	};

	enum GraphicsVertexFormats
	{
		FormatPositionTextureColor = 0x0,
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

	class IUniformsFacade
	{
	public:
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
		virtual IVertexBuffer * API_CALL createVertexBuffer() = 0;

		virtual void API_CALL viewport(int width, int height) = 0;
		virtual void API_CALL setClearState(uint color, float depth) = 0;
		virtual void API_CALL clear() = 0;
		virtual void API_CALL draw(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void *vertices, void *indices, int primitivesCount) = 0;
		virtual void API_CALL drawVertexBuffer(GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IVertexBuffer *vertexBuffer, void *indices, int primitivesCount) = 0;

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

	extern "C" DLLEXPORT IGraphicsFactory * API_CALL createGraphicsFactory();
	extern "C" DLLEXPORT IGeometryFactory * API_CALL createGeometryFactory();
}

#endif /*LLGE_H*/