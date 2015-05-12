namespace FaustGames.Framework
{
    public abstract class Game
    {
        public abstract void Load(LoadArgs e);
        public abstract void Update(UpdateArgs e);
        public abstract void Render(RenderArgs e);
    }
}
