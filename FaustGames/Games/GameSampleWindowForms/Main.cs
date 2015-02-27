using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using llge;
using OpenGLWindow;

namespace GameSampleWindowForms
{
   
    public partial class Main : Form
    {
        private readonly OGLWindow _oglWindow;

        private readonly GraphicsFactory _graphicsFactory;
        private readonly GraphicsFacade _graphicsFacade;

        private readonly EntitiesFactory _entitiesFactory;
        private readonly EntitiesWorld _entitiesWorld;

        private List<MeshExportData> _data = new List<MeshExportData>();
        private readonly List<string> _images = new List<string>();
        private readonly Dictionary<int, Texture> _textures = new Dictionary<int, Texture>();

        private readonly Dictionary<int, MeshLayer> _layers = new Dictionary<int, MeshLayer>();
        private List<MeshLayer> _layersList = new List<MeshLayer>();
        private List<Entity> Entities;

        private float _x;
        private float _y;
        private float _z;

        private const float Fov = (float)Math.PI *0.5f;
        
        public Main()
        {
           
            InitializeComponent();
            if (components == null)
                components = new Container();
            _oglWindow = new OGLWindow(this, ClientSize.Width, ClientSize.Height);
            components.Add(new DisposableContainerComponent(_oglWindow));

            _entitiesFactory = llge.llge.CreateEntitiesFactory();
            _entitiesWorld = _entitiesFactory.CreateEntitiesWorld();
            components.Add(new DisposeActionContainerComponent(() => _entitiesFactory.Dispose()));
            components.Add(new DisposeActionContainerComponent(() => _entitiesWorld.Dispose()));

            _graphicsFactory = llge.llge.CreateGraphicsFactory();
            _graphicsFacade = _graphicsFactory.CreateGraphicsFacade();
            components.Add(new DisposeActionContainerComponent(() => _graphicsFactory.Dispose()));
            components.Add(new DisposeActionContainerComponent(() => _graphicsFacade.Dispose()));

            _entitiesWorld.GetCamera().SetFov(Fov);
            
            _graphicsFacade.Create();
            using (var reader = new BinaryReader(File.OpenRead(@"d:\mesh_export.data")))
            {
                var count = reader.ReadInt32();
                for (var j = 0; j < count; j++)
                {
                    var mesh = new MeshExportData();
                    _data.Add(mesh);
                    var imageFile = reader.ReadString();
                    var index = _images.IndexOf(imageFile);
                    if (index < 0)
                    {
                        index = _images.Count;
                        _images.Add(imageFile);
                    }
                    mesh.TextureId = index;
                    var verticesCount = reader.ReadInt32();
                    var indicesCount = reader.ReadInt32();
                    mesh.Vertices = new MeshExportVertex[verticesCount];
                    mesh.Indices = new ushort[indicesCount];
                    mesh.Z = float.MinValue;

                    float minX = float.MaxValue;
                    float maxX = float.MinValue;
                    float minY = float.MaxValue;
                    float maxY = float.MinValue;

                    for (var i = 0; i < mesh.Vertices.Length; i++)
                    {
                        var v = new MeshExportVertex();
                        v.X = reader.ReadSingle();
                        v.Y = reader.ReadSingle();
                        v.Z = reader.ReadSingle();
                        v.U = reader.ReadUInt16();
                        v.V = reader.ReadUInt16();
                        v.Color = reader.ReadUInt32();

                        if (mesh.Z < v.Z)
                            mesh.Z = v.Z;

                        if (minX > v.X)
                            minX = v.X;
                        if (minY > v.Y)
                            minY = v.Y;
                        if (maxX < v.X)
                            maxX = v.X;
                        if (maxY < v.Y)
                            maxY = v.Y;

                        //v.Color = 0xffffffff;
                        //v.Z = 0.5f;
                        mesh.Vertices[i] = v;
                    }
                    for (var i = 0; i < mesh.Indices.Length; i++)
                    {
                        mesh.Indices[i] = reader.ReadUInt16();
                    }
                    mesh.X = (minX + maxX)*0.5f;
                    mesh.Y = (minY + maxY)*0.5f;
                    
                    for (var i = 0; i < mesh.Vertices.Length; i++)
                    {
                        mesh.Vertices[i].X -= mesh.X;
                        mesh.Vertices[i].Y -= mesh.Y;
                    }
                    
                    mesh.MinX = minX;
                    mesh.MaxX = maxX;
                    mesh.MinY = minY;
                    mesh.MaxY = maxY;

                    var sizeX = maxX - minX;
                    var sizeY = maxY - minY;

                    var dsizeX = sizeX * (float)Math.Tan(Fov * 0.5f);
                    var dsizeY = sizeY * (float)Math.Tan(Fov * 0.5f);

                    mesh.MinX = minX - dsizeX;
                    mesh.MaxX = maxX + dsizeX;
                    mesh.MinY = minY - dsizeY;
                    mesh.MaxY = maxY + dsizeY;
                    // correntBoundsWithZ;
                }
            }

            for (var i = 0; i < _images.Count; i++)
            {
                var image = _images[i];
                using (var bmp = new Bitmap(Path.Combine("D:\\", image + ".png")))
                {

                    var data = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadOnly,
                        PixelFormat.Format32bppArgb);
                    var texture = _graphicsFacade.CreateTexture();
                    texture.Create();
                    components.Add(new DisposeActionContainerComponent(texture.Dispose));
                    texture.LoadPixels(bmp.Width, bmp.Height, data.Scan0);
                    _textures.Add(i, texture);
                    bmp.UnlockBits(data);
                }
            }

