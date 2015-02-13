using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using llge;
using OpenGLWindow;

namespace GameSampleWindowForms
{
   
    public partial class Main : Form
    {
        private readonly OGLWindow _oglWindow;
        private readonly GraphicsFactory _factory;
        private readonly GeometryFactory _geometryFactory;
        private readonly QuadTree _quadTree;
        private readonly GraphicsFacade _facade;
        private readonly Texture _texture;
        private List<MeshExportData> _data = new List<MeshExportData>();
        private readonly List<string> _images = new List<string>();
        private readonly Dictionary<int, Texture> _textures = new Dictionary<int, Texture>();

        private float _x;
        private float _y;
        private float _z;
        
        public Main()
        {
           
            InitializeComponent();
            if (components == null)
                components = new Container();
            _oglWindow = new OGLWindow(this, ClientSize.Width, ClientSize.Height);
            components.Add(new DisposableContainerComponent(_oglWindow));

            _geometryFactory = llge.llge.CreateGeometryFactory();
            _quadTree = _geometryFactory.CreateQuadTree();
            components.Add(new DisposeActionContainerComponent(() => _geometryFactory.Dispose()));
            components.Add(new DisposeActionContainerComponent(() => _quadTree.Dispose()));

            _factory = llge.llge.CreateGraphicsFactory();
            _facade = _factory.CreateGraphicsFacade();
            components.Add(new DisposeActionContainerComponent(() => _facade.Dispose()));
            components.Add(new DisposeActionContainerComponent(() => _factory.Dispose()));
            Application.Idle += ApplicationOnIdle;

            _texture = _facade.CreateTexture();
            components.Add(new DisposeActionContainerComponent(() => _texture.Dispose()));

            _facade.Create();
            _texture.Create();
            
            _texture.LoadPixels(2, 2, new uint[]
            {
                0xffffffff,
                0xff0000ff,
                0xff0000ff,
                0xffffffff,
            });

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
                        v.Z = 0.5f;
                        mesh.Vertices[i] = v;
                    }
                    for (var i = 0; i < mesh.Indices.Length; i++)
                    {
                        mesh.Indices[i] = reader.ReadUInt16();
                    }
                    _quadTree.Insert(minX, minY, maxX, maxY, j);
                }
            }

            for (var i = 0; i < _images.Count; i++)
            {
                var image = _images[i];
                using (var bmp = new Bitmap(Path.Combine("D:\\", image + ".png")))
                {

                    var data = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadOnly,
                        PixelFormat.Format32bppArgb);
                    var texture = _facade.CreateTexture();
                    texture.Create();
                    components.Add(new DisposeActionContainerComponent(texture.Dispose));
                    texture.LoadPixels(bmp.Width, bmp.Height, data.Scan0);
                    _textures.Add(i, texture);
                    bmp.UnlockBits(data);
                }
            }

            _facade.Viewport(ClientSize.Width, ClientSize.Height);
            _facade.SetClearState(0x7784ff, 1.0f);
        }

        private Stopwatch _stopwatch;
        private float? _downMX;
        private float? _downMY;
        private float? _downX;
        private float? _downY;
        private MouseButtons _prevButtons = MouseButtons.None;
        private int[] _queryResult = new int[16384];

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

            _facade.Clear();
            const float zoom = 0.001f;
            _facade.SetProjection(new float[]
            {
                zoom, 0, 0, 0,
                0, zoom*Width/Height, 0, 0,
                0, 0, 1, 0,
                -_x * zoom, -_y * zoom*Width/Height, -_z, 1,
            });
            /*
            _facade.Draw(
                new []
                {
                    new MeshExportVertex(-1, -1, 0.5f, 0, 0, 0xffffffff),
                    new MeshExportVertex(-1, 1, 0.5f, 0, 0, 0xffffffff),
                    new MeshExportVertex(-0.25f, 1, 0.5f, 0, 0, 0xffffffff),
                    new MeshExportVertex(-0.25f, -1, 0.5f, 0, 0, 0xffffffff),
                }, 
                new ushort[]
                {
                    0, 2, 1,
                    0, 3, 2,
                }, 
                2);

            _facade.Draw(
                new[]
                {
                    new MeshExportVertex(0.25f, -1, 0.5f, 0, 0, 0xffffffff),
                    new MeshExportVertex(0.25f, 1, 0.5f, 0, 0, 0xffffffff),
                    new MeshExportVertex(1, 1, 0.5f, 0, 0, 0xffffffff),
                    new MeshExportVertex(1, -1, 0.5f, 0, 0, 0xffffffff),
                },
                new ushort[]
                {
                    0, 1, 2,
                    0, 2, 3,
                },
                2);
            */
            _quadTree.Query(_x - 1000, _y - 1000, _x + 1000, _y + 1000);

            Text = _quadTree.GetIterationsCount().ToString();

            var count = _quadTree.GetQueryResultsCount();
            _quadTree.GetQueryResults(_queryResult);

            var drawList = new List<MeshExportData>();
            for (int i = 0; i < count; i++)
            {
                var id = _queryResult[i];
                drawList.Add(_data[id]);
            }

            drawList = drawList.OrderByDescending(d => d.Z).ToList();

            _facade.GetUniforms().SetLightMap(_texture);
            foreach (var data in drawList)
            {
                _facade.GetUniforms().SetTexture(_textures[data.TextureId]);

                _facade.Draw(
                    data.Vertices,
                    data.Indices,
                    data.Indices.Length / 3);
            }
            /*
            foreach (var data in _data)
            {
                _facade.GetUniforms().GetTextureUniform().SetTexture(_textures[data.TextureId]);

                _facade.Draw(
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
            _facade.Cleanup();
            _texture.Cleanup();
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

    public static class GraphicsFacadeExtension
    {
        public static void SetProjection(this GraphicsFacade facade, float[] value)
        {
            var pinnedArray = GCHandle.Alloc(value, GCHandleType.Pinned);
            var pointer = pinnedArray.AddrOfPinnedObject();
            facade.GetUniforms().SetProjection(pointer);
            pinnedArray.Free();
        }
        public static void Draw(this GraphicsFacade facade, MeshExportVertex[] vertices, ushort[] indices, int primitivesCount)
        {
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

    public class MeshExportData
    {
        public float Z;
        public int TextureId;
        public MeshExportVertex[] Vertices;
        public ushort[] Indices;
    }

}
