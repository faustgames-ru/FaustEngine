using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Zombo.GraphicsBinding
{
    public class GLWindow
    {
        public GLWindow(Control parentForm, OpenGLContext context)
        {
            hDC = WinApi.GetDC(parentForm.Handle);

            if (hDC != null)
            {
                SetWindowPixelFormat();
                context.CreateContext(hDC);
                InitGL();
            }
        }

        public void Render(float timeDelta)
        {
        }

        public void SwapOpenGLBuffers()
        {
            WinApi.SwapBuffers(hDC);
        }

        internal IntPtr hDC;

        protected int SetWindowPixelFormat()
        {
            PixelFormatDescriptor pfd = new PixelFormatDescriptor               // pfd Tells Windows How We Want Things To Be
            {
                nSize = (ushort)Marshal.SizeOf(typeof(PixelFormatDescriptor)),  // Size Of This Pixel Format Descriptor
                nVersion = 1,                                                   // Version Number
                dwFlags = PFDBitFlag.PFD_DRAW_TO_WINDOW |                       // Format Must Support Window
                          PFDBitFlag.PFD_SUPPORT_OPENGL |                                 // Format Must Support OpenGL
                          PFDBitFlag.PFD_DOUBLEBUFFER,                                    // Must Support Double Buffering
                iPixelType = PixelType.PFD_TYPE_RGBA,                           // Request An RGBA Format
                cColorBits = 16,								                // Select Our Color Depth
                cDepthBits = 16,                                                // 16Bit Z-Buffer (Depth Buffer)  
            };

            int iPixelFormat;

            // get the device context's best, available pixel format match 
            if ((iPixelFormat = WinApi.ChoosePixelFormat(hDC, ref pfd)) == 0)
            {
                //MessageBox.Show("ChoosePixelFormat Failed");
                return 0;
            }

            // make that match the device context's current pixel format 
            if (!WinApi.SetPixelFormat(hDC, iPixelFormat, ref pfd))
            {
                //MessageBox.Show("SetPixelFormat Failed");
                return 0;
            }

            return 1;
        }

        void InitGL()
        {
        }
    };
}