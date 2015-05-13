using System;
using System.ComponentModel;
using System.Windows.Forms;
using OpenGLWindow;

namespace FaustGames.Framework.WinForms
{
    public class PlatformWinForms
    {
        public PlatformWinForms(IContainer container, IWin32Window renderRegion, Game game)
        {
            Init(container, renderRegion, game);
        }

        public void Init(IContainer container, IWin32Window renderRegion, Game game)
        {
            _container = container;
            _renderRegion = renderRegion;
            _game = game;
            _timer = new Timer(_container);
            _timer.Interval = 10;
            _timer.Tick += TimerOnTick;
            _timer.Enabled = true;

            _oglWindow = new OGLWindow(_renderRegion);
            _container.Add(new DisposableContainerComponent(_oglWindow));
        }

        private void TimerOnTick(object sender, EventArgs eventArgs)
        {
            if (!_loaded)
            {
                _game.Load(_loadArgs);
                _loaded = true;
            }
            _updateArgs.Elapsed = TimeSpan.FromMilliseconds(_timer.Interval);
            _game.Update(_updateArgs);
            _game.Render(_renderArgs);
            _oglWindow.SwapOpenGLBuffers();
        }

        private bool _loaded;
        private Timer _timer;
        private IContainer _container;
        private Game _game;
        private readonly UpdateArgs _updateArgs = new UpdateArgs();
        private readonly RenderArgs _renderArgs = new RenderArgs();
        private readonly LoadArgs _loadArgs = new LoadArgs();
        private IWin32Window _renderRegion;
        private OGLWindow _oglWindow;
    }
}
