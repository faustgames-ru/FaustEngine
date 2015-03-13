/* ============================================================== */
/* This file is automatically generated. Please do not modify it. */
/* ============================================================== */

#include "llge.h"

namespace llge
{
	extern "C" DLLEXPORT int API_CALL llge_Texture_getId (ITexture * classInstance)
	{
		return classInstance->getId();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Texture_LoadPixels (ITexture * classInstance, int width, int height, void * pixels)
	{
		classInstance->LoadPixels(width, height, pixels);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Texture_create (ITexture * classInstance)
	{
		classInstance->create();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Texture_cleanup (ITexture * classInstance)
	{
		classInstance->cleanup();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Texture_dispose (ITexture * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT int API_CALL llge_Cubemap_getId (ICubemap * classInstance)
	{
		return classInstance->getId();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Cubemap_LoadPixels (ICubemap * classInstance, CubemapPlane plane, int width, int height, void * pixels)
	{
		classInstance->LoadPixels(plane, width, height, pixels);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Cubemap_create (ICubemap * classInstance)
	{
		classInstance->create();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Cubemap_cleanup (ICubemap * classInstance)
	{
		classInstance->cleanup();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Cubemap_dispose (ICubemap * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_UniformsFacade_setTime (IUniformsFacade * classInstance, float value)
	{
		classInstance->setTime(value);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_UniformsFacade_setNormalmap (IUniformsFacade * classInstance, ITexture * texture)
	{
		classInstance->setNormalmap(texture);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_UniformsFacade_setEnvironment (IUniformsFacade * classInstance, ITexture * cubemap)
	{
		classInstance->setEnvironment(cubemap);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_UniformsFacade_setTexture (IUniformsFacade * classInstance, ITexture * texture)
	{
		classInstance->setTexture(texture);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_UniformsFacade_setLightMap (IUniformsFacade * classInstance, ITexture * texture)
	{
		classInstance->setLightMap(texture);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_UniformsFacade_setProjection (IUniformsFacade * classInstance, void * floatMatrix)
	{
		classInstance->setProjection(floatMatrix);
	}
	
	extern "C" DLLEXPORT int API_CALL llge_VertexBuffer_getId (IVertexBuffer * classInstance)
	{
		return classInstance->getId();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_VertexBuffer_create (IVertexBuffer * classInstance)
	{
		classInstance->create();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_VertexBuffer_setData (IVertexBuffer * classInstance, void * data, int count)
	{
		classInstance->setData(data, count);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_VertexBuffer_cleanup (IVertexBuffer * classInstance)
	{
		classInstance->cleanup();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_VertexBuffer_dispose (IVertexBuffer * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT IUniformsFacade * API_CALL llge_GraphicsFacade_getUniforms (IGraphicsFacade * classInstance)
	{
		return classInstance->getUniforms();
	}
	
	extern "C" DLLEXPORT ITexture * API_CALL llge_GraphicsFacade_createTexture (IGraphicsFacade * classInstance)
	{
		return classInstance->createTexture();
	}
	
	extern "C" DLLEXPORT ICubemap * API_CALL llge_GraphicsFacade_createCubemap (IGraphicsFacade * classInstance)
	{
		return classInstance->createCubemap();
	}
	
	extern "C" DLLEXPORT IVertexBuffer * API_CALL llge_GraphicsFacade_createVertexBuffer (IGraphicsFacade * classInstance)
	{
		return classInstance->createVertexBuffer();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_viewport (IGraphicsFacade * classInstance, int width, int height)
	{
		classInstance->viewport(width, height);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_setClearState (IGraphicsFacade * classInstance, uint color, float depth)
	{
		classInstance->setClearState(color, depth);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_clear (IGraphicsFacade * classInstance)
	{
		classInstance->clear();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_draw (IGraphicsFacade * classInstance, GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void * vertices, void * indices, int primitivesCount)
	{
		classInstance->draw(effect, vertexFormat, vertices, indices, primitivesCount);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_drawVertexBuffer (IGraphicsFacade * classInstance, GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IVertexBuffer * vertexBuffer, void * indices, int primitivesCount)
	{
		classInstance->drawVertexBuffer(effect, vertexFormat, vertexBuffer, indices, primitivesCount);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_setEffectConstantFloat (IGraphicsFacade * classInstance, GraphicsEffects effect, char * name, float value)
	{
		classInstance->setEffectConstantFloat(effect, name, value);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_create (IGraphicsFacade * classInstance)
	{
		classInstance->create();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_cleanup (IGraphicsFacade * classInstance)
	{
		classInstance->cleanup();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_dispose (IGraphicsFacade * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT IGraphicsFacade * API_CALL llge_GraphicsFactory_createGraphicsFacade (IGraphicsFactory * classInstance)
	{
		return classInstance->createGraphicsFacade();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFactory_dispose (IGraphicsFactory * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT int API_CALL llge_QuadTree_insert (IQuadTree * classInstance, float minX, float minY, float maxX, float maxY, int userData)
	{
		return classInstance->insert(minX, minY, maxX, maxY, userData);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_QuadTree_remove (IQuadTree * classInstance, int id)
	{
		classInstance->remove(id);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_QuadTree_query (IQuadTree * classInstance, float minX, float minY, float maxX, float maxY)
	{
		classInstance->query(minX, minY, maxX, maxY);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_QuadTree_getQueryResults (IQuadTree * classInstance, void * intBuffer)
	{
		classInstance->getQueryResults(intBuffer);
	}
	
	extern "C" DLLEXPORT int API_CALL llge_QuadTree_getQueryResultsCount (IQuadTree * classInstance)
	{
		return classInstance->getQueryResultsCount();
	}
	
	extern "C" DLLEXPORT int API_CALL llge_QuadTree_getIterationsCount (IQuadTree * classInstance)
	{
		return classInstance->getIterationsCount();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_QuadTree_dispose (IQuadTree * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT IQuadTree * API_CALL llge_GeometryFactory_createQuadTree (IGeometryFactory * classInstance)
	{
		return classInstance->createQuadTree();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GeometryFactory_dispose (IGeometryFactory * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Entity_setBounds (IEntity * classInstance, float minX, float minY, float maxX, float maxY, float zOrder)
	{
		classInstance->setBounds(minX, minY, maxX, maxY, zOrder);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Entity_setMesh (IEntity * classInstance, ITexture * texture, void * vertices, int verticesCount, void * indices, int indicesCount)
	{
		classInstance->setMesh(texture, vertices, verticesCount, indices, indicesCount);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Entity_setWorldPosition (IEntity * classInstance, float x, float y, float z)
	{
		classInstance->setWorldPosition(x, y, z);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Entity_setWorldRotation (IEntity * classInstance, float value)
	{
		classInstance->setWorldRotation(value);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Entity_setWorldScale (IEntity * classInstance, float value)
	{
		classInstance->setWorldScale(value);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Entity_setLocalPivot (IEntity * classInstance, float x, float y, float z)
	{
		classInstance->setLocalPivot(x, y, z);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Entity_setLocalPosition (IEntity * classInstance, float x, float y, float z)
	{
		classInstance->setLocalPosition(x, y, z);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Entity_setLocalRotation (IEntity * classInstance, float value)
	{
		classInstance->setLocalRotation(value);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Entity_setLocalScale (IEntity * classInstance, float value)
	{
		classInstance->setLocalScale(value);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Entity_addToWorld (IEntity * classInstance)
	{
		classInstance->addToWorld();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Entity_removeFromWorld (IEntity * classInstance)
	{
		classInstance->removeFromWorld();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Entity_dispose (IEntity * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Camera_setPosition (ICamera * classInstance, float x, float y, float z)
	{
		classInstance->setPosition(x, y, z);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Camera_setFov (ICamera * classInstance, float fov)
	{
		classInstance->setFov(fov);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Camera_setAspect (ICamera * classInstance, float aspect)
	{
		classInstance->setAspect(aspect);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Camera_setRotation (ICamera * classInstance, float rotationZ)
	{
		classInstance->setRotation(rotationZ);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Camera_setPlanes (ICamera * classInstance, float zn, float zf)
	{
		classInstance->setPlanes(zn, zf);
	}
	
	extern "C" DLLEXPORT ICamera * API_CALL llge_EntitiesWorld_getCamera (IEntitiesWorld * classInstance)
	{
		return classInstance->getCamera();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_EntitiesWorld_setUnpdateBounds (IEntitiesWorld * classInstance, float minX, float minY, float maxX, float maxY)
	{
		classInstance->setUnpdateBounds(minX, minY, maxX, maxY);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_EntitiesWorld_setRenderBounds (IEntitiesWorld * classInstance, float minX, float minY, float maxX, float maxY)
	{
		classInstance->setRenderBounds(minX, minY, maxX, maxY);
	}
	
	extern "C" DLLEXPORT IEntity * API_CALL llge_EntitiesWorld_createMesh2d (IEntitiesWorld * classInstance)
	{
		return classInstance->createMesh2d();
	}
	
	extern "C" DLLEXPORT int API_CALL llge_EntitiesWorld_update (IEntitiesWorld * classInstance, float elapsed)
	{
		return classInstance->update(elapsed);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_EntitiesWorld_dispose (IEntitiesWorld * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT IEntitiesWorld * API_CALL llge_EntitiesFactory_createEntitiesWorld (IEntitiesFactory * classInstance)
	{
		return classInstance->createEntitiesWorld();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_EntitiesFactory_dispose (IEntitiesFactory * classInstance)
	{
		classInstance->dispose();
	}
	
}

