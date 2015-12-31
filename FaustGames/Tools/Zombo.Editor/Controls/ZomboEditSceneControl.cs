using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using zombo;
using Zombo.GraphicsBinding;

namespace Zombo.Editor.Controls
{
    public partial class ZomboEditSceneControl : UserControl
    {
        private readonly GLWindow _glWindow;
        private readonly ZomboEditor _editor;
        private readonly ZomboEditorMouse _editorMouse;
        private readonly ZomboEditorViewport _editorViewport;
        private bool _initialized;

        private readonly Lazy<Font> _designerModeFont = new Lazy<Font>(()=>new Font(FontFamily.GenericSansSerif, 8.0f));

        public ZomboEditor ZomboEditor => _editor;
        public bool _ready = false;

        public ZomboEditSceneControl()
        {
            InitializeComponent();
            if (!DesignMode)
            {
                _glWindow = new GLWindow(this, OpenGLGlobals.OpenGLContext);
                try
                {
                    OpenGLGlobals.InitializeGLFunctionsEntires(_glWindow);
                }
                catch (Exception ex)
                {
                    OnException(ex);
                }
                try
                {
                    _editor = zombo.zombo.GetZomboEditor();
                    var editorInput = zombo.zombo.GetZomboEditorInput();
                    _editorMouse = editorInput.GetEditorMouse();
                    _editorViewport = zombo.zombo.GetZomboEditorViewport();
                }
                catch (Exception ex)
                {
                    OnException(ex);
                }
            }
            _ready = true;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (!DesignMode)
            {
                CallRender();
            }
            else
            {
                e.Graphics.FillRectangle(Brushes.RoyalBlue, ClientRectangle);
                var text = "Scene editor";
                var size = e.Graphics.MeasureString(text, _designerModeFont.Value);
                e.Graphics.DrawString(text, _designerModeFont.Value, Brushes.White, 
                    new PointF((ClientSize.Width - size.Width) / 2, (ClientSize.Height - size.Height) / 2));
            }
        }

        private void OnException(Exception ex)
        {
        }

        protected override void OnResize(EventArgs e)
        {
            base.OnResize(e);
            if (!_ready) return;
            CallUpdateAndRender();
        }

        public void ExecuteOpenGLAction(Action action)
        {
            if (action == null)
                return;
            OpenGLGlobals.OpenGLContext.MakeCurrent(_glWindow);
            action();
            OpenGLGlobals.OpenGLContext.MakeCurrent(null);
        }

        private uint GetMouseState()
        {
            MouseButtons buttons = MouseButtons;
            uint buttonsState = 0;
            if ((buttons & MouseButtons.Left) != MouseButtons.None)
                buttonsState |= 0x1;
            if ((buttons & MouseButtons.Right) != MouseButtons.None)
                buttonsState |= 0x2;
            if ((buttons & MouseButtons.Middle) != MouseButtons.None)
                buttonsState |= 0x4;
            return buttonsState;
        }

        private void CallRender()
        {
            ExecuteOpenGLAction(() =>
            {
                try
                {
                    _editorViewport.Update(ClientSize.Width, ClientSize.Height);
                    _editor.Render();
                }
                catch (Exception ex)
                {
                    OnException(ex);
                }
                _glWindow.SwapOpenGLBuffers();
            });
        }

        private void CallUpdateAndRender()
        {            
            _editorViewport.Update(ClientSize.Width, ClientSize.Height);
            if (!_initialized)
            {
                _initialized = true;
                ExecuteOpenGLAction(() =>
                {
                    _editor.Init();
                });
            }
            _editor.Update();
            CallRender();
        }

        private void ZomboEditSceneControl_MouseMove(object sender, MouseEventArgs e)
        {
            if (DesignMode) return;
            _editorMouse.Update(e.X, e.Y, GetMouseState());
            CallUpdateAndRender();
        }

        private void ZomboEditSceneControl_MouseDown(object sender, MouseEventArgs e)
        {
            if (DesignMode) return;
            _editorMouse.Update(e.X, e.Y, GetMouseState());
            CallUpdateAndRender();
        }

        private void ZomboEditSceneControl_MouseUp(object sender, MouseEventArgs e)
        {
            if (DesignMode) return;
            _editorMouse.Update(e.X, e.Y, GetMouseState());
            CallUpdateAndRender();
        }
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
