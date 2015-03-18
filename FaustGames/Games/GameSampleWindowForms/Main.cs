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
using GameSampleWindowForms.Properties;
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
        private readonly Dictionary<int, TextureImage2d> _textures = new Dictionary<int, TextureImage2d>();

        private readonly Dictionary<int, MeshLayer> _layers = new Dictionary<int, MeshLayer>();
        private List<MeshLayer> _layersList = new List<MeshLayer>();
        private List<Entity> Entities;
        private TextureImage2d _envMap;
        private TextureImage2d _whiteTexture;
        private TextureImage2d _env;
        private TextureImage2d _nrm;

        private WaterVertex[] _waterPlane;
        private ushort[] _waterIndices;

        private float _x = 0;
        private float _y = 500;
        private float _z = 0;

        private const float Fov = (float)Math.PI *0.5f;
        private const float waterMinX = -10000;
        private const float waterMaxX = 10000;
        private const float waterMinZ = -1000;
        private const float waterMaxZ = 10000;
        private const float waterStepX = 500;
        private const float waterStepZ = 500;
        public Main()
        {
           
            InitializeComponent();
            if (components == null)
                components = new Container();
            _oglWindow = new OGLWindow(_renderRegion, _renderRegion.ClientSize.Width, _renderRegion.ClientSize.Height);
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
            using (var reader = new BinaryReader(File.OpenRead(ContetnFolder + @"\mesh_export.data")))
            {
                var count = reader.ReadInt32();
                for (var j = 0; j < count; j++)
                {
                    var mesh = new MeshExportData();
                    var imageFile = reader.ReadString();
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

                    //if (mesh.Z > 500)
                    {
                        var index = _images.IndexOf(imageFile);
                        if (index < 0)
                        {
                            index = _images.Count;
                            _images.Add(imageFile);
                        }
                        mesh.TextureId = index;
                        _data.Add(mesh);
                    }

                    // correntBoundsWithZ;
                }
            }

            for (var i = 0; i < _images.Count; i++)
            {
                var image = _images[i];
                var texture = CreateTexture(image);
                _textures.Add(i, texture);
                /*
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
                 */ 
            }
            _envMap = CreateTexture("skybox_fragment");
            _whiteTexture = CreateTexture("white_texture");
            if (Settings.Default.useEnv)
            {
                _env = _envMap;
            }
            else
            {
                _env = _whiteTexture;
            }

            _nrm = CreateTexture("water_nrm");
            var waterGrid = new List<WaterVertex>();
            var waterIndices = new List<ushort>();
            for (var x = waterMinX + waterStepX; x <= waterMaxX; x += waterStepX)
            {
                var px = x - waterStepX;
                for (var z = waterMinZ + waterStepX; z <= waterMaxZ; z += waterStepZ)
                {
                    var pz = z - waterStepZ;
                    var i = waterGrid.Count;
                    waterGrid.Add(new WaterVertex(px, -1000, pz, 0, 0));
                    waterGrid.Add(new WaterVertex(x, -1000, pz, ushort.MaxValue, 0));
                    waterGrid.Add(new WaterVertex(x, -1000, z, ushort.MaxValue, ushort.MaxValue));
                    waterGrid.Add(new WaterVertex(px, -1000, z, 0, ushort.MaxValue));
                    
                    waterIndices.AddRange(new[]
                    {
                        (ushort)(i+0), (ushort)(i+1), (ushort)(i+2),
                        (ushort)(i+0), (ushort)(i+2), (ushort)(i+3),
                    });
                }
            }

            _waterPlane = waterGrid.ToArray();
            _waterIndices = waterIndices.ToArray();

            for (var i = 0; i < _data.Count; i++)
            {
                var mesh = _data[i];
                var meshEntity = _entitiesWorld.CreateEntity();
                meshEntity.SetComponents(ComponentsTypes.Aadd2d | ComponentsTypes.Render2d | ComponentsTypes.Transform2d);
                meshEntity.GetAabb2d().Update(mesh.MinX, mesh.MinY, mesh.MaxX, mesh.MaxY, -mesh.Z);
                meshEntity.SetMesh(_textures[mesh.TextureId].GetTexture(), mesh.Vertices, mesh.Indices);
                meshEntity.GetTransform2d().SetWorldPosition(mesh.X, mesh.Y, 0);
                
                _entitiesWorld.AddEntity(meshEntity);
                _entitiesWorld.UpdateEntity(meshEntity, ComponentsTypes.Transform2d);
            }

            _graphicsFacade.Viewport(_renderRegion.ClientSize.Width, _renderRegion.ClientSize.Height);
            _graphicsFacade.SetClearState(0x7784ff, 1.0f);
            
            _textureVelocity.Value = (int) (Settings.Default.textureVelocity*100);
            _wavesVelocity.Value = (int) (Settings.Default.wavesVelocity*100);
            _normalScaleZ.Value = (int) (Settings.Default.normalScaleZ*100);
            _wavesScale.Value = (int) Settings.Default.wavesScale;
            _envOffsetY.Value = (int)(Settings.Default.envOffsetY*100);
            _envScaleY.Value = (int)(Settings.Default.envScaleY*100);
            _reflectionBrightness.Value = (int)(Settings.Default.reflectionBrightness * 100);
            _reflectionContrast.Value = (int)(Settings.Default.reflectionContrast * 100);
            _reflectionSaturation.Value = (int)(Settings.Default.reflectionSaturation * 100);
            _reflectionTint0.BackColor = Settings.Default.reflectionTint0;
            _reflectionTint1.BackColor = Settings.Default.reflectionTint1;
            _useEnv.Checked = Settings.Default.useEnv;

            _x = Settings.Default.cameraPositionX;
            _y = Settings.Default.cameraPositionY;

            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "textureVelocity", Settings.Default.textureVelocity);
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "wavesVelocity", Settings.Default.wavesVelocity);
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "wavesScale", Settings.Default.wavesScale);
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "normalScaleZ", Settings.Default.normalScaleZ);
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "envOffsetY", Settings.Default.envOffsetY);
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "envScaleY", Settings.Default.envScaleY);
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "reflectionBrightness", Settings.Default.reflectionBrightness);
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "reflectionContrast", Settings.Default.reflectionContrast);
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "reflectionSaturation", Settings.Default.reflectionSaturation);
            _graphicsFacade.SetEffectConstantColor(GraphicsEffects.EffectWater, "reflectionTint0", (uint)(Settings.Default.reflectionTint0.ToArgb()));
            _graphicsFacade.SetEffectConstantColor(GraphicsEffects.EffectWater, "reflectionTint1", (uint)(Settings.Default.reflectionTint1.ToArgb()));
        }


        private Stopwatch _stopwatch;
        private float? _downMX;
        private float? _downMY;
        private float? _downX;
        private float? _downY;
        private MouseButtons _prevButtons = MouseButtons.None;
        private int[] _queryResult = new int[16384];

        MeshBatch _batch = new MeshBatch();
        private const string ContetnFolder = "Content";
        private unsafe TextureImage2d CreateTexture(string fileName)
        {
            using (var bmp = new Bitmap(Path.Combine(ContetnFolder, fileName + ".png")))
            {
                var data = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadOnly,
                    PixelFormat.Format32bppArgb);
                var texture = _graphicsFacade.CreateTextureImage2d();
                texture.Create();
                components.Add(new DisposeActionContainerComponent(texture.Dispose));
                var pixels = new uint[bmp.Width*bmp.Height];
                var pixelsAdr = (uint*)data.Scan0.ToPointer();
                for (var i = 0; i < pixels.Length; i++)
                {
                    var a = ((*pixelsAdr) >> 24) & 0xff;
                    var r = ((*pixelsAdr) >> 16) & 0xff;
                    var g = ((*pixelsAdr) >> 8) & 0xff;
                    var b = ((*pixelsAdr) >> 0) & 0xff;
                    pixels[i] = (a << 24) + (b << 16) + (g << 8) + r;
                    pixelsAdr++;
                }
                fixed (uint* ptr = pixels)
                {
                    texture.LoadPixels(bmp.Width, bmp.Height, new IntPtr(ptr));
                }
                bmp.UnlockBits(data);
                return texture;
            }
        }

        private float _time = 0;

        private void DoUpdate()
        {
            if (_stopwatch == null)
            {
                _stopwatch = Stopwatch.StartNew();
            }
            else
            {
                _stopwatch.Stop();
                _time += (float)_stopwatch.Elapsed.TotalSeconds;
                _stopwatch = Stopwatch.StartNew();
            }


            var pos = MousePosition;
            var contains = _renderRegion.ClientRectangle.Contains(_renderRegion.PointToClient(pos));
            var buttons = MouseButtons;
            if ((_prevButtons != MouseButtons.Left) && (buttons == MouseButtons.Left))
            {
                if (contains)
                    Main_MouseDown(this, new MouseEventArgs(buttons, 0, pos.X, pos.Y, 0));
            }

            if (contains)
                Main_MouseMove(this, new MouseEventArgs(buttons, 0, pos.X, pos.Y, 0));
            _prevButtons = buttons;

            const float zoom = 0.001f;
            
            _entitiesWorld.SetRenderBounds(_x - 2300, _y - 1700, _x + 2300, _y + 1700);
            _entitiesWorld.GetCamera().SetPosition(_x, _y, -1800);
            _entitiesWorld.GetCamera().SetAspect((float)_renderRegion.Height / _renderRegion.Width);
            _entitiesWorld.GetCamera().SetPlanes(0.1f, 50000f);
           
            _graphicsFacade.GetUniforms().SetEnvironment(_env.GetTexture());
            _graphicsFacade.GetUniforms().SetNormalmap(_nrm.GetTexture());
            _graphicsFacade.GetUniforms().SetTime(_time);
            
            _graphicsFacade.Clear();

            _graphicsFacade.Draw(
                _waterPlane,
                _waterIndices,
                _waterIndices.Length / 3);
            Text = _entitiesWorld.Update(0).ToString(CultureInfo.InvariantCulture);
            /*
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
                    Settings.Default.cameraPositionX = _x;
                    Settings.Default.cameraPositionY = _y;
                    Settings.Default.Save();
                }
            }

        }

        private void _wavesScale_ValueChanged(object sender, EventArgs e)
        {
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "wavesScale",
                Settings.Default.wavesScale = _wavesScale.Value);
            Settings.Default.Save();
        }

        private void _textureVelocity_ValueChanged(object sender, EventArgs e)
        {
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "textureVelocity", 
                Settings.Default.textureVelocity = (float)_textureVelocity.Value / 100);
            Settings.Default.Save();
        }

        private void _wavesVelocity_ValueChanged(object sender, EventArgs e)
        {
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "wavesVelocity",
                Settings.Default.wavesVelocity = (float)_wavesVelocity.Value / 100);
            Settings.Default.Save();
        }

        private void _normalScaleZ_ValueChanged(object sender, EventArgs e)
        {
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "normalScaleZ",
                Settings.Default.normalScaleZ = (float)_normalScaleZ.Value / 100);
            Settings.Default.Save();
        }

        private void _envOffsetY_ValueChanged(object sender, EventArgs e)
        {
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "envOffsetY",
                Settings.Default.envOffsetY = (float)_envOffsetY.Value / 100);
            Settings.Default.Save();
        }

        private void _envScaleY_ValueChanged(object sender, EventArgs e)
        {
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "envScaleY",
                Settings.Default.envScaleY = (float) _envScaleY.Value/100);
            Settings.Default.Save();
        }

        private void _reflectionSaturation_ValueChanged(object sender, EventArgs e)
        {
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "reflectionSaturation",
                Settings.Default.reflectionSaturation = (float)_reflectionSaturation.Value / 100);
            Settings.Default.Save();
        }

        private void _reflectionBrightness_ValueChanged(object sender, EventArgs e)
        {
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "reflectionBrightness",
                Settings.Default.reflectionBrightness = (float)_reflectionBrightness.Value / 100);
            Settings.Default.Save();

        }

        private void _reflectionContrast_ValueChanged(object sender, EventArgs e)
        {
            _graphicsFacade.SetEffectConstantFloat(GraphicsEffects.EffectWater, "reflectionContrast",
                Settings.Default.reflectionContrast = (float)_reflectionContrast.Value / 100);
            Settings.Default.Save();
        }

        private void _reflectionTint0_Click(object sender, EventArgs e)
        {
            _colorDialog.Color = _reflectionTint0.BackColor;
            if (_colorDialog.ShowDialog(this) != DialogResult.OK) return;
            _graphicsFacade.SetEffectConstantColor(GraphicsEffects.EffectWater, "reflectionTint0",
                (uint)_colorDialog.Color.ToArgb());
            _reflectionTint0.BackColor = Settings.Default.reflectionTint0 = _colorDialog.Color;
            Settings.Default.Save();
        }

        private void _reflectionTint1_Click(object sender, EventArgs e)
        {
            _colorDialog.Color = _reflectionTint0.BackColor;
            if (_colorDialog.ShowDialog(this) != DialogResult.OK) return;
            _graphicsFacade.SetEffectConstantColor(GraphicsEffects.EffectWater, "reflectionTint1",
                (uint)_colorDialog.Color.ToArgb());
            _reflectionTint1.BackColor = Settings.Default.reflectionTint1 = _colorDialog.Color;
            Settings.Default.Save();
        }

        private void _useEnv_CheckedChanged(object sender, EventArgs e)
        {
            Settings.Default.useEnv = _useEnv.Checked;

            if (Settings.Default.useEnv)
            {
                _env = _envMap;
            }
            else
            {
                _env = _whiteTexture;
            }
            Settings.Default.Save();
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
        public unsafe static void SetData(this VertexBuffer buffer, WaterVertex[] vertices)
        {
            var pinnedArray = GCHandle.Alloc(vertices, GCHandleType.Pinned);
            var pointer = pinnedArray.AddrOfPinnedObject();
            buffer.SetData(pointer, vertices.Length * sizeof(MeshExportVertex));
            pinnedArray.Free();
        }
        
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
        public static void LoadPixels(this TextureImage2d texture, int width, int height, uint[] pixels)
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
                entity.GetRender2d().SetMesh(texture, new IntPtr(pointerVertices), vertices.Length, new IntPtr(pointerIndices), indices.Length);
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

        public static unsafe void Draw(this GraphicsFacade facade, WaterVertex[] vertices, ushort[] indices,
            int primitivesCount)
        {
            fixed (WaterVertex* pointerVertices = vertices)
            fixed (ushort* pointerIndiceas = indices)
            {
                facade.Draw(
                    GraphicsEffects.EffectWater,
                    GraphicsVertexFormats.FormatPositionTexture,
                    new IntPtr(pointerVertices),
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
    public struct WaterVertex
    {
        public float X;
        public float Y;
        public float Z;
        public ushort U;
        public ushort V;

        public WaterVertex(float x, float y, float z, ushort u, ushort v)
        {
            X = x;
            Y = y;
            Z = z;
            U = u;
            V = v;
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
