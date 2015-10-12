using FaustGames.Framework;

namespace WallpapersWindowsClient
{
    public class WallpaperController : Game
    {
        private asteroids.AsteroidsNativeController _naticveController;
        public WallpaperController()
        {
            _naticveController = asteroids.asteroids.CreateAsteroidsNativeController();
        }

        public override void Load(LoadArgs e)
        {
            _naticveController.Load();
        }

        public override void Update(UpdateArgs e)
        {
            _naticveController.Update((float)e.Elapsed.TotalSeconds);
        }

        public override void Render(RenderArgs e)
        {
            _naticveController.Render(e.ViewportWidth, e.ViewportHeight);
        }
    }
}