            for (var i = 0; i < _data.Count; i++)
            {
                var mesh = _data[i];
                var meshEntity = _entitiesWorld.CreateMesh2d();
                meshEntity.SetBounds(mesh.MinX, mesh.MinY, mesh.MaxX, mesh.MaxY, -mesh.Z);
                meshEntity.SetMesh(_textures[mesh.TextureId], mesh.Vertices, mesh.Indices);
                meshEntity.SetWorldPosition(mesh.X, mesh.Y, 0);
                meshEntity.AddToWorld();
            }

            _graphicsFacade.Viewport(ClientSize.Width, ClientSize.Height);
            _graphicsFacade.SetClearState(0x7784ff, 1.0f);
        }

        private Stopwatch _stopwatch;
        private float? _downMX;
        private float? _downMY;
        private float? _downX;
        private float? _downY;
        private MouseButtons _prevButtons = MouseButtons.None;
        private int[] _queryResult = new int[16384];

        MeshBatch _batch = new MeshBatch();

        private void DoUpdate()
        {
            var pos = MousePosition;
            var buttons = MouseButtons;
            if ((_prevButtons != MouseButtons.Left) && (buttons == MouseButtons.Left))
            {
                Main_MouseDown(this, new MouseEventArgs(buttons, 0, pos.X, pos.Y, 0));
            }
            Main_MouseMove(this, new MouseEventArgs(buttons, 0, pos.X, pos.Y, 0));
            _prevButtons = buttons;

            const float zoom = 0.001f;

            _entitiesWorld.SetRenderBounds(_x - 1300, _y - 700, _x + 1300, _y + 700);
            _entitiesWorld.GetCamera().SetPosition(-_x, -_y, 800);
            _entitiesWorld.GetCamera().SetAspect((float)Height/Width);
            _entitiesWorld.GetCamera().SetPlanes(0.1f, 50000f);

            Text = _entitiesWorld.Update(0).ToString(CultureInfo.InvariantCulture);
            /*
            _graphicsFacade.SetProjection(new float[]
            {
                zoom, 0, 0, 0,
                0, zoom*Width/Height, 0, 0,
                0, 0, 1, 0,
                -_x * zoom, -_y * zoom*Width/Height, -_z, 1,
            });
            _graphicsFacade.Clear();
            foreach (var data in _data)
            {
                _graphicsFacade.GetUniforms().SetTexture(_textures[data.TextureId]);
                _graphicsFacade.Draw(
                    data.Vertices,
                    data.Indices,
                    data.Indices.Length / 3);
            }
             */ 
            _oglWindow.SwapOpenGLBuffers();
            
        }

        private void ApplicationOnIdle(object sender, EventArgs eventArgs)
        {
            //DoUpdate();
        }

        private void _timer_Tick(object sender, EventArgs e)
        {
            DoUpdate();
        }

        private void Main_FormClosed(object sender, FormClosedEventArgs e)
        {
            _graphicsFacade.Cleanup();
        }

