/* ============================================================== */
/* This file is automatically generated. Please do not modify it. */
/* ============================================================== */

using System;
using System.Runtime.InteropServices;

namespace llge
{
	public class Version
	{
		public const string Dll = "llge";
	}
	
	public enum BlendMode
	{
		None = 0x0,
		Alpha = 0x1,
		Additive = 0x2,
	}
	
	public enum GraphicsEffects
	{
		EffectTextureColor = 0x0,
		EffectTextureLightmapColor = 0x1,
		EffectWater = 0x2,
		EffectSolid = 0x3,
		EffectRenderDepth = 0x4,
		EffectSolidColor = 0x5,
	}
	
	public enum GraphicsVertexFormats
	{
		FormatPositionTextureColor = 0x0,
		FormatPositionNormal = 0x1,
		FormatPositionTexture = 0x2,
		FormatPositionColor = 0x3,
	}
	
	public enum CubemapPlane
	{
		PositiveX = 0x0,
		NegativeX = 0x1,
		PositiveY = 0x2,
		NegativeY = 0x3,
		PositiveZ = 0x4,
		NegativeZ = 0x5,
	}
	
	public enum TextureImage2dFormat
	{
		Rgba = 0x0,
		Rgb = 0x1,
	}
	
	public enum ComponentsTypes
	{
		Aadd2d = 0x1,
		Transform2d = 0x2,
		Render2d = 0x4,
		MatrixTransform = 0x8,
	}
	
