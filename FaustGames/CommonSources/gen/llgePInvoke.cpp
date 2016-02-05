/* ============================================================== */
/* This file is automatically generated. Please do not modify it. */
/* ============================================================== */

#include "llge.h"

namespace llge
{
	extern "C" DLLEXPORT uint API_CALL llge_Texture_getId (ITexture * classInstance)
	{
		return classInstance->getId();
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_Texture_getTextureInstance (ITexture * classInstance)
	{
		return classInstance->getTextureInstance();
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_TextureImage2d_getTextureImageInstance (ITextureImage2d * classInstance)
	{
		return classInstance->getTextureImageInstance();
	}
	
	extern "C" DLLEXPORT ITexture * API_CALL llge_TextureImage2d_getTexture (ITextureImage2d * classInstance)
	{
		return classInstance->getTexture();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_TextureImage2d_LoadPixels (ITextureImage2d * classInstance, int width, int height, TextureImage2dFormat format, void * pixels)
	{
		classInstance->LoadPixels(width, height, format, pixels);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_TextureImage2d_create (ITextureImage2d * classInstance)
	{
		classInstance->create();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_TextureImage2d_cleanup (ITextureImage2d * classInstance)
	{
		classInstance->cleanup();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_TextureImage2d_dispose (ITextureImage2d * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_RenderTarget2d_getRenderTargetInstance (IRenderTarget2d * classInstance)
	{
		return classInstance->getRenderTargetInstance();
	}
	
	extern "C" DLLEXPORT ITexture * API_CALL llge_RenderTarget2d_getTexture (IRenderTarget2d * classInstance)
	{
		return classInstance->getTexture();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_RenderTarget2d_create (IRenderTarget2d * classInstance, int width, int height)
	{
		classInstance->create(width, height);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_RenderTarget2d_cleanup (IRenderTarget2d * classInstance)
	{
		classInstance->cleanup();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_RenderTarget2d_dispose (IRenderTarget2d * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_RenderTargetDepth2d_getRenderTargetInstance (IRenderTargetDepth2d * classInstance)
	{
		return classInstance->getRenderTargetInstance();
	}
	
	extern "C" DLLEXPORT ITexture * API_CALL llge_RenderTargetDepth2d_getTexture (IRenderTargetDepth2d * classInstance)
	{
		return classInstance->getTexture();
	}
	
	extern "C" DLLEXPORT ITexture * API_CALL llge_RenderTargetDepth2d_getDepthTexture (IRenderTargetDepth2d * classInstance)
	{
		return classInstance->getDepthTexture();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_RenderTargetDepth2d_create (IRenderTargetDepth2d * classInstance, int width, int height)
	{
		classInstance->create(width, height);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_RenderTargetDepth2d_cleanup (IRenderTargetDepth2d * classInstance)
	{
		classInstance->cleanup();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_RenderTargetDepth2d_dispose (IRenderTargetDepth2d * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_UniformsFacade_resetSamplers (IUniformsFacade * classInstance)
	{
		classInstance->resetSamplers();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_UniformsFacade_setTime (IUniformsFacade * classInstance, float value)
	{
		classInstance->setTime(value);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_UniformsFacade_setNormalmap (IUniformsFacade * classInstance, ITexture * texture)
	{
		classInstance->setNormalmap(texture);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_UniformsFacade_setEnvironment (IUniformsFacade * classInstance, ITexture * texture)
	{
		classInstance->setEnvironment(texture);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_UniformsFacade_setDepthmap (IUniformsFacade * classInstance, ITexture * texture)
	{
		classInstance->setDepthmap(texture);
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
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_viewport (IGraphicsFacade * classInstance, int width, int height)
	{
		classInstance->viewport(width, height);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_setRenderTarget (IGraphicsFacade * classInstance, void * renderTargetInstance)
	{
		classInstance->setRenderTarget(renderTargetInstance);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_setClearState (IGraphicsFacade * classInstance, uint color, float depth)
	{
		classInstance->setClearState(color, depth);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_setBlendMode (IGraphicsFacade * classInstance, BlendMode blendMode)
	{
		classInstance->setBlendMode(blendMode);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_clear (IGraphicsFacade * classInstance)
	{
		classInstance->clear();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_clearDepth (IGraphicsFacade * classInstance)
	{
		classInstance->clearDepth();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_resetDrawCallsCounter (IGraphicsFacade * classInstance)
	{
		classInstance->resetDrawCallsCounter();
	}
	
	extern "C" DLLEXPORT int API_CALL llge_GraphicsFacade_getDrawCallsCounterValue (IGraphicsFacade * classInstance)
	{
		return classInstance->getDrawCallsCounterValue();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_drawEdges (IGraphicsFacade * classInstance, GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void * vertices, int primitivesCount)
	{
		classInstance->drawEdges(effect, vertexFormat, vertices, primitivesCount);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_draw (IGraphicsFacade * classInstance, GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void * vertices, int primitivesCount)
	{
		classInstance->draw(effect, vertexFormat, vertices, primitivesCount);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_drawElements (IGraphicsFacade * classInstance, GraphicsEffects effect, GraphicsVertexFormats vertexFormat, void * vertices, void * indices, int primitivesCount)
	{
		classInstance->drawElements(effect, vertexFormat, vertices, indices, primitivesCount);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_drawVertexBuffer (IGraphicsFacade * classInstance, GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IVertexBuffer * vertexBuffer, void * indices, int primitivesCount)
	{
		classInstance->drawVertexBuffer(effect, vertexFormat, vertexBuffer, indices, primitivesCount);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_setEffectConstantFloat (IGraphicsFacade * classInstance, GraphicsEffects effect, char * name, float value)
	{
		classInstance->setEffectConstantFloat(effect, name, value);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_setEffectConstantColor (IGraphicsFacade * classInstance, GraphicsEffects effect, char * name, uint value)
	{
		classInstance->setEffectConstantColor(effect, name, value);
	}
	
	extern "C" DLLEXPORT int API_CALL llge_GraphicsFacade_getPixelsWidth (IGraphicsFacade * classInstance)
	{
		return classInstance->getPixelsWidth();
	}
	
	extern "C" DLLEXPORT int API_CALL llge_GraphicsFacade_getPixelsHeight (IGraphicsFacade * classInstance)
	{
		return classInstance->getPixelsHeight();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_getPixels (IGraphicsFacade * classInstance, IntPtr target)
	{
		classInstance->getPixels(target);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_create (IGraphicsFacade * classInstance)
	{
		classInstance->create();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_GraphicsFacade_grabDefaultRenderTarget (IGraphicsFacade * classInstance)
	{
		classInstance->grabDefaultRenderTarget();
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
	
	extern "C" DLLEXPORT ITextureImage2d * API_CALL llge_GraphicsFactory_createTextureImage2d (IGraphicsFactory * classInstance, bool generateMipmaps, bool useFilter)
	{
		return classInstance->createTextureImage2d(generateMipmaps, useFilter);
	}
	
	extern "C" DLLEXPORT IRenderTarget2d * API_CALL llge_GraphicsFactory_createRenderTarget2d (IGraphicsFactory * classInstance)
	{
		return classInstance->createRenderTarget2d();
	}
	
	extern "C" DLLEXPORT IRenderTargetDepth2d * API_CALL llge_GraphicsFactory_createRenderTargetDepth2d (IGraphicsFactory * classInstance)
	{
		return classInstance->createRenderTargetDepth2d();
	}
	
	extern "C" DLLEXPORT IVertexBuffer * API_CALL llge_GraphicsFactory_createVertexBuffer (IGraphicsFactory * classInstance)
	{
		return classInstance->createVertexBuffer();
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
	
	extern "C" DLLEXPORT void API_CALL llge_Aabb2d_update (IAabb2d * classInstance, float minX, float minY, float maxX, float maxY, float zOrder)
	{
		classInstance->update(minX, minY, maxX, maxY, zOrder);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Render2d_setMeshesCount (IRender2d * classInstance, int meshesCount)
	{
		classInstance->setMeshesCount(meshesCount);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Render2d_setMesh (IRender2d * classInstance, int meshIndex, ITexture * texture, void * vertices, int verticesCount, void * indices, int indicesCount)
	{
		classInstance->setMesh(meshIndex, texture, vertices, verticesCount, indices, indicesCount);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_MatrixTransform_setTransform (IMatrixTransform * classInstance, void * floatMatrix)
	{
		classInstance->setTransform(floatMatrix);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Transform2d_setWorldPosition (ITransform2d * classInstance, float x, float y, float z)
	{
		classInstance->setWorldPosition(x, y, z);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Transform2d_setWorldRotation (ITransform2d * classInstance, float value)
	{
		classInstance->setWorldRotation(value);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Transform2d_setWorldScale (ITransform2d * classInstance, float value)
	{
		classInstance->setWorldScale(value);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Transform2d_setLocalPivot (ITransform2d * classInstance, float x, float y, float z)
	{
		classInstance->setLocalPivot(x, y, z);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Transform2d_setLocalPosition (ITransform2d * classInstance, float x, float y, float z)
	{
		classInstance->setLocalPosition(x, y, z);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Transform2d_setLocalRotation (ITransform2d * classInstance, float value)
	{
		classInstance->setLocalRotation(value);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Transform2d_setLocalScale (ITransform2d * classInstance, float value)
	{
		classInstance->setLocalScale(value);
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_Entity_getSelfInstance (IEntity * classInstance)
	{
		return classInstance->getSelfInstance();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Entity_setComponents (IEntity * classInstance, ComponentsTypes types)
	{
		classInstance->setComponents(types);
	}
	
	extern "C" DLLEXPORT IAabb2d * API_CALL llge_Entity_getAabb2d (IEntity * classInstance)
	{
		return classInstance->getAabb2d();
	}
	
	extern "C" DLLEXPORT IRender2d * API_CALL llge_Entity_getRender2d (IEntity * classInstance)
	{
		return classInstance->getRender2d();
	}
	
	extern "C" DLLEXPORT ITransform2d * API_CALL llge_Entity_getTransform2d (IEntity * classInstance)
	{
		return classInstance->getTransform2d();
	}
	
	extern "C" DLLEXPORT IMatrixTransform * API_CALL llge_Entity_getMatrixTransform (IEntity * classInstance)
	{
		return classInstance->getMatrixTransform();
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
	
	extern "C" DLLEXPORT IEntity * API_CALL llge_EntitiesWorld_createEntity (IEntitiesWorld * classInstance)
	{
		return classInstance->createEntity();
	}
	
	extern "C" DLLEXPORT int API_CALL llge_EntitiesWorld_update (IEntitiesWorld * classInstance, float elapsed)
	{
		return classInstance->update(elapsed);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_EntitiesWorld_updateEntity (IEntitiesWorld * classInstance, IEntity * entity, ComponentsTypes types)
	{
		classInstance->updateEntity(entity, types);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_EntitiesWorld_addEntity (IEntitiesWorld * classInstance, IEntity * entity)
	{
		classInstance->addEntity(entity);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_EntitiesWorld_removeEntity (IEntitiesWorld * classInstance, IEntity * entity)
	{
		classInstance->removeEntity(entity);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_EntitiesWorld_dispose (IEntitiesWorld * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_EntitiesWorld_clear (IEntitiesWorld * classInstance)
	{
		classInstance->clear();
	}
	
	extern "C" DLLEXPORT IEntitiesWorld * API_CALL llge_EntitiesFactory_createEntitiesWorld (IEntitiesFactory * classInstance)
	{
		return classInstance->createEntitiesWorld();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_EntitiesFactory_dispose (IEntitiesFactory * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_Batch2d_getNativeInstance (IBatch2d * classInstance)
	{
		return classInstance->getNativeInstance();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Batch2d_addProjection (IBatch2d * classInstance, void * floatMatrix)
	{
		classInstance->addProjection(floatMatrix);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Batch2d_addRenderTarget (IBatch2d * classInstance, IntPtr * renderTargetInstance)
	{
		classInstance->addRenderTarget(renderTargetInstance);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Batch2d_startBatch (IBatch2d * classInstance)
	{
		classInstance->startBatch();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Batch2d_finishBatch (IBatch2d * classInstance)
	{
		classInstance->finishBatch();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Batch2d_setToneMap (IBatch2d * classInstance, uint tonemapId)
	{
		classInstance->setToneMap(tonemapId);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Batch2d_drawEx (IBatch2d * classInstance, GraphicsEffects effect, BlendMode blendMode, IntPtr config, void * vertices, int verticesCount, void * indices, int indicesCount)
	{
		classInstance->drawEx(effect, blendMode, config, vertices, verticesCount, indices, indicesCount);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Batch2d_draw (IBatch2d * classInstance, GraphicsEffects effect, BlendMode blendMode, ITexture * textureId, uint lightmapId, void * vertices, int verticesCount, void * indices, int indicesCount)
	{
		classInstance->draw(effect, blendMode, textureId, lightmapId, vertices, verticesCount, indices, indicesCount);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_Batch2d_execute (IBatch2d * classInstance, bool usePostProcess)
	{
		classInstance->execute(usePostProcess);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineSkeleton_setTransform (ISpineSkeleton * classInstance, void * floatMatrix)
	{
		classInstance->setTransform(floatMatrix);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineSkeleton_setColor (ISpineSkeleton * classInstance, uint color)
	{
		classInstance->setColor(color);
	}
	
	extern "C" DLLEXPORT float API_CALL llge_SpineSkeleton_getMinX (ISpineSkeleton * classInstance)
	{
		return classInstance->getMinX();
	}
	
	extern "C" DLLEXPORT float API_CALL llge_SpineSkeleton_getMinY (ISpineSkeleton * classInstance)
	{
		return classInstance->getMinY();
	}
	
	extern "C" DLLEXPORT float API_CALL llge_SpineSkeleton_getMaxX (ISpineSkeleton * classInstance)
	{
		return classInstance->getMaxX();
	}
	
	extern "C" DLLEXPORT float API_CALL llge_SpineSkeleton_getMaxY (ISpineSkeleton * classInstance)
	{
		return classInstance->getMaxY();
	}
	
	extern "C" DLLEXPORT float API_CALL llge_SpineSkeleton_getZ (ISpineSkeleton * classInstance)
	{
		return classInstance->getZ();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineSkeleton_renderEx (ISpineSkeleton * classInstance, IBatch2d * batch, IntPtr effectConfig, GraphicsEffects effect)
	{
		classInstance->renderEx(batch, effectConfig, effect);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineSkeleton_render (ISpineSkeleton * classInstance, IBatch2d * batch, int lightmapId, GraphicsEffects effect)
	{
		classInstance->render(batch, lightmapId, effect);
	}
	
	extern "C" DLLEXPORT int API_CALL llge_SpineSkeleton_getGeometry (ISpineSkeleton * classInstance, void * vertices, int verticeLimit, void * indices, int indicesLimit)
	{
		return classInstance->getGeometry(vertices, verticeLimit, indices, indicesLimit);
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_SpineSkeleton_getNativeInstance (ISpineSkeleton * classInstance)
	{
		return classInstance->getNativeInstance();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineSkeleton_updateWorldTransform (ISpineSkeleton * classInstance)
	{
		classInstance->updateWorldTransform();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineSkeleton_setBonesToSetupPose (ISpineSkeleton * classInstance)
	{
		classInstance->setBonesToSetupPose();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineSkeleton_setSlotsToSetupPose (ISpineSkeleton * classInstance)
	{
		classInstance->setSlotsToSetupPose();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineSkeleton_applySkin (ISpineSkeleton * classInstance, IntPtr spineSkinNativeInstance)
	{
		classInstance->applySkin(spineSkinNativeInstance);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineSkeleton_dispose (ISpineSkeleton * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_SpineSkin_getNativeInstance (ISpineSkin * classInstance)
	{
		return classInstance->getNativeInstance();
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_SpineSkin_getName (ISpineSkin * classInstance)
	{
		return classInstance->getName();
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_SpineAnimation_getNativeInstance (ISpineAnimation * classInstance)
	{
		return classInstance->getNativeInstance();
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_SpineAnimation_getName (ISpineAnimation * classInstance)
	{
		return classInstance->getName();
	}
	
	extern "C" DLLEXPORT float API_CALL llge_SpineAnimation_getDuration (ISpineAnimation * classInstance)
	{
		return classInstance->getDuration();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineAnimationState_update (ISpineAnimationState * classInstance, float delta)
	{
		classInstance->update(delta);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineAnimationState_apply (ISpineAnimationState * classInstance, ISpineSkeleton * skeleton)
	{
		classInstance->apply(skeleton);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineAnimationState_setAnimation (ISpineAnimationState * classInstance, ISpineAnimation * animation, bool loop, bool normalize)
	{
		classInstance->setAnimation(animation, loop, normalize);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineAnimationState_addAnimation (ISpineAnimationState * classInstance, ISpineAnimation * animation, bool loop, float delay)
	{
		classInstance->addAnimation(animation, loop, delay);
	}
	
	extern "C" DLLEXPORT int API_CALL llge_SpineAnimationState_getSpineEventIndices (ISpineAnimationState * classInstance, IntPtr indices, int limit)
	{
		return classInstance->getSpineEventIndices(indices, limit);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineAnimationState_dispose (ISpineAnimationState * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineAnimationStateData_setMix (ISpineAnimationStateData * classInstance, ISpineAnimation * from, ISpineAnimation * to, float time)
	{
		classInstance->setMix(from, to, time);
	}
	
	extern "C" DLLEXPORT ISpineAnimationState * API_CALL llge_SpineAnimationStateData_createState (ISpineAnimationStateData * classInstance)
	{
		return classInstance->createState();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineAnimationStateData_dispose (ISpineAnimationStateData * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_SpineEvent_getName (ISpineEvent * classInstance)
	{
		return classInstance->getName();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineResource_load (ISpineResource * classInstance, String atlasText, String jsonText, String dir)
	{
		classInstance->load(atlasText, jsonText, dir);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineResource_unLoad (ISpineResource * classInstance)
	{
		classInstance->unLoad();
	}
	
	extern "C" DLLEXPORT ISpineAnimation * API_CALL llge_SpineResource_getSpineAnimation (ISpineResource * classInstance, int i)
	{
		return classInstance->getSpineAnimation(i);
	}
	
	extern "C" DLLEXPORT int API_CALL llge_SpineResource_getSpineAnimationsCount (ISpineResource * classInstance)
	{
		return classInstance->getSpineAnimationsCount();
	}
	
	extern "C" DLLEXPORT ISpineSkin * API_CALL llge_SpineResource_getSpineSkin (ISpineResource * classInstance, int i)
	{
		return classInstance->getSpineSkin(i);
	}
	
	extern "C" DLLEXPORT int API_CALL llge_SpineResource_getSpineSkinsCount (ISpineResource * classInstance)
	{
		return classInstance->getSpineSkinsCount();
	}
	
	extern "C" DLLEXPORT ISpineEvent * API_CALL llge_SpineResource_getSpineEvent (ISpineResource * classInstance, int i)
	{
		return classInstance->getSpineEvent(i);
	}
	
	extern "C" DLLEXPORT int API_CALL llge_SpineResource_getSpineEventsCount (ISpineResource * classInstance)
	{
		return classInstance->getSpineEventsCount();
	}
	
	extern "C" DLLEXPORT ISpineSkeleton * API_CALL llge_SpineResource_createSkeleton (ISpineResource * classInstance, void * floatMatrix)
	{
		return classInstance->createSkeleton(floatMatrix);
	}
	
	extern "C" DLLEXPORT ISpineAnimationStateData * API_CALL llge_SpineResource_createStateData (ISpineResource * classInstance)
	{
		return classInstance->createStateData();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_SpineResource_dispose (ISpineResource * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT int API_CALL llge_NativeMemoryProfiler_getTexturesSize (INativeMemoryProfiler * classInstance)
	{
		return classInstance->getTexturesSize();
	}
	
	extern "C" DLLEXPORT int API_CALL llge_NativeMemoryProfiler_getAllocationsSize (INativeMemoryProfiler * classInstance)
	{
		return classInstance->getAllocationsSize();
	}
	
	extern "C" DLLEXPORT int API_CALL llge_NativeMemoryProfiler_getHeapSize (INativeMemoryProfiler * classInstance)
	{
		return classInstance->getHeapSize();
	}
	
	extern "C" DLLEXPORT TextureImage2dFormat API_CALL llge_TextureBuffer2d_getFormat (ITextureBuffer2d * classInstance)
	{
		return classInstance->getFormat();
	}
	
	extern "C" DLLEXPORT int API_CALL llge_TextureBuffer2d_getWidth (ITextureBuffer2d * classInstance)
	{
		return classInstance->getWidth();
	}
	
	extern "C" DLLEXPORT int API_CALL llge_TextureBuffer2d_getHeight (ITextureBuffer2d * classInstance)
	{
		return classInstance->getHeight();
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_TextureBuffer2d_getPixels (ITextureBuffer2d * classInstance)
	{
		return classInstance->getPixels();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_ContentAtlasMap_resetMap (IContentAtlasMap * classInstance)
	{
		classInstance->resetMap();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_ContentAtlasMap_addRect (IContentAtlasMap * classInstance, char * name, int pageIndex, int x, int y, int width, int height)
	{
		classInstance->addRect(name, pageIndex, x, y, width, height);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_ContentAtlasMap_loadTextures (IContentAtlasMap * classInstance)
	{
		classInstance->loadTextures();
	}
	
	extern "C" DLLEXPORT IContentAtlasMap * API_CALL llge_ContentManager_getContentAtlasMap (IContentManager * classInstance)
	{
		return classInstance->getContentAtlasMap();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_ContentManager_replaceSeparator (IContentManager * classInstance, bool value)
	{
		classInstance->replaceSeparator(value);
	}
	
	extern "C" DLLEXPORT int API_CALL llge_ContentManager_registerImage (IContentManager * classInstance, char * name)
	{
		return classInstance->registerImage(name);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_ContentManager_startLoad (IContentManager * classInstance)
	{
		classInstance->startLoad();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_ContentManager_loadImage (IContentManager * classInstance, int id, ITextureImage2d * textureImage)
	{
		classInstance->loadImage(id, textureImage);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_ContentManager_reloadImages (IContentManager * classInstance)
	{
		classInstance->reloadImages();
	}
	
	extern "C" DLLEXPORT ITextureBuffer2d * API_CALL llge_ContentManager_loadBuffer (IContentManager * classInstance, int id)
	{
		return classInstance->loadBuffer(id);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_ContentManager_finishLoad (IContentManager * classInstance)
	{
		classInstance->finishLoad();
	}
	
	extern "C" DLLEXPORT bool API_CALL llge_ContentManager_update (IContentManager * classInstance)
	{
		return classInstance->update();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_ContentManager_dispose (IContentManager * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_ObbContentProvider_openObbFile (IObbContentProvider * classInstance, const char * obbFile)
	{
		classInstance->openObbFile(obbFile);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_ObbContentProvider_closeObbFile (IObbContentProvider * classInstance)
	{
		classInstance->closeObbFile();
	}
	
	extern "C" DLLEXPORT bool API_CALL llge_ObbContentProvider_existsContent (IObbContentProvider * classInstance, const char * name)
	{
		return classInstance->existsContent(name);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_ObbContentProvider_openContent (IObbContentProvider * classInstance, const char * name)
	{
		classInstance->openContent(name);
	}
	
	extern "C" DLLEXPORT int API_CALL llge_ObbContentProvider_read (IObbContentProvider * classInstance, void * buffer, int bytesLimit)
	{
		return classInstance->read(buffer, bytesLimit);
	}
	
	extern "C" DLLEXPORT int API_CALL llge_ObbContentProvider_getContentSize (IObbContentProvider * classInstance)
	{
		return classInstance->getContentSize();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_ObbContentProvider_closeContent (IObbContentProvider * classInstance)
	{
		classInstance->closeContent();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_NavMeshConfig_clear (INavMeshConfig * classInstance)
	{
		classInstance->clear();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_NavMeshConfig_addPolygon (INavMeshConfig * classInstance, IntPtr vertices2f, uint verticesCount)
	{
		classInstance->addPolygon(vertices2f, verticesCount);
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_NavMeshConfig_getNativeInstance (INavMeshConfig * classInstance)
	{
		return classInstance->getNativeInstance();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_NavMeshConfig_dispose (INavMeshConfig * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT int API_CALL llge_NavPath_getVerticesCount (INavPath * classInstance)
	{
		return classInstance->getVerticesCount();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_NavPath_getVertices (INavPath * classInstance, IntPtr vertices2f)
	{
		classInstance->getVertices(vertices2f);
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_NavPath_getNativeInstance (INavPath * classInstance)
	{
		return classInstance->getNativeInstance();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_PathMesh_fillNavPath (IPathMesh * classInstance, INavPath * path)
	{
		classInstance->fillNavPath(path);
	}
	
	extern "C" DLLEXPORT int API_CALL llge_PathMesh_getTriagnlesCount (IPathMesh * classInstance)
	{
		return classInstance->getTriagnlesCount();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_PathMesh_getTriagnles (IPathMesh * classInstance, IntPtr triangles3f)
	{
		classInstance->getTriagnles(triangles3f);
	}
	
	extern "C" DLLEXPORT IntPtr API_CALL llge_PathMesh_getNativeInstance (IPathMesh * classInstance)
	{
		return classInstance->getNativeInstance();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_PathMesh_dispose (IPathMesh * classInstance)
	{
		classInstance->dispose();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_NavMesh_buildNavMesh (INavMesh * classInstance, INavMeshConfig * config)
	{
		classInstance->buildNavMesh(config);
	}
	
	extern "C" DLLEXPORT int API_CALL llge_NavMesh_getTriagnlesCount (INavMesh * classInstance)
	{
		return classInstance->getTriagnlesCount();
	}
	
	extern "C" DLLEXPORT void API_CALL llge_NavMesh_getTriagnles (INavMesh * classInstance, IntPtr triangles3f)
	{
		classInstance->getTriagnles(triangles3f);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_NavMesh_fillPathMesh (INavMesh * classInstance, float x0, float y0, float x1, float y1, IPathMesh * mesh)
	{
		classInstance->fillPathMesh(x0, y0, x1, y1, mesh);
	}
	
	extern "C" DLLEXPORT void API_CALL llge_NavMesh_dispose (INavMesh * classInstance)
	{
		classInstance->dispose();
	}
	
}