        private void Main_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                _downMX = e.X;
                _downMY = e.Y;
                _downX = _x;
                _downY = _y;
            }
        }

        private void Main_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (
                    (_downMX != null) &&
                    (_downMY != null) &&
                    (_downX != null) &&
                    (_downY != null))
                {
                    var offsetX = e.X - _downMX.Value;
                    var offsetY = e.Y - _downMY.Value;
                    _x = _downX.Value + offsetX;
                    _y = _downY.Value - offsetY;
                }
            }

        }
    }

    internal class DisposeActionContainerComponent : IComponent
    {
        private readonly Action _disposeAction;
        public DisposeActionContainerComponent(Action disposeAction)
        {
            _disposeAction = disposeAction;
        }

        public void Dispose()
        {
            _disposeAction();
        }

        public ISite Site { get; set; }
        public event EventHandler Disposed;
    }

    internal class DisposableContainerComponent : IComponent
    {
        private readonly IDisposable _disposable;
        public bool IsDisposed;
        public DisposableContainerComponent(IDisposable disposable)
        {
            _disposable = disposable;
        }


        public void Dispose()
        {
            IsDisposed = true;
            _disposable.Dispose();
        }

        public ISite Site { get; set; }
        public event EventHandler Disposed;
    }

    public static class VertexBufferExtension
    {
        public unsafe static void SetData(this VertexBuffer buffer, MeshExportVertex[] vertices)
        {
            var pinnedArray = GCHandle.Alloc(vertices, GCHandleType.Pinned);
            var pointer = pinnedArray.AddrOfPinnedObject();
            buffer.SetData(pointer, vertices.Length * sizeof(MeshExportVertex));
            pinnedArray.Free();
        }
    }


    public static class TextureExtension
    {
        public static void LoadPixels(this Texture texture, int width, int height, uint[] pixels)
        {
            var pinnedArray = GCHandle.Alloc(pixels, GCHandleType.Pinned);
            var pointer = pinnedArray.AddrOfPinnedObject();
            texture.LoadPixels(width, height, pointer);
            pinnedArray.Free();
        }
    }

    public static class QuadTreeExtensions
    {
        public static void GetQueryResults(this QuadTree tree, int[] results)
        {
            var pinnedArray = GCHandle.Alloc(results, GCHandleType.Pinned);
            var pointer = pinnedArray.AddrOfPinnedObject();
            tree.GetQueryResults(pointer);
            pinnedArray.Free();
        }
    }


    public static class ProjectionUniformExtension
    {

    }

    public static class EntityExtension
    {
        public unsafe static void SetMesh(this Entity entity, Texture texture, MeshExportVertex[] vertices, ushort[] indices)
        {
            fixed (MeshExportVertex* pointerVertices = vertices)
            fixed (ushort* pointerIndices = indices)
            {
                entity.SetMesh(texture, new IntPtr(pointerVertices), vertices.Length, new IntPtr(pointerIndices), indices.Length);
            }
        }
    }

    public static class GraphicsFacadeExtension
    {
        public static void SetProjection(this GraphicsFacade facade, float[] value)
        {
            var pinnedArray = GCHandle.Alloc(value, GCHandleType.Pinned);
            var pointer = pinnedArray.AddrOfPinnedObject();
            facade.GetUniforms().SetProjection(pointer);
            pinnedArray.Free();
        }

        public unsafe static void DrawVertexBuffer(this GraphicsFacade facade, VertexBuffer vertexBuffer, ushort[] indices,
            int primitivesCount)
        {
            fixed (ushort* pointerIndiceas = indices)
            {
                facade.DrawVertexBuffer(
                    GraphicsEffects.EffectTextureLightmapColor,
                    GraphicsVertexFormats.FormatPositionTextureColor,
                    vertexBuffer,
                    new IntPtr(pointerIndiceas),
                    primitivesCount
                    );
            }
        }

        public unsafe static void Draw(this GraphicsFacade facade, MeshExportVertex[] vertices, ushort[] indices, int primitivesCount)
        {
            fixed (MeshExportVertex* pointerVertices = vertices)
            fixed (ushort* pointerIndiceas = indices)
            {
                facade.Draw(
                    GraphicsEffects.EffectTextureColor,
                    GraphicsVertexFormats.FormatPositionTextureColor,
                    new IntPtr(pointerVertices),
                    new IntPtr(pointerIndiceas),
                    primitivesCount
                    );
            }
            /*
            var pinnedArrayVertices = GCHandle.Alloc(vertices, GCHandleType.Pinned);
            var pointerVertices = pinnedArrayVertices.AddrOfPinnedObject();
            var pinnedArrayIndices = GCHandle.Alloc(indices, GCHandleType.Pinned);
            var pointerIndiceas = pinnedArrayIndices.AddrOfPinnedObject();
            facade.Draw(
                GraphicsEffects.EffectTextureLightmapColor,
                GraphicsVertexFormats.FormatPositionTextureColor,
                pointerVertices,
                pointerIndiceas,
                primitivesCount
                );
            pinnedArrayVertices.Free();
            pinnedArrayIndices.Free();
            */ 
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct MeshExportVertex
    {
        public float X;
        public float Y;
        public float Z;
        public ushort U;
        public ushort V;
        public uint Color;

        public MeshExportVertex(
            float x,
            float y,
            float z,
            ushort u,
            ushort v,
            uint color)
        {
            X = x;
            Y = y;
            Z = z;
            U = u;
            V = v;
            Color = color;
        }
    }

    public class MeshBatch
    {
        public List<MeshExportVertex> Vertices = new List<MeshExportVertex>();
        public List<ushort> Indices = new List<ushort>();
        public Texture Texture;
        public int Calls;

        public void Render(GraphicsFacade facade, MeshExportData data, Texture texture)
        {
            if (Texture != texture)
                Finish(facade);
            Texture = texture;
            var index = Vertices.Count;
            Vertices.AddRange(data.Vertices);
            for (var i = 0; i < data.Indices.Length; i++)
            {
                Indices.Add((ushort)(index + data.Indices[i]));
            }
        }

        public void Finish(GraphicsFacade facade)
        {
            if (Texture != null)
            {
                facade.GetUniforms().SetTexture(Texture);
                facade.Draw(Vertices.ToArray(), Indices.ToArray(), Indices.Count/3);
            }
            Vertices.Clear();
            Indices.Clear();
            Calls++;
        }
    }

    public class MeshBlock
    {
        public int TextureId;
        public MeshExportVertex[] Vertices;
        public ushort[] Indices;

        public List<MeshExportData> Data = new List<MeshExportData>();

        public MeshBlock(int textureId)
        {
            TextureId = textureId;
        }

        public void Build()
        {
            var vertices = new List<MeshExportVertex>();
            var indices = new List<ushort>();
            foreach (var data in Data)
            {
                var baseIndex = vertices.Count;
                vertices.AddRange(data.Vertices);
                foreach (var i in data.Indices)
                {
                    indices.Add((ushort)(i + baseIndex));
                }
            }
            Vertices = vertices.ToArray();
            Indices = indices.ToArray();
        }

        public void BuildInidices()
        {
            var indices = new List<ushort>();
            foreach (var data in Data)
            {
                indices.AddRange(data.Indices);
            }
            Indices = indices.ToArray();
        }
    }

    public class MeshLayer
    {
        public int LayerIndex;
        public List<MeshExportData> Data = new List<MeshExportData>();
        public Dictionary<int, MeshBlock> Blocks = new Dictionary<int, MeshBlock>();

        public MeshLayer(int layerIndex)
        {
            LayerIndex = layerIndex;
        }

        public void Build()
        {
            foreach (var data in Data.OrderByDescending(d => d.Z))
            {
                if (!Blocks.ContainsKey(data.TextureId))
                    Blocks.Add(data.TextureId, new MeshBlock(data.TextureId));
                Blocks[data.TextureId].Data.Add(data);
            }
            foreach (var block in Blocks)
                block.Value.Build();
        }

        public void BuildInidices()
        {
            foreach (var data in Data.OrderByDescending(d => d.Z))
            {
                if (!Blocks.ContainsKey(data.TextureId))
                    Blocks.Add(data.TextureId, new MeshBlock(data.TextureId));
                Blocks[data.TextureId].Data.Add(data);
            }
            foreach (var block in Blocks)
                block.Value.BuildInidices();
        }
    }
    public class MeshExportData
    {
        public float X;
        public float Y;
        public float Z;
        public int TextureId;
        public MeshExportVertex[] Vertices;
        public ushort[] Indices;

        public float MinX;
        public float MinY;
        public float MaxX;
        public float MaxY;
    }

}