	public class Texture
	{
		public IntPtr ClassInstance;
		public uint GetId ()
		{
			return llge_Texture_getId(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private uint llge_Texture_getId (IntPtr classInstance);
		public void Dispose ()
		{
			llge_Texture_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Texture_dispose (IntPtr classInstance);
	}
	
	public class TextureImage2d
	{
		public IntPtr ClassInstance;
		public Texture GetTexture ()
		{
			return new Texture{ ClassInstance = llge_TextureImage2d_getTexture(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_TextureImage2d_getTexture (IntPtr classInstance);
		public void LoadPixels (int width, int height, TextureImage2dFormat format, IntPtr pixels)
		{
			llge_TextureImage2d_LoadPixels(ClassInstance, width, height, format, pixels);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_TextureImage2d_LoadPixels (IntPtr classInstance, int width, int height, TextureImage2dFormat format, IntPtr pixels);
		public void Create ()
		{
			llge_TextureImage2d_create(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_TextureImage2d_create (IntPtr classInstance);
		public void Cleanup ()
		{
			llge_TextureImage2d_cleanup(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_TextureImage2d_cleanup (IntPtr classInstance);
		public void Dispose ()
		{
			llge_TextureImage2d_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_TextureImage2d_dispose (IntPtr classInstance);
	}
	
	public class RenderTarget2d
	{
		public IntPtr ClassInstance;
		public IntPtr GetRenderTargetInstance ()
		{
			return llge_RenderTarget2d_getRenderTargetInstance(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_RenderTarget2d_getRenderTargetInstance (IntPtr classInstance);
		public Texture GetTexture ()
		{
			return new Texture{ ClassInstance = llge_RenderTarget2d_getTexture(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_RenderTarget2d_getTexture (IntPtr classInstance);
		public void Create (int width, int height)
		{
			llge_RenderTarget2d_create(ClassInstance, width, height);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_RenderTarget2d_create (IntPtr classInstance, int width, int height);
		public void Cleanup ()
		{
			llge_RenderTarget2d_cleanup(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_RenderTarget2d_cleanup (IntPtr classInstance);
		public void Dispose ()
		{
			llge_RenderTarget2d_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_RenderTarget2d_dispose (IntPtr classInstance);
	}
	
	public class RenderTargetDepth2d
	{
		public IntPtr ClassInstance;
		public IntPtr GetRenderTargetInstance ()
		{
			return llge_RenderTargetDepth2d_getRenderTargetInstance(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_RenderTargetDepth2d_getRenderTargetInstance (IntPtr classInstance);
		public Texture GetTexture ()
		{
			return new Texture{ ClassInstance = llge_RenderTargetDepth2d_getTexture(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_RenderTargetDepth2d_getTexture (IntPtr classInstance);
		public Texture GetDepthTexture ()
		{
			return new Texture{ ClassInstance = llge_RenderTargetDepth2d_getDepthTexture(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_RenderTargetDepth2d_getDepthTexture (IntPtr classInstance);
		public void Create (int width, int height)
		{
			llge_RenderTargetDepth2d_create(ClassInstance, width, height);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_RenderTargetDepth2d_create (IntPtr classInstance, int width, int height);
		public void Cleanup ()
		{
			llge_RenderTargetDepth2d_cleanup(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_RenderTargetDepth2d_cleanup (IntPtr classInstance);
		public void Dispose ()
		{
			llge_RenderTargetDepth2d_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_RenderTargetDepth2d_dispose (IntPtr classInstance);
	}
	
	public class UniformsFacade
	{
		public IntPtr ClassInstance;
		public void SetTime (float value)
		{
			llge_UniformsFacade_setTime(ClassInstance, value);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_UniformsFacade_setTime (IntPtr classInstance, float value);
		public void SetNormalmap (Texture texture)
		{
			llge_UniformsFacade_setNormalmap(ClassInstance, texture.ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_UniformsFacade_setNormalmap (IntPtr classInstance, IntPtr texture);
		public void SetEnvironment (Texture texture)
		{
			llge_UniformsFacade_setEnvironment(ClassInstance, texture.ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_UniformsFacade_setEnvironment (IntPtr classInstance, IntPtr texture);
		public void SetDepthmap (Texture texture)
		{
			llge_UniformsFacade_setDepthmap(ClassInstance, texture.ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_UniformsFacade_setDepthmap (IntPtr classInstance, IntPtr texture);
		public void SetTexture (Texture texture)
		{
			llge_UniformsFacade_setTexture(ClassInstance, texture.ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_UniformsFacade_setTexture (IntPtr classInstance, IntPtr texture);
		public void SetLightMap (Texture texture)
		{
			llge_UniformsFacade_setLightMap(ClassInstance, texture.ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_UniformsFacade_setLightMap (IntPtr classInstance, IntPtr texture);
		public void SetProjection (IntPtr floatMatrix)
		{
			llge_UniformsFacade_setProjection(ClassInstance, floatMatrix);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_UniformsFacade_setProjection (IntPtr classInstance, IntPtr floatMatrix);
	}
	
	public class VertexBuffer
	{
		public IntPtr ClassInstance;
		public int GetId ()
		{
			return llge_VertexBuffer_getId(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private int llge_VertexBuffer_getId (IntPtr classInstance);
		public void Create ()
		{
			llge_VertexBuffer_create(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_VertexBuffer_create (IntPtr classInstance);
		public void SetData (IntPtr data, int count)
		{
			llge_VertexBuffer_setData(ClassInstance, data, count);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_VertexBuffer_setData (IntPtr classInstance, IntPtr data, int count);
		public void Cleanup ()
		{
			llge_VertexBuffer_cleanup(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_VertexBuffer_cleanup (IntPtr classInstance);
		public void Dispose ()
		{
			llge_VertexBuffer_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_VertexBuffer_dispose (IntPtr classInstance);
	}
	
	public class GraphicsFacade
	{
		public IntPtr ClassInstance;
		public UniformsFacade GetUniforms ()
		{
			return new UniformsFacade{ ClassInstance = llge_GraphicsFacade_getUniforms(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_GraphicsFacade_getUniforms (IntPtr classInstance);
		public void Viewport (int width, int height)
		{
			llge_GraphicsFacade_viewport(ClassInstance, width, height);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_viewport (IntPtr classInstance, int width, int height);
		public void SetRenderTarget (IntPtr renderTargetInstance)
		{
			llge_GraphicsFacade_setRenderTarget(ClassInstance, renderTargetInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_setRenderTarget (IntPtr classInstance, IntPtr renderTargetInstance);
		public void SetClearState (uint color, float depth)
		{
			llge_GraphicsFacade_setClearState(ClassInstance, color, depth);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_setClearState (IntPtr classInstance, uint color, float depth);
		public void SetBlendMode (BlendMode blendMode)
		{
			llge_GraphicsFacade_setBlendMode(ClassInstance, blendMode);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_setBlendMode (IntPtr classInstance, BlendMode blendMode);
		public void Clear ()
		{
			llge_GraphicsFacade_clear(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_clear (IntPtr classInstance);
		public void ClearDepth ()
		{
			llge_GraphicsFacade_clearDepth(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_clearDepth (IntPtr classInstance);
		public void DrawEdges (GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IntPtr vertices, int primitivesCount)
		{
			llge_GraphicsFacade_drawEdges(ClassInstance, effect, vertexFormat, vertices, primitivesCount);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_drawEdges (IntPtr classInstance, GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IntPtr vertices, int primitivesCount);
		public void Draw (GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IntPtr vertices, int primitivesCount)
		{
			llge_GraphicsFacade_draw(ClassInstance, effect, vertexFormat, vertices, primitivesCount);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_draw (IntPtr classInstance, GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IntPtr vertices, int primitivesCount);
		public void DrawElements (GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IntPtr vertices, IntPtr indices, int primitivesCount)
		{
			llge_GraphicsFacade_drawElements(ClassInstance, effect, vertexFormat, vertices, indices, primitivesCount);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_drawElements (IntPtr classInstance, GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IntPtr vertices, IntPtr indices, int primitivesCount);
		public void DrawVertexBuffer (GraphicsEffects effect, GraphicsVertexFormats vertexFormat, VertexBuffer vertexBuffer, IntPtr indices, int primitivesCount)
		{
			llge_GraphicsFacade_drawVertexBuffer(ClassInstance, effect, vertexFormat, vertexBuffer.ClassInstance, indices, primitivesCount);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_drawVertexBuffer (IntPtr classInstance, GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IntPtr vertexBuffer, IntPtr indices, int primitivesCount);
		public void SetEffectConstantFloat (GraphicsEffects effect, string name, float value)
		{
			llge_GraphicsFacade_setEffectConstantFloat(ClassInstance, effect, name, value);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_setEffectConstantFloat (IntPtr classInstance, GraphicsEffects effect, string name, float value);
		public void SetEffectConstantColor (GraphicsEffects effect, string name, uint value)
		{
			llge_GraphicsFacade_setEffectConstantColor(ClassInstance, effect, name, value);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_setEffectConstantColor (IntPtr classInstance, GraphicsEffects effect, string name, uint value);
		public void Create ()
		{
			llge_GraphicsFacade_create(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_create (IntPtr classInstance);
		public void Cleanup ()
		{
			llge_GraphicsFacade_cleanup(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_cleanup (IntPtr classInstance);
		public void Dispose ()
		{
			llge_GraphicsFacade_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_dispose (IntPtr classInstance);
	}
	
	public class GraphicsFactory
	{
		public IntPtr ClassInstance;
		public GraphicsFacade CreateGraphicsFacade ()
		{
			return new GraphicsFacade{ ClassInstance = llge_GraphicsFactory_createGraphicsFacade(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_GraphicsFactory_createGraphicsFacade (IntPtr classInstance);
		public TextureImage2d CreateTextureImage2d (bool generateMipmaps)
		{
			return new TextureImage2d{ ClassInstance = llge_GraphicsFactory_createTextureImage2d(ClassInstance, generateMipmaps) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_GraphicsFactory_createTextureImage2d (IntPtr classInstance, bool generateMipmaps);
		public RenderTarget2d CreateRenderTarget2d ()
		{
			return new RenderTarget2d{ ClassInstance = llge_GraphicsFactory_createRenderTarget2d(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_GraphicsFactory_createRenderTarget2d (IntPtr classInstance);
		public RenderTargetDepth2d CreateRenderTargetDepth2d ()
		{
			return new RenderTargetDepth2d{ ClassInstance = llge_GraphicsFactory_createRenderTargetDepth2d(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_GraphicsFactory_createRenderTargetDepth2d (IntPtr classInstance);
		public VertexBuffer CreateVertexBuffer ()
		{
			return new VertexBuffer{ ClassInstance = llge_GraphicsFactory_createVertexBuffer(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_GraphicsFactory_createVertexBuffer (IntPtr classInstance);
		public void Dispose ()
		{
			llge_GraphicsFactory_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFactory_dispose (IntPtr classInstance);
	}
	
	public class QuadTree
	{
		public IntPtr ClassInstance;
		public int Insert (float minX, float minY, float maxX, float maxY, int userData)
		{
			return llge_QuadTree_insert(ClassInstance, minX, minY, maxX, maxY, userData);
		}
		
		[DllImport(Version.Dll)]
		static extern private int llge_QuadTree_insert (IntPtr classInstance, float minX, float minY, float maxX, float maxY, int userData);
		public void Remove (int id)
		{
			llge_QuadTree_remove(ClassInstance, id);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_QuadTree_remove (IntPtr classInstance, int id);
		public void Query (float minX, float minY, float maxX, float maxY)
		{
			llge_QuadTree_query(ClassInstance, minX, minY, maxX, maxY);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_QuadTree_query (IntPtr classInstance, float minX, float minY, float maxX, float maxY);
		public void GetQueryResults (IntPtr intBuffer)
		{
			llge_QuadTree_getQueryResults(ClassInstance, intBuffer);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_QuadTree_getQueryResults (IntPtr classInstance, IntPtr intBuffer);
		public int GetQueryResultsCount ()
		{
			return llge_QuadTree_getQueryResultsCount(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private int llge_QuadTree_getQueryResultsCount (IntPtr classInstance);
		public int GetIterationsCount ()
		{
			return llge_QuadTree_getIterationsCount(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private int llge_QuadTree_getIterationsCount (IntPtr classInstance);
		public void Dispose ()
		{
			llge_QuadTree_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_QuadTree_dispose (IntPtr classInstance);
	}
	
	public class GeometryFactory
	{
		public IntPtr ClassInstance;
		public QuadTree CreateQuadTree ()
		{
			return new QuadTree{ ClassInstance = llge_GeometryFactory_createQuadTree(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_GeometryFactory_createQuadTree (IntPtr classInstance);
		public void Dispose ()
		{
			llge_GeometryFactory_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GeometryFactory_dispose (IntPtr classInstance);
	}
	
	public class Aabb2d
	{
		public IntPtr ClassInstance;
		public void Update (float minX, float minY, float maxX, float maxY, float zOrder)
		{
			llge_Aabb2d_update(ClassInstance, minX, minY, maxX, maxY, zOrder);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Aabb2d_update (IntPtr classInstance, float minX, float minY, float maxX, float maxY, float zOrder);
	}
	
	public class Render2d
	{
		public IntPtr ClassInstance;
		public void SetMeshesCount (int meshesCount)
		{
			llge_Render2d_setMeshesCount(ClassInstance, meshesCount);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Render2d_setMeshesCount (IntPtr classInstance, int meshesCount);
		public void SetMesh (int meshIndex, Texture texture, IntPtr vertices, int verticesCount, IntPtr indices, int indicesCount)
		{
			llge_Render2d_setMesh(ClassInstance, meshIndex, texture.ClassInstance, vertices, verticesCount, indices, indicesCount);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Render2d_setMesh (IntPtr classInstance, int meshIndex, IntPtr texture, IntPtr vertices, int verticesCount, IntPtr indices, int indicesCount);
	}
	
	public class MatrixTransform
	{
		public IntPtr ClassInstance;
		public void SetTransform (IntPtr floatMatrix)
		{
			llge_MatrixTransform_setTransform(ClassInstance, floatMatrix);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_MatrixTransform_setTransform (IntPtr classInstance, IntPtr floatMatrix);
	}
	
	public class Transform2d
	{
		public IntPtr ClassInstance;
		public void SetWorldPosition (float x, float y, float z)
		{
			llge_Transform2d_setWorldPosition(ClassInstance, x, y, z);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Transform2d_setWorldPosition (IntPtr classInstance, float x, float y, float z);
		public void SetWorldRotation (float value)
		{
			llge_Transform2d_setWorldRotation(ClassInstance, value);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Transform2d_setWorldRotation (IntPtr classInstance, float value);
		public void SetWorldScale (float value)
		{
			llge_Transform2d_setWorldScale(ClassInstance, value);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Transform2d_setWorldScale (IntPtr classInstance, float value);
		public void SetLocalPivot (float x, float y, float z)
		{
			llge_Transform2d_setLocalPivot(ClassInstance, x, y, z);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Transform2d_setLocalPivot (IntPtr classInstance, float x, float y, float z);
		public void SetLocalPosition (float x, float y, float z)
		{
			llge_Transform2d_setLocalPosition(ClassInstance, x, y, z);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Transform2d_setLocalPosition (IntPtr classInstance, float x, float y, float z);
		public void SetLocalRotation (float value)
		{
			llge_Transform2d_setLocalRotation(ClassInstance, value);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Transform2d_setLocalRotation (IntPtr classInstance, float value);
		public void SetLocalScale (float value)
		{
			llge_Transform2d_setLocalScale(ClassInstance, value);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Transform2d_setLocalScale (IntPtr classInstance, float value);
	}
	
	public class Entity
	{
		public IntPtr ClassInstance;
		public IntPtr GetSelfInstance ()
		{
			return llge_Entity_getSelfInstance(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_Entity_getSelfInstance (IntPtr classInstance);
		public void SetComponents (ComponentsTypes types)
		{
			llge_Entity_setComponents(ClassInstance, types);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Entity_setComponents (IntPtr classInstance, ComponentsTypes types);
		public Aabb2d GetAabb2d ()
		{
			return new Aabb2d{ ClassInstance = llge_Entity_getAabb2d(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_Entity_getAabb2d (IntPtr classInstance);
		public Render2d GetRender2d ()
		{
			return new Render2d{ ClassInstance = llge_Entity_getRender2d(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_Entity_getRender2d (IntPtr classInstance);
		public Transform2d GetTransform2d ()
		{
			return new Transform2d{ ClassInstance = llge_Entity_getTransform2d(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_Entity_getTransform2d (IntPtr classInstance);
		public MatrixTransform GetMatrixTransform ()
		{
			return new MatrixTransform{ ClassInstance = llge_Entity_getMatrixTransform(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_Entity_getMatrixTransform (IntPtr classInstance);
		public void Dispose ()
		{
			llge_Entity_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Entity_dispose (IntPtr classInstance);
	}
	
	public class Camera
	{
		public IntPtr ClassInstance;
		public void SetPosition (float x, float y, float z)
		{
			llge_Camera_setPosition(ClassInstance, x, y, z);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Camera_setPosition (IntPtr classInstance, float x, float y, float z);
		public void SetFov (float fov)
		{
			llge_Camera_setFov(ClassInstance, fov);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Camera_setFov (IntPtr classInstance, float fov);
		public void SetAspect (float aspect)
		{
			llge_Camera_setAspect(ClassInstance, aspect);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Camera_setAspect (IntPtr classInstance, float aspect);
		public void SetRotation (float rotationZ)
		{
			llge_Camera_setRotation(ClassInstance, rotationZ);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Camera_setRotation (IntPtr classInstance, float rotationZ);
		public void SetPlanes (float zn, float zf)
		{
			llge_Camera_setPlanes(ClassInstance, zn, zf);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Camera_setPlanes (IntPtr classInstance, float zn, float zf);
	}
	
	public class EntitiesWorld
	{
		public IntPtr ClassInstance;
		public Camera GetCamera ()
		{
			return new Camera{ ClassInstance = llge_EntitiesWorld_getCamera(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_EntitiesWorld_getCamera (IntPtr classInstance);
		public void SetUnpdateBounds (float minX, float minY, float maxX, float maxY)
		{
			llge_EntitiesWorld_setUnpdateBounds(ClassInstance, minX, minY, maxX, maxY);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_EntitiesWorld_setUnpdateBounds (IntPtr classInstance, float minX, float minY, float maxX, float maxY);
		public void SetRenderBounds (float minX, float minY, float maxX, float maxY)
		{
			llge_EntitiesWorld_setRenderBounds(ClassInstance, minX, minY, maxX, maxY);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_EntitiesWorld_setRenderBounds (IntPtr classInstance, float minX, float minY, float maxX, float maxY);
		public Entity CreateEntity ()
		{
			return new Entity{ ClassInstance = llge_EntitiesWorld_createEntity(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_EntitiesWorld_createEntity (IntPtr classInstance);
		public int Update (float elapsed)
		{
			return llge_EntitiesWorld_update(ClassInstance, elapsed);
		}
		
		[DllImport(Version.Dll)]
		static extern private int llge_EntitiesWorld_update (IntPtr classInstance, float elapsed);
		public void UpdateEntity (Entity entity, ComponentsTypes types)
		{
			llge_EntitiesWorld_updateEntity(ClassInstance, entity.ClassInstance, types);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_EntitiesWorld_updateEntity (IntPtr classInstance, IntPtr entity, ComponentsTypes types);
		public void AddEntity (Entity entity)
		{
			llge_EntitiesWorld_addEntity(ClassInstance, entity.ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_EntitiesWorld_addEntity (IntPtr classInstance, IntPtr entity);
		public void RemoveEntity (Entity entity)
		{
			llge_EntitiesWorld_removeEntity(ClassInstance, entity.ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_EntitiesWorld_removeEntity (IntPtr classInstance, IntPtr entity);
		public void Dispose ()
		{
			llge_EntitiesWorld_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_EntitiesWorld_dispose (IntPtr classInstance);
	}
	
	public class EntitiesFactory
	{
		public IntPtr ClassInstance;
		public EntitiesWorld CreateEntitiesWorld ()
		{
			return new EntitiesWorld{ ClassInstance = llge_EntitiesFactory_createEntitiesWorld(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_EntitiesFactory_createEntitiesWorld (IntPtr classInstance);
		public void Dispose ()
		{
			llge_EntitiesFactory_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_EntitiesFactory_dispose (IntPtr classInstance);
	}
	
	public class Batch2d
	{
		public IntPtr ClassInstance;
		public void SetTransform (IntPtr transform)
		{
			llge_Batch2d_setTransform(ClassInstance, transform);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Batch2d_setTransform (IntPtr classInstance, IntPtr transform);
		public void StartBatch ()
		{
			llge_Batch2d_startBatch(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Batch2d_startBatch (IntPtr classInstance);
		public void FinishBatch ()
		{
			llge_Batch2d_finishBatch(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Batch2d_finishBatch (IntPtr classInstance);
		public void SetTexture (Texture texture)
		{
			llge_Batch2d_setTexture(ClassInstance, texture.ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Batch2d_setTexture (IntPtr classInstance, IntPtr texture);
		public void Draw (IntPtr vertices, int verticesCount, IntPtr indices, int indicesCount)
		{
			llge_Batch2d_draw(ClassInstance, vertices, verticesCount, indices, indicesCount);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Batch2d_draw (IntPtr classInstance, IntPtr vertices, int verticesCount, IntPtr indices, int indicesCount);
	}
	
	public class TextureBuffer2d
	{
		public IntPtr ClassInstance;
		public TextureImage2dFormat GetFormat ()
		{
			return llge_TextureBuffer2d_getFormat(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private TextureImage2dFormat llge_TextureBuffer2d_getFormat (IntPtr classInstance);
		public int GetWidth ()
		{
			return llge_TextureBuffer2d_getWidth(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private int llge_TextureBuffer2d_getWidth (IntPtr classInstance);
		public int GetHeight ()
		{
			return llge_TextureBuffer2d_getHeight(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private int llge_TextureBuffer2d_getHeight (IntPtr classInstance);
		public IntPtr GetPixels ()
		{
			return llge_TextureBuffer2d_getPixels(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_TextureBuffer2d_getPixels (IntPtr classInstance);
	}
	
	public class ContentManager
	{
		public IntPtr ClassInstance;
		public int RegisterImage (string name)
		{
			return llge_ContentManager_registerImage(ClassInstance, name);
		}
		
		[DllImport(Version.Dll)]
		static extern private int llge_ContentManager_registerImage (IntPtr classInstance, string name);
		public void StartLoad ()
		{
			llge_ContentManager_startLoad(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_ContentManager_startLoad (IntPtr classInstance);
		public void LoadImage (int id, TextureImage2d textureImage)
		{
			llge_ContentManager_loadImage(ClassInstance, id, textureImage.ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_ContentManager_loadImage (IntPtr classInstance, int id, IntPtr textureImage);
		public TextureBuffer2d LoadBuffer (int id)
		{
			return new TextureBuffer2d{ ClassInstance = llge_ContentManager_loadBuffer(ClassInstance, id) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_ContentManager_loadBuffer (IntPtr classInstance, int id);
		public void FinishLoad ()
		{
			llge_ContentManager_finishLoad(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_ContentManager_finishLoad (IntPtr classInstance);
		public void Dispose ()
		{
			llge_ContentManager_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_ContentManager_dispose (IntPtr classInstance);
	}
	
	public class llge
	{
		static public Batch2d CreateBatch2d ()
		{
			return new Batch2d{ ClassInstance = createBatch2d() };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr createBatch2d ();
		static public Texture CreateTextureByID (uint id)
		{
			return new Texture{ ClassInstance = createTextureByID(id) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr createTextureByID (uint id);
		static public ContentManager CreateContentManager ()
		{
			return new ContentManager{ ClassInstance = createContentManager() };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr createContentManager ();
		static public EntitiesFactory CreateEntitiesFactory ()
		{
			return new EntitiesFactory{ ClassInstance = createEntitiesFactory() };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr createEntitiesFactory ();
		static public GraphicsFactory CreateGraphicsFactory ()
		{
			return new GraphicsFactory{ ClassInstance = createGraphicsFactory() };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr createGraphicsFactory ();
		static public GeometryFactory CreateGeometryFactory ()
		{
			return new GeometryFactory{ ClassInstance = createGeometryFactory() };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr createGeometryFactory ();
		static public void InitRenderContext ()
		{
			initRenderContext();
		}
		
		[DllImport(Version.Dll)]
		static extern private void initRenderContext ();
	}
	
}

