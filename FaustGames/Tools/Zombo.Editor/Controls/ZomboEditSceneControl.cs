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
        private readonly Lazy<Font> _designerModeFont = new Lazy<Font>(()=>new Font(FontFamily.GenericSansSerif, 8.0f));
        public ZomboEditSceneControl()
        {
            InitializeComponent();
            if (!DesignMode)
            {
                _glWindow = new GLWindow(this, OpenGLGlobals.OpenGLContext);
                try
                {
                    _editor = zombo.zombo.CreateZomboEditor();
                    //components.Add(new ZomboEditorDisposer(_editor));
                }
                catch (Exception ex)
                {
                    OnException(ex);
                }
            }
        }
        
        protected override void OnPaint(PaintEventArgs e)
        {
            if (!DesignMode)
            {
                ExecuteOpenGLAction(() =>
                {
                    try
                    {
                        _editor.Render(ClientSize.Width, ClientSize.Height);
                    }
                    catch (Exception ex)
                    {
                        OnException(ex);
                    }
                    _glWindow.SwapOpenGLBuffers();
                });
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

        private void ZomboEditSceneControl_MouseMove(object sender, MouseEventArgs e)
        {
            _editor.UpdateMouse(ClientSize.Width, ClientSize.Height, e.X, e.Y, GetMouseState());
        }

        private void ZomboEditSceneControl_MouseDown(object sender, MouseEventArgs e)
        {
            _editor.UpdateMouse(ClientSize.Width, ClientSize.Height, e.X, e.Y, GetMouseState());
        }

        private void ZomboEditSceneControl_MouseUp(object sender, MouseEventArgs e)
        {
            _editor.UpdateMouse(ClientSize.Width, ClientSize.Height, e.X, e.Y, GetMouseState());
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

    class ZomboEditorDisposer : Component
    {
        private ZomboEditor _editor;

        public ZomboEditorDisposer(ZomboEditor editor)
        {
            _editor = editor;
        }

        protected override void Dispose(bool disposing)
        {
            _editor.Release();
            base.Dispose(disposing);
        }
    }
}
