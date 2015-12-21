using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Windows.Forms;
using zombo;
using Zombo.GraphicsBinding;
using Zombo.Windows.Properties;

namespace Zombo.Windows
{
    public partial class GameWindow : Form
    {
        public GameWindow()
        {
            SuspendLayout();
            if (Settings.Default.FullScreen)
            {
                FormBorderStyle = FormBorderStyle.None;
                WindowState = FormWindowState.Maximized;
                TopMost = true;
            }
            else
            {
                FormBorderStyle = FormBorderStyle.FixedSingle;
                StartPosition = FormStartPosition.CenterScreen;
                WindowState = FormWindowState.Normal;
                TopMost = false;
            }
            ResumeLayout();

            SetStyle(ControlStyles.Opaque, true);
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            InitializeComponent();

            if (!Settings.Default.FullScreen)
            {
                Width = Settings.Default.WindowWidth;
                Height = Settings.Default.WindowHeight;
            }

            _glWindow = new GLWindow(this, OpenGLGlobals.OpenGLContext);

            try
            {
                OpenGLGlobals.InitializeGLFunctionsEntires(_glWindow);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.ToString());
            }

            Application.Idle += ApplicationOnIdle;
            Disposed += (sender, args) =>
            {
                if (_game != null)
                {
                    _game.Release();
                }
                Application.Idle -= ApplicationOnIdle;
            };
        }

        private void ApplicationOnIdle(object sender, EventArgs e)
        {
            Invalidate();
        }
        public void ExecuteOpenGLAction(Action action)
        {
            if (action == null)
                return;
            OpenGLGlobals.OpenGLContext.MakeCurrent(_glWindow);
            action();
            OpenGLGlobals.OpenGLContext.MakeCurrent(null);
        }

        private double _delta;
        readonly Queue<Action> _renderActions = new Queue<Action>();

        public virtual void OnRender(RenderEventArgs e)
        {
            if (_game == null) return;
            while (_renderActions.Count > 0)
            {
                var action = _renderActions.Dequeue();
                action();
            }

            _delta += e.Elapsed.TotalSeconds;
            if (_delta > 0.001)
            {
                _game.Update(ClientSize.Width, ClientSize.Height, (float)_delta);
                _delta = 0;
            }
            _game.Render(ClientSize.Width, ClientSize.Height, (float)e.Elapsed.TotalSeconds);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (_stopwatch != null)
            {
                _stopwatch.Stop();
                _renderEventArgs.Elapsed = _stopwatch.Elapsed;
            }
            else
            {
                _renderEventArgs.Elapsed = new TimeSpan(0);
            }

            _stopwatch = Stopwatch.StartNew();

            ExecuteOpenGLAction(() =>
            {
                OnRender(_renderEventArgs);
                _glWindow.SwapOpenGLBuffers();
            });
        }
        
        Stopwatch _stopwatch;
        private readonly GLWindow _glWindow;
        private readonly RenderEventArgs _renderEventArgs = new RenderEventArgs();
        private ZomboGame _game;

        private void LoadGame()
        {
            _game.SetRootPath(Application.StartupPath.Replace('\\', '/') +"/");
            _game.Load(ClientSize.Width, ClientSize.Height);
        }

        private void GameWindow_Load(object sender, EventArgs e)
        {
            _game = zombo.zombo.CreateZomboGame();
            _renderActions.Enqueue(LoadGame);
        }
    }


    public class RenderEventArgs : EventArgs
    {
        public TimeSpan Elapsed;
    }

    public static class OpenGLGlobals
    {
        public static OpenGLContext OpenGLContext = new OpenGLContext();

        public static void InitializeGLFunctionsEntires(GLWindow window)
        {
            if (IsGLInitialized) return;
            OpenGLContext.MakeCurrent(window);

            zombo.zombo.InitZomboRenderContext();

            OpenGLContext.MakeCurrent(null);
        }

        public static bool IsGLInitialized = false;
    }
}
