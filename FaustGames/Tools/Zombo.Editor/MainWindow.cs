﻿using System;
using System.Diagnostics;
using System.Windows.Forms;
using GraphicsBinding;
using InputHook;
using zombo;
using WinApi = GraphicsBinding.WinApi;

namespace Zombo.Editor
{
    public partial class MainWindow : Form
    {
        public MainWindow()
        {
            InitializeComponent();
            Input.Register(Handle);
            _glWindow = new GLWindow(this, OpenGLGlobals.OpenGLContext);
            try
            {
                OpenGLGlobals.InitializeGLFunctionsEntires(_glWindow);
                _editor = zombo.zombo.CreateZomboEditor();
                _editor.SetRootPath(Application.StartupPath.Replace('\\', '/') + "/ZmoboContent/");
            }
            catch (Exception e)
            {
                OnException(e);
            }

            Application.Idle += ApplicationIdle;
            Closed += WindowClosed;
        }

        protected override void WndProc(ref Message m)
        {
            Input.WinProc(ref m);
            base.WndProc(ref m);
        }

        private void OnException(Exception e)
        {
            MessageBox.Show(e.ToString());
        }

        private uint GetMouseButtons()
        {
            uint mouseButtons = 0;
            if ((MouseButtons & MouseButtons.Left) != MouseButtons.None)
            {
                mouseButtons |= 0x1;
            }
            if ((MouseButtons & MouseButtons.Right) != MouseButtons.None)
            {
                mouseButtons |= 0x2;
            }
            if ((MouseButtons & MouseButtons.Middle) != MouseButtons.None)
            {
                mouseButtons |= 0x4;
            }
            return mouseButtons;
        }

        private void InternalCallUpdateAndRender()
        {
            Input.Update();

            var seconds = (float)_stopwatch.Elapsed.TotalSeconds;
            _stopwatch.Restart();
            Text = $"fps: {(1.0f / seconds):000.0}";
            _editor.UpdateEnvironment(ClientSize.Width, ClientSize.Height, seconds);
            var mousePos = PointToClient(Cursor.Position);
            _editor.UpdateMouse(Input.Mouse.XDelta, Input.Mouse.YDelta, mousePos.X, mousePos.Y, GetMouseButtons());
            if (!_initialized)
            {
                ExecuteOpenGLAction(InternalCallLoad);
                _initialized = true;
            }
            _editor.Update();
            ExecuteOpenGLAction(InternalCallRender);
        }

        private void InternalCallLoad()
        {
            _editor.Load();
        }
        private void InternalCallRender()
        {
            _editor.Render();
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
        private readonly ZomboEditor _editor;
        private Stopwatch _stopwatch = new Stopwatch();
    }
}
