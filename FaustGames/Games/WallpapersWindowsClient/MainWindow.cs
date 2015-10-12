using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;
using FaustGames.Framework;
using FaustGames.Framework.WinForms;
using OpenGLWindow;

namespace WallpapersWindowsClient
{
    public partial class MainWindow : Form
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        public void RunLoop()
        {
            _oglWindow = new OGLWindow(_renderPanel);
            if (components == null)
                components = new Container();
            components.Add(new DisposableContainerComponent(_oglWindow));
            llge.llge.InitRenderContext();
            _game.Load(_loadArgs);
            while (Created)
            {
                UpdateFrame();
                RenderFrame();
                Application.DoEvents();
            }

        }
        private void RenderFrame()
        {
            _renderArgs.ViewportWidth = _renderPanel.ClientSize.Width;
            _renderArgs.ViewportHeight = _renderPanel.ClientSize.Height;
            _game.Render(_renderArgs);
            _oglWindow.SwapOpenGLBuffers();
        }

        private void UpdateFrame()
        {
            _updateArgs.Elapsed = _stopwatch.Elapsed;
            _stopwatch.Restart();
            _game.Update(_updateArgs);
        }

        private readonly LoadArgs _loadArgs = new LoadArgs();
        private readonly UpdateArgs _updateArgs = new UpdateArgs();
        private readonly RenderArgs _renderArgs = new RenderArgs();

        private readonly Stopwatch _stopwatch = new Stopwatch();
        private readonly Game _game = new WallpaperController();
        private OGLWindow _oglWindow;
    }
}
