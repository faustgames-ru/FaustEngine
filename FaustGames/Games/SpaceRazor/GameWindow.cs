using System;
using System.Diagnostics;
using System.Windows.Forms;
using GraphicsBinding;
using razor;

namespace SpaceRazor
{
    public partial class GameWindow : Form
    {
        public GameWindow()
        {
            InitializeComponent();
            _glWindow = new GLWindow(this, OpenGLGlobals.OpenGLContext);

            try
            {
                OpenGLGlobals.InitializeGLFunctionsEntires(_glWindow);
                _game = razor.razor.CreateRazorGame();
                _game.SetRootPath(Application.StartupPath.Replace('\\', '/') + "/RazorContent/");
            }
            catch (Exception e)
            {
                OnException(e);
            }

            Application.Idle += ApplicationIdle;
            Closed += WindowClosed;
        }
        
        private void OnException(Exception e)
        {
            MessageBox.Show(e.ToString());
        }

        private void InternalCallUpdateAndRender()
        {
            var seconds = (float)_stopwatch.Elapsed.TotalSeconds;
            _stopwatch.Restart();
            _game.UpdateEnvironment(ClientSize.Width, ClientSize.Height, seconds);
            if (!_initialized)
            {
                ExecuteOpenGLAction(InternalCallLoad);
                _initialized = true;
            }
            _game.Update();
            ExecuteOpenGLAction(InternalCallRender);
        }

        private void InternalCallLoad()
        {
            _game.Load();
        }
        private void InternalCallRender()
        {
            _game.Render();
            _glWindow.SwapOpenGLBuffers();
        }

        public void ExecuteOpenGLAction(Action action)
        {
            if (action == null)
                return;
            OpenGLGlobals.OpenGLContext.MakeCurrent(_glWindow);
            action();
            OpenGLGlobals.OpenGLContext.MakeCurrent(null);
        }

        private bool IsApplicationIdle()
        {
            NativeMessage result;
            return WinApi.PeekMessage(out result, IntPtr.Zero, (uint)0, (uint)0, (uint)0) == 0;
        }

        private void ApplicationIdle(object sender, EventArgs e)
        {
            while (IsApplicationIdle())
            {
                InternalCallUpdateAndRender();
            }
        }

        private void WindowClosed(object sender, EventArgs e)
        {
            Application.Idle -= ApplicationIdle;
        }

        private bool _initialized = false;
        private readonly GLWindow _glWindow;
        private readonly RazorGame _game;
        private Stopwatch _stopwatch = new Stopwatch();
    }
}
