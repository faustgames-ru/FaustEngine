#ifndef LLGE_H
#define LLGE_H

#define API_CALL __stdcall
#define DLLEXPORT __declspec( dllexport )

namespace llge
{
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
	
	class ISpriteComponent
	{
	public:
		virtual void API_CALL updateVertex(int index, float x, float y, float z, short u, short v, int color) = 0;
		virtual void API_CALL updateImage(IImage * image) = 0;
	};

	class IStaticSpriteEntity
	{
	public:
		virtual ISpriteComponent * API_CALL getSpriteComponent() = 0;
		virtual IAabb2dComponent * API_CALL getAabbComponent() = 0;
		virtual IRenderLayerComponent * API_CALL getRenderLayerComponent() = 0;
	};

	class IDynamicSpriteEntity
	{
	public:
		virtual ITransform2dComponent * API_CALL getTransform2dComponent() = 0;
		virtual ISpriteComponent * API_CALL getSpriteComponent() = 0;
		virtual IAabb2dComponent * API_CALL getAabbComponent() = 0;
	};

	class IEntitiesFactory
	{
	public:
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
		virtual void API_CALL dispose() = 0;
	};

	extern "C" DLLEXPORT IFactory * API_CALL createFactory();
}

#endif /*LLGE_H*/