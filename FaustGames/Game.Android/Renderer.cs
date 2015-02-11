using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

using Android.App;
using Android.Content;
using Android.Graphics;
using Android.OS;
using Android.Runtime;
using Android.Util;
using Android.Views;
using Android.Widget;
using llge;
using OpenTK.Input;
using Environment = Android.OS.Environment;

namespace Game.Android
{
    public class Renderer
    {
        private GraphicsFactory _factory;
        private GraphicsFacade _graphics;
        private readonly GeometryFactory _geometryFactory;
        private readonly QuadTree _quadTree;
        private List<MeshExportData> _data = new List<MeshExportData>();
        private readonly List<string> _images = new List<string>();
        private readonly Dictionary<int, Texture> _textures = new Dictionary<int, Texture>();
        private readonly Dictionary<int, MeshLayer> _layers = new Dictionary<int, MeshLayer>();
        private List<MeshLayer> _layersList = new List<MeshLayer>();
        private string _dataPath;
        private readonly Texture _texture;
        public Renderer()
        {
            _factory = llge.llge.CreateGraphicsFactory();
            _graphics = _factory.CreateGraphicsFacade();
            _geometryFactory = llge.llge.CreateGeometryFactory();
            _quadTree = _geometryFactory.CreateQuadTree();

            var path = Environment.ExternalStorageDirectory.AbsolutePath;
            var dataPath = _dataPath = System.IO.Path.Combine(path, "test_data");
            
            if (!Directory.Exists(dataPath)) return;
            using (var reader = new BinaryReader(File.OpenRead(System.IO.Path.Combine(dataPath, "mesh_export.data"))))
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

                        if (minX > v.X)
                            minX = v.X;
                        if (minY > v.Y)
                            minY = v.Y;
                        if (maxX < v.X)
                            maxX = v.X;
                        if (maxY < v.Y)
                            maxY = v.Y;


                        if (mesh.Z < v.Z)
                            mesh.Z = v.Z;

                        //v.Color = 0xffffffff;
                        //v.Z = 0.5f;
                        mesh.Vertices[i] = v;
                    }
                    for (var i = 0; i < mesh.Indices.Length; i++)
                    {
                        mesh.Indices[i] = reader.ReadUInt16();
                    }
                    var layerIndex = (int) (mesh.Z*1000);
                    if (!_layers.ContainsKey(layerIndex))
                        _layers.Add(layerIndex, new MeshLayer(layerIndex));
                    _layers[layerIndex].Data.Add(mesh);
                    _quadTree.Insert(minX, minY, maxX, maxY, j);
                }
            }
            _layersList = _layers.Values.OrderByDescending(l => l.LayerIndex).ToList();
            foreach (var layer in _layersList)
            {
                layer.Build();
            }

            _texture = _graphics.CreateTexture();
            for (var i = 0; i < _images.Count; i++)
            {
                var texture = _graphics.CreateTexture();
                _textures.Add(i, texture);
            }

        }

        private int _w;
        private int _h;
        private int[] _queryResult = new int[16384];
        public void Create()
        {
            _graphics.Create();
            _graphics.SetClearState(0x7784ff, 1.0f);

            _texture.Create();
            _texture.LoadPixels(2, 2, new uint[]
            {
                0xffffffff,
                0xff0000ff,
                0xff0000ff,
                0xffffffff,
            });

            if (!Directory.Exists(_dataPath)) return;

            foreach (var texture in _textures)
            {
                var image = _images[texture.Key];
                using (
                    var bmp = BitmapFactory.DecodeFile(System.IO.Path.Combine(_dataPath, image + ".png"),
                        new BitmapFactory.Options {InScaled = false, InPreferQualityOverSpeed = true}))
                {
                    var width = bmp.Width;
                    var height = bmp.Height;

                    var pixels = new int[width*height];
                    if ((width != bmp.Width) || (height != bmp.Height))
                    {
                        using (var imagePadded = Bitmap.CreateBitmap(width, height, Bitmap.Config.Argb8888))
                        {
                            var canvas = new Canvas(imagePadded);
                            canvas.DrawARGB(0, 0, 0, 0);
                            canvas.DrawBitmap(bmp, 0, 0, null);
                            imagePadded.GetPixels(pixels, 0, width, 0, 0, width, height);
                            imagePadded.Recycle();
                        }
                    }
                    else
                    {
                        bmp.GetPixels(pixels, 0, width, 0, 0, width, height);
                    }
                    bmp.Recycle();
                    //bool empty = true;
                    //unchecked
                    //{
                    //    for (int i = 0; i < pixels.Length; i++)
                    //    {
                    //        var pixel = pixels[i];
                    //        if (pixel != 0)
                    //        {
                    //            pixels[i] = (int)0xff0000ff;
                    //            empty = false;
                    //        }
                    //        else
                    //        {
                    //            pixels[i] = (int)0xff00ffff;
                    //        }
                    //    }
                    //}
                    texture.Value.Create();
                    texture.Value.LoadPixels(width, height, pixels);
                }
            }
        }

        private float _x = 0;
        private float _y = 0;
        private float _z = 0;

        private long _time;
        public static long DTime ;
        public void Render(int w, int h)
        {
            var time = System.Environment.TickCount;
            var dTime = DTime = time - _time;
            _time = time;

            _w = w;
            _h = h;

            //var watch = Stopwatch.StartNew();
            _quadTree.Query(_x - 1000, _y - 1000, _x + 1000, _y + 1000);
            //watch.Stop();
            //Log.Debug("FrameTime", "query:" +watch.ElapsedMilliseconds);
            //watch = Stopwatch.StartNew();
            var count = _quadTree.GetQueryResultsCount();
            _quadTree.GetQueryResults(_queryResult);

            //var drawList = new List<MeshExportData>();
            _layers.Clear();
            for (int i = 0; i < count; i++)
            {
                var id = _queryResult[i];
                var data = _data[id];
                var z = (int) (data.Z*1000);
                if (!_layers.ContainsKey(z))
                    _layers.Add(z, new MeshLayer(z));
                _layers[z].Data.Add(data);
                //drawList.Add(data);
            }
            _layersList = _layers.Values.OrderByDescending(l => l.LayerIndex).ToList();
            foreach (var layer in _layersList)
            {
                layer.Build();
            }

            //drawList = drawList.OrderByDescending(d => d.Z).ToList();
            //watch.Stop();
            //Log.Debug("FrameTime", "prepare:" + watch.ElapsedMilliseconds);

            //watch = Stopwatch.StartNew();
            _graphics.Viewport(w, h);

            _graphics.Clear();
            const float zoom = 0.001f;
            _graphics.GetUniforms().SetProjection(new float[]
            {
                zoom, 0, 0, 0,
                0, zoom*_w/_h, 0, 0,
                0, 0, 1, 0,
                -_x * zoom, -_y * zoom*_w/_h, -_z, 1,
            });
            var calls = 0;
            _graphics.GetUniforms().SetLightMap(_texture);
            foreach (var l in _layersList)
            {
                foreach (var b in l.Blocks)
                {
                    _graphics.GetUniforms().SetTexture(_textures[b.Value.TextureId]);
                    _graphics.Draw(
                        b.Value.Vertices,
                        b.Value.Indices,
                        b.Value.Indices.Length / 3);
                    calls++;
                }
            }

            /*
            foreach (var data in drawList)
            {
                _graphics.GetUniforms().GetTextureUniform().SetTexture(_textures[data.TextureId]);

                _graphics.Draw(
                    data.Vertices,
                    data.Indices,
                    data.Indices.Length / 3);
            }
            */
            //watch.Stop();
            //Log.Debug("FrameTime", "render:" + watch.ElapsedMilliseconds);

            /*
            var calls = 0;
            foreach (var l in _layersList)
            {
                foreach (var b in l.Blocks)
                {
                    _graphics.GetUniforms().GetTextureUniform().SetTexture(_textures[b.Value.TextureId]);
                    _graphics.Draw(
                        b.Value.Vertices,
                        b.Value.Indices,
                        b.Value.Indices.Length / 3);
                    calls++;
                }
            }
            */
            /*
            foreach (var data in _data)
            {
                _graphics.GetUniforms().GetTextureUniform().SetTexture(_textures[data.TextureId]);

                _graphics.Draw(
                    data.Vertices,
                    data.Indices,
                    data.Indices.Length / 3);
            }
             */
            //Log.Debug("FrameTime", (System.Environment.TickCount - time).ToString());

        }

        private float? _downMX;
        private float? _downMY;
        private float? _downX;
        private float? _downY;

        private void Main_MouseDown(object sender, MotionEvent e)
        {
            _downMX = e.GetX();
            _downMY = e.GetY();
            _downX = _x;
            _downY = _y;
        }

        private void Main_MouseMove(object sender, MotionEvent e)
        {
            if (
                (_downMX != null) &&
                (_downMY != null) &&
                (_downX != null) &&
                (_downY != null))
            {
                var offsetX = e.GetX() - _downMX.Value;
                var offsetY = e.GetY() - _downMY.Value;
                _x = _downX.Value + offsetX;
                _y = _downY.Value - offsetY;
            }
        }

        public bool OnTouch(View v, MotionEvent e)
        {
            switch (e.Action)
            {
                case MotionEventActions.Down:
                    Main_MouseDown(this, e);
                    break;
                case MotionEventActions.Move:
                    Main_MouseMove(this, e);
                    break;
            }
            return true;
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

        public static void LoadPixels(this Texture texture, int width, int height, int[] pixels)
        {
            var pinnedArray = GCHandle.Alloc(pixels, GCHandleType.Pinned);
            var pointer = pinnedArray.AddrOfPinnedObject();
            texture.LoadPixels(width, height, pointer);
            pinnedArray.Free();
        }
    }

    public static class ProjectionUniformExtension
    {
        public static void SetProjection(this UniformsFacade uniforms, float[] value)
        {
            var pinnedArray = GCHandle.Alloc(value, GCHandleType.Pinned);
            var pointer = pinnedArray.AddrOfPinnedObject();
            uniforms.SetProjection(pointer);
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
                facade.GetEffectsFacade().GetTextureLightmapColorEffect(),
                facade.GetVertexFormatsFacade().GetPositionTextureColorFormat(),
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
            foreach (var data in Data)
            {
                if (!Blocks.ContainsKey(data.TextureId))
                    Blocks.Add(data.TextureId, new MeshBlock(data.TextureId));
                Blocks[data.TextureId].Data.Add(data);
            }
            foreach (var block in Blocks)
                block.Value.Build();
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

    public class MeshExportData
    {
        public float Z;
        public int TextureId;
        public MeshExportVertex[] Vertices;
        public ushort[] Indices;
    }

}