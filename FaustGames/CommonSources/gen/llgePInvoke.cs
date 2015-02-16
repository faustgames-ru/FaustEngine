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
	
	public enum GraphicsEffects
	{
		EffectTextureColor = 0x0,
		EffectTextureLightmapColor = 0x1,
	}
	
	public enum GraphicsVertexFormats
	{
		FormatPositionTextureColor = 0x0,
	}
	
	[StructLayout(LayoutKind.Sequential)]
	public struct Mesh2dVertex
	{
		public float x;
		public float y;
		public float z;
		public ushort u;
		public ushort v;
		public uint color;
	}
	
	public class StaticRenderLayer
	{
		public IntPtr ClassInstance;
	}
	
	public class Image
	{
		public IntPtr ClassInstance;
	}
	
	public class Aabb2dComponent
	{
		public IntPtr ClassInstance;
		public void UpateAabb2d (float minX, float minY, float maxX, float maxY)
		{
			llge_Aabb2dComponent_upateAabb2d(ClassInstance, minX, minY, maxX, maxY);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Aabb2dComponent_upateAabb2d (IntPtr classInstance, float minX, float minY, float maxX, float maxY);
	}
	
	public class Transform2dComponent
	{
		public IntPtr ClassInstance;
		public void UpateTransform (float x, float y, float z, float rotation, float scale)
		{
			llge_Transform2dComponent_upateTransform(ClassInstance, x, y, z, rotation, scale);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Transform2dComponent_upateTransform (IntPtr classInstance, float x, float y, float z, float rotation, float scale);
		public void UpatePivot (float x, float y)
		{
			llge_Transform2dComponent_upatePivot(ClassInstance, x, y);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Transform2dComponent_upatePivot (IntPtr classInstance, float x, float y);
	}
	
	public class RenderLayerComponent
	{
		public IntPtr ClassInstance;
		public void UpateRenderLayer (StaticRenderLayer value)
		{
			llge_RenderLayerComponent_upateRenderLayer(ClassInstance, value.ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_RenderLayerComponent_upateRenderLayer (IntPtr classInstance, IntPtr value);
	}
	
	public class Mesh2dComponent
	{
		public IntPtr ClassInstance;
		public void UpdateData (IntPtr mesh2dVertices, IntPtr ushortIndices)
		{
			llge_Mesh2dComponent_updateData(ClassInstance, mesh2dVertices, ushortIndices);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Mesh2dComponent_updateData (IntPtr classInstance, IntPtr mesh2dVertices, IntPtr ushortIndices);
	}
	
	public class Mesh2dEntity
	{
		public IntPtr ClassInstance;
		public Transform2dComponent GetTransform ()
		{
			return new Transform2dComponent{ ClassInstance = llge_Mesh2dEntity_getTransform(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_Mesh2dEntity_getTransform (IntPtr classInstance);
		public Aabb2dComponent GetAabbComponent ()
		{
			return new Aabb2dComponent{ ClassInstance = llge_Mesh2dEntity_getAabbComponent(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_Mesh2dEntity_getAabbComponent (IntPtr classInstance);
		public Mesh2dComponent GetMesh ()
		{
			return new Mesh2dComponent{ ClassInstance = llge_Mesh2dEntity_getMesh(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_Mesh2dEntity_getMesh (IntPtr classInstance);
	}
	
	public class EntitiesFactory
	{
		public IntPtr ClassInstance;
		public Mesh2dEntity CreateMeshEntity (int verticesCount, int indicesCount)
		{
			return new Mesh2dEntity{ ClassInstance = llge_EntitiesFactory_createMeshEntity(ClassInstance, verticesCount, indicesCount) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_EntitiesFactory_createMeshEntity (IntPtr classInstance, int verticesCount, int indicesCount);
		public void DisposeMeshEntities ()
		{
			llge_EntitiesFactory_disposeMeshEntities(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_EntitiesFactory_disposeMeshEntities (IntPtr classInstance);
		public void Dispose ()
		{
			llge_EntitiesFactory_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_EntitiesFactory_dispose (IntPtr classInstance);
	}
	
	public class RenderSystem
	{
		public IntPtr ClassInstance;
		public StaticRenderLayer CreateStaticLayer (float z)
		{
			return new StaticRenderLayer{ ClassInstance = llge_RenderSystem_createStaticLayer(ClassInstance, z) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_RenderSystem_createStaticLayer (IntPtr classInstance, float z);
		public Image CreateImage ()
		{
			return new Image{ ClassInstance = llge_RenderSystem_createImage(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_RenderSystem_createImage (IntPtr classInstance);
	}
	
	public class TestRenderSystem
	{
		public IntPtr ClassInstance;
		public void Viewport (int width, int height)
		{
			llge_TestRenderSystem_viewport(ClassInstance, width, height);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_TestRenderSystem_viewport (IntPtr classInstance, int width, int height);
		public void Create ()
		{
			llge_TestRenderSystem_create(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_TestRenderSystem_create (IntPtr classInstance);
		public void Render ()
		{
			llge_TestRenderSystem_render(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_TestRenderSystem_render (IntPtr classInstance);
		public void Cleanup ()
		{
			llge_TestRenderSystem_cleanup(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_TestRenderSystem_cleanup (IntPtr classInstance);
		public void Dispose ()
		{
			llge_TestRenderSystem_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_TestRenderSystem_dispose (IntPtr classInstance);
	}
	
	public class Factory
	{
		public IntPtr ClassInstance;
		public TestRenderSystem CreateRenderSystem ()
		{
			return new TestRenderSystem{ ClassInstance = llge_Factory_createRenderSystem(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_Factory_createRenderSystem (IntPtr classInstance);
		public EntitiesFactory CreateEntitiesFactory ()
		{
			return new EntitiesFactory{ ClassInstance = llge_Factory_createEntitiesFactory(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_Factory_createEntitiesFactory (IntPtr classInstance);
		public void Dispose ()
		{
			llge_Factory_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Factory_dispose (IntPtr classInstance);
	}
	
	public class Texture
	{
		public IntPtr ClassInstance;
		public int GetId ()
		{
			return llge_Texture_getId(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private int llge_Texture_getId (IntPtr classInstance);
		public void LoadPixels (int width, int height, IntPtr pixels)
		{
			llge_Texture_LoadPixels(ClassInstance, width, height, pixels);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Texture_LoadPixels (IntPtr classInstance, int width, int height, IntPtr pixels);
		public void Create ()
		{
			llge_Texture_create(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Texture_create (IntPtr classInstance);
		public void Cleanup ()
		{
			llge_Texture_cleanup(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Texture_cleanup (IntPtr classInstance);
		public void Dispose ()
		{
			llge_Texture_dispose(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_Texture_dispose (IntPtr classInstance);
	}
	
	public class UniformsFacade
	{
		public IntPtr ClassInstance;
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
		public Texture CreateTexture ()
		{
			return new Texture{ ClassInstance = llge_GraphicsFacade_createTexture(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_GraphicsFacade_createTexture (IntPtr classInstance);
		public VertexBuffer CreateVertexBuffer ()
		{
			return new VertexBuffer{ ClassInstance = llge_GraphicsFacade_createVertexBuffer(ClassInstance) };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr llge_GraphicsFacade_createVertexBuffer (IntPtr classInstance);
		public void Viewport (int width, int height)
		{
			llge_GraphicsFacade_viewport(ClassInstance, width, height);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_viewport (IntPtr classInstance, int width, int height);
		public void SetClearState (uint color, float depth)
		{
			llge_GraphicsFacade_setClearState(ClassInstance, color, depth);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_setClearState (IntPtr classInstance, uint color, float depth);
		public void Clear ()
		{
			llge_GraphicsFacade_clear(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_clear (IntPtr classInstance);
		public void Draw (GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IntPtr vertices, IntPtr indices, int primitivesCount)
		{
			llge_GraphicsFacade_draw(ClassInstance, effect, vertexFormat, vertices, indices, primitivesCount);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_draw (IntPtr classInstance, GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IntPtr vertices, IntPtr indices, int primitivesCount);
		public void DrawVertexBuffer (GraphicsEffects effect, GraphicsVertexFormats vertexFormat, VertexBuffer vertexBuffer, IntPtr indices, int primitivesCount)
		{
			llge_GraphicsFacade_drawVertexBuffer(ClassInstance, effect, vertexFormat, vertexBuffer.ClassInstance, indices, primitivesCount);
		}
		
		[DllImport(Version.Dll)]
		static extern private void llge_GraphicsFacade_drawVertexBuffer (IntPtr classInstance, GraphicsEffects effect, GraphicsVertexFormats vertexFormat, IntPtr vertexBuffer, IntPtr indices, int primitivesCount);
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
	
	public class llge
	{
		static public Factory CreateFactory ()
		{
			return new Factory{ ClassInstance = createFactory() };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr createFactory ();
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
	}
	
}

