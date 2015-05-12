using System.IO;
using System.Runtime.InteropServices;
using FaustGames.Framework;

namespace GameSampleSpine
{
    class GameSpine : Game
    {
        readonly llge.SpineResource _spineResource = llge.llge.CreateSpineResource();
        private llge.SpineSkeleton _skeleton;
        private llge.SpineAnimationStateData _stateData;
        private llge.SpineAnimationState _state;
        private llge.SpineAnimation _death;
        private llge.Batch2d _batch = llge.llge.CreateBatch2d();
        public override void Load(LoadArgs e)
        {
            var atlas = File.ReadAllText("spineboy.atlas");
            var json = File.ReadAllText("spineboy.json");
            _spineResource.Load(atlas, json);

            //for (var i = 0; i < _spineResource.GetSpineAnimationsCount(); i++)
            //{
            //    var animation = _spineResource.GetSpineAnimation(i);
            //    var name = Marshal.PtrToStringAnsi(animation.GetName());

            //}

            _death = _spineResource.GetSpineAnimation(0);

            _skeleton = _spineResource.CreateSkeleton();
            _stateData = _spineResource.CreateStateData();
            _state = _stateData.CreateState();
            _state.SetAnimation(_death, true);
        }

        public override void Update(UpdateArgs e)
        {
            _state.Update((float)e.Elapsed.TotalSeconds);
            _state.Apply(_skeleton);
            _skeleton.UpdateWorldTransform();
        }

        public override void Render(RenderArgs e)
        {
            _skeleton.Render(_batch);
        }
    }
}
