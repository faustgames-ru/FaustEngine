using GraphicsBinding;
using llge;
using Moon.Designer.Core.Controls;
using System;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using WinApi = GraphicsBinding.WinApi;

namespace TestColorReplacment
{
    public partial class MainWindow : Form
    {
        uint[] _colors = new []
        {
            0xffff0000,
            0xffff0000,
            0xffff0000,
            0xffff0000,
            0xffff0000,
            0xffff0000,
        };

        public MainWindow()
        {
            InitializeComponent();
            _colordialog.PreviewColorChanged += ColorPreview;

            _glWindow = new GLWindow(this, OpenGLGlobals.OpenGLContext);
            try
            {
                OpenGLGlobals.InitializeGLFunctionsEntires(_glWindow);
                _game = llge.llge.CreateTestGame();
                _game.SetRootPath(Application.StartupPath.Replace('\\', '/') + "/Content/");
            }
            catch (Exception e)
            {
                OnException(e);
            }

            Application.Idle += ApplicationIdle;
            Closed += WindowClosed;

            _hair.BackColor = Color.FromArgb((int)_colors[0]);
            _arms.BackColor = Color.FromArgb((int)_colors[1]);
            _hands.BackColor = Color.FromArgb((int)_colors[2]);
            _thighs.BackColor = Color.FromArgb((int)_colors[3]);
            _foots.BackColor = Color.FromArgb((int)_colors[4]);
            _weapon.BackColor = Color.FromArgb((int)_colors[5]);
        }
        /*
        protected override void WndProc(ref Message m)
        {
            if (IsDisposed) return;
            base.WndProc(ref m);
        }
        */
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

        private unsafe void InternalCallUpdateAndRender()
        {
            try
            {
                var seconds = (float)_stopwatch.Elapsed.TotalSeconds;
                _stopwatch.Restart();
                Text = $"fps: {(1.0f / seconds):000.0}";
                _game.UpdateEnvironment(ClientSize.Width, ClientSize.Height, seconds);
                var mousePos = PointToClient(Cursor.Position);
                if (!_initialized)
                {
                    ExecuteOpenGLAction(InternalCallLoad);
                    _initialized = true;
                }
                fixed (uint* ptr = _colors)
                {
                    _game.UpdateColors(new IntPtr(ptr));
                }
                _game.Update();
                ExecuteOpenGLAction(InternalCallRender);
            }
            catch (Exception ex)
            {
            }
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
        private readonly TestGame _game;
        private Stopwatch _stopwatch = new Stopwatch();

        private ColorPickerDialog _colordialog = new ColorPickerDialog();
        private int _colorindex;

        private void ColorPreview(object sender, EventArgs e)
        {
            _colors[_colorindex] = (uint)_colordialog.Color.ToArgb();
        }

        private void SelectColor(int colorIndex, Button sender)
        {
            _colorindex = colorIndex;
            var prevColor = _colors[_colorindex];
            _colordialog.Color = Color.FromArgb((int)_colors[_colorindex]);
            if (DialogResult.OK == _colordialog.ShowDialog())
            {
                _colors[_colorindex] = (uint)_colordialog.Color.ToArgb();
                sender.BackColor = _colordialog.Color;
            }
            else
            {
                _colors[_colorindex] = prevColor;
            }

        }
        private void _hair_Click(object sender, EventArgs e)
        {
            SelectColor(0, _hair);
        }

        private void _arms_Click(object sender, EventArgs e)
        {
            SelectColor(1, _arms);
        }

        private void _hands_Click(object sender, EventArgs e)
        {
            SelectColor(2, _hands);
        }

        private void _thighs_Click(object sender, EventArgs e)
        {
            SelectColor(3, _thighs);
        }

        private void _foots_Click(object sender, EventArgs e)
        {
            SelectColor(4, _foots);
        }

        private void _weapon_Click(object sender, EventArgs e)
        {
            SelectColor(5, _weapon);
        }
    }
}
