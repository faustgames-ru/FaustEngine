using System;

namespace Zombo.GraphicsBinding
{
    public class OpenGLContext
    {
        public OpenGLContext()
        {
            _hglrc = IntPtr.Zero;
        }

        public void MakeCurrent(GLWindow window)
        {
            if (window == null)
            {
                if (WinApi.wglMakeCurrent(IntPtr.Zero, IntPtr.Zero) == 0)
                {
                    //MessageBox::Show("wglMakeCurrent Failed");
                }
            }
            else
            {
                if (WinApi.wglMakeCurrent(window.hDC, _hglrc) == 0)
                {
                    //MessageBox::Show("wglMakeCurrent Failed");
                }
            }
        }

        IntPtr _hglrc;

        public void CreateContext(IntPtr hdc)
        {
            if (_hglrc != IntPtr.Zero) return;
            if ((_hglrc = WinApi.wglCreateContext(hdc)) == IntPtr.Zero)
            {
                //MessageBox::Show("wglCreateContext Failed");
            }
        }
    };
}