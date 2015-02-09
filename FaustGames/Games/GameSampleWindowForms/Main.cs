using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using llge;
using OpenGLWindow;

namespace GameSampleWindowForms
{
    [StructLayout(LayoutKind.Sequential)]
    public struct MeshExportVertex
    {
        public float X;
        public float Y;
        public float Z;
        public ushort U;
        public ushort V;
        public uint Color;
    }

    public class MeshExportData
    {
        public MeshExportVertex[] Vertices;
        public ushort[] Indices;
    }

    public partial class Main : Form
    {
        private readonly OGLWindow _oglWindow;
        private readonly GraphicsFactory _factory;
        private readonly GraphicsFacade _facade;
        private readonly Texture _texture;
        private List<MeshExportData> _data = new List<MeshExportData>();
        public Main()
        {

            using (var reader = new BinaryReader(File.OpenRead(@"d:\mesh_export.data")))
            {
                var count = reader.ReadInt32();
                for (var j = 0; j < count; j++)
                {
                    var mesh = new MeshExportData();
                    _data.Add(mesh);
                    var verticesCount = reader.ReadInt32();
                    var indicesCount = reader.ReadInt32();
                    mesh.Vertices = new MeshExportVertex[verticesCount];
                    mesh.Indices = new ushort[indicesCount];
                    for (var i = 0; i < mesh.Vertices.Length; i++)
                    {
                        var v = new MeshExportVertex();
                        v.X = reader.ReadSingle();
                        v.Y = reader.ReadSingle();
                        v.Z = reader.ReadSingle();
                        v.U = reader.ReadUInt16();
                        v.V = reader.ReadUInt16();
                        v.Color = reader.ReadUInt32();
                        mesh.Vertices[i] = v;
                    }
                    for (var i = 0; i < mesh.Indices.Length; i++)
                    {
                        mesh.Indices[i] = reader.ReadUInt16();
                    }
                }
            }

            InitializeComponent();
            if (components == null)
                components = new Container();
            _oglWindow = new OGLWindow(this, ClientSize.Width, ClientSize.Height);
            components.Add(new DisposableContainerComponent(_oglWindow));

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
                0xffffffff,
                0xffffffff,
                0xffffffff,
            });
            
            _facade.Viewport(ClientSize.Width, ClientSize.Height);
            _facade.SetClearState(0x7784ff, 1.0f);
        }

        private Stopwatch _stopwatch;

        private void DoUpdate()
        {
            _facade.Clear();
            _facade.GetUniforms().GetTextureUniform().SetTexture(_texture);
            
            _facade.GetUniforms().GetProjectionUniformm().SetProjection(new float[]
            {
                0.01f, 0, 0, 0,
                0, 0.01f, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1,
            });

            foreach (var data in _data)
            {
                _facade.Draw(
                    data.Vertices,
                    data.Indices,
                    data.Indices.Length / 3);
            }
            _oglWindow.SwapOpenGLBuffers();
        }

        private void ApplicationOnIdle(object sender, EventArgs eventArgs)
        {
            DoUpdate();
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

    public static class ProjectionUniformExtension
    {
        public static void SetProjection(this ProjectionUniform uniform, float[] value)
        {
            var pinnedArray = GCHandle.Alloc(value, GCHandleType.Pinned);
            var pointer = pinnedArray.AddrOfPinnedObject();
            uniform.SetProjection(pointer);
            pinnedArray.Free();
        }
    }

    public static class GraphicsFacadeExtension
    {
        public static void Draw(this GraphicsFacade facade, MeshExportVertex[] vertices, ushort[] indices, int primitivesCount)
        {
            var pinnedArrayVertices = GCHandle.Alloc(vertices, GCHandleType.Pinned);
            var pointerVertices = pinnedArrayVertices.AddrOfPinnedObject();
            var pinnedArrayIndices = GCHandle.Alloc(indices, GCHandleType.Pinned);
            var pointerIndiceas = pinnedArrayIndices.AddrOfPinnedObject();
            facade.Draw(
                facade.GetEffectsFacade().GetTextureColorEffect(),
                facade.GetVertexFormatsFacade().GetPositionTextureColorFormat(),
                pointerVertices,
                pointerIndiceas,
                primitivesCount
                );
            pinnedArrayVertices.Free();
            pinnedArrayIndices.Free();
        }
    }
}
