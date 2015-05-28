using System;
using System.IO;
using System.Runtime.InteropServices;
using FaustGames.Framework;
using llge;

namespace GameSampleSpine
{
    class GameSpine : Game
    {
        public static GameSpine Instance = new GameSpine();

        public override void Load(LoadArgs e)
        {
            _graphicsFactory = llge.llge.CreateGraphicsFactory();
            _graphics = _graphicsFactory.CreateGraphicsFacade();
            llge.llge.InitRenderContext();
            _graphics.Create();

            var atlas = File.ReadAllText("spineboy.atlas");
            var json = File.ReadAllText("spineboy.json");
            _content.StartLoad();
            _spineResource.Load(atlas, json);
            _content.FinishLoad();

            //for (var i = 0; i < _spineResource.GetSpineAnimationsCount(); i++)
            //{
            //    var animation = _spineResource.GetSpineAnimation(i);
            //    var name = Marshal.PtrToStringAnsi(animation.GetName());

            //}

            _walk = _spineResource.GetSpineAnimation(0); //walk
            
            _death = _spineResource.GetSpineAnimation(0); //death
            //_death = _spineResource.GetSpineAnimation(1); //hit
            //_death = _spineResource.GetSpineAnimation(2); //idle
            _jump = _spineResource.GetSpineAnimation(3); //jump
            _run = _spineResource.GetSpineAnimation(4); //run
            _shoot = _spineResource.GetSpineAnimation(5); //shoot
            //_death = _spineResource.GetSpineAnimation(6); //test
            _walk = _spineResource.GetSpineAnimation(7); //walk
            
            _skeleton = _spineResource.CreateSkeleton();
            _stateData = _spineResource.CreateStateData();

            const float defaultMix = 0.2f;
            
            _stateData.SetMix(_run, _death, defaultMix);
            _stateData.SetMix(_run, _death, defaultMix);
            _stateData.SetMix(_run, _jump, defaultMix);
            _stateData.SetMix(_jump, _run, defaultMix);
            _stateData.SetMix(_run, _walk, defaultMix);
            _stateData.SetMix(_walk, _run, defaultMix);
            _stateData.SetMix(_walk, _jump, defaultMix);
            _stateData.SetMix(_jump, _walk, defaultMix);
            
            _state = _stateData.CreateState();
            _state.SetAnimation(_walk, true, false);

            _stateShoot = _stateData.CreateState();
            _stateShoot.SetAnimation(_shoot, true, false);
        }

        public void Jump()
        {
            _state.SetAnimation(_jump, true, false);
            _state.AddAnimation(_walk, true, -0.25f);
        }
        public void Walk()
        {
            _state.SetAnimation(_walk, true, true);
        }
        public void Run()
        {
            _state.SetAnimation(_run, true, true);
        }
        public override void Update(UpdateArgs e)
        {
            _content.Update();
            _state.Update((float)e.Elapsed.TotalSeconds);
            _stateShoot.Update((float)e.Elapsed.TotalSeconds);
            _state.Apply(_skeleton);
            _stateShoot.Apply(_skeleton);
            _skeleton.UpdateWorldTransform();
        }

        private readonly float[] _transform =
        {
            0.001f, 0.00f, 0.00f, 0.00f,
            0.00f, 0.001f, 0.00f, 0.00f,
            0.00f, 0.00f, 1.00f, 0.00f,
            0.00f, 0.00f, 0.00f, 1.00f
        };

        private unsafe void AddTransform()
        {
            fixed (float* ptr = _transform)
            {
                _batch.AddProjection(new IntPtr(ptr));
            }
        }

        public override void Render(RenderArgs e)
        {
            const float scalex = 0.001f;
            const float scaley = scalex * 4 / 3;
            _transform[0] = scalex;
            _transform[5] = scaley;

            _graphics.SetClearState(0xffa0c0c0, 1.0f);
            _graphics.Clear();
            _batch.StartBatch();
            AddTransform();
            _skeleton.Render(_batch);
            _batch.FinishBatch();
            _batch.Execute();
        }

        private SpineAnimationStateData _stateData;
        private SpineAnimationState _state;
        private SpineAnimationState _stateShoot;
        private SpineAnimation _walk;
        private SpineAnimation _run;
        private SpineAnimation _shoot;
        private SpineAnimation _death;
        private SpineAnimation _jump;

        readonly SpineResource _spineResource = llge.llge.CreateSpineResource();
        private SpineSkeleton _skeleton;
        private Batch2d _batch = llge.llge.CreateBatch2d();
        private GraphicsFactory _graphicsFactory = llge.llge.CreateGraphicsFactory();
        private GraphicsFacade _graphics;
        private ContentManager _content = llge.llge.CreateContentManager();
    }
}
