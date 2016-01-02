using System;
using System.Runtime.InteropServices;

namespace Zombo.GraphicsBinding
{
    [Flags]
    public enum PFDBitFlag
    {
        /// <summary>
        /// The buffer can draw to a window or device surface
        /// </summary>
        PFD_DRAW_TO_WINDOW = 4,
        /// <summary>
        /// The buffer can draw to a memory bitmap
        /// </summary>
        PFD_DRAW_TO_BITMAP = 8,
        /// <summary>
        /// The buffer supports GDI drawing. This flag and 
        /// PFD_DOUBLEBUFFER are mutually exclusive in the current generic implementation
        /// </summary>
        PFD_SUPPORT_GDI = 16,
        /// <summary>
        /// The buffer supports OpenGL drawing
        /// </summary>
        PFD_SUPPORT_OPENGL = 32,
        /// <summary>
        /// The pixel format is supported by a device driver that accelerates the generic 
        /// implementation. If this flag is clear and the PFD_GENERIC_FORMAT flag is set,
        /// the pixel format is supported by the generic implementation only
        /// </summary>
        PFD_GENERIC_ACCELERATED = 4096,
        /// <summary>
        /// The pixel format is supported by the GDI software implementation, which is also
        /// known as the generic implementation. If this bit is clear, the pixel format is
        /// supported by a device driver or hardware
        /// </summary>
        PFD_GENERIC_FORMAT = 64,
        /// <summary>
        /// The buffer uses RGBA pixels on a palette-managed device. A logical palette is 
        /// required to achieve the best results for this pixel type. Colors in the palette
        /// should be specified according to the values of the cRedBits, cRedShift, cGreenBits,
        /// cGreenShift, cBluebits, and cBlueShift members of PIXELFORMATDESCRIPTOR structure. The palette should be created and 
        /// realized in the device context before calling wglMakeCurrent
        /// </summary>
        PFD_NEED_PALETTE = 128,
        /// <summary>
        /// Defined in the pixel format descriptors of hardware that supports one hardware palette
        /// in 256-color mode only. For such systems to use hardware acceleration, the hardware palette
        /// must be in a fixed order (for example, 3-3-2) when in RGBA mode or must match the logical
        /// palette when in color-index mode. 
        /// When this flag is set, you must call SetSystemPaletteUse in your program to force a one-to-one
        /// mapping of the logical palette and the system palette. If your OpenGL hardware supports multiple
        /// hardware palettes and the device driver can allocate spare hardware palettes for OpenGL, this flag
        /// is typically clear.
        /// This flag is not set in the generic pixel formats
        /// </summary>
        PFD_NEED_SYSTEM_PALETTE = 256,
        /// <summary>
        /// The buffer is double-buffered. This flag and PFD_SUPPORT_GDI are mutually exclusive in the current 
        /// generic implementation
        /// </summary>
        PFD_DOUBLEBUFFER = 1,
        /// <summary>
        /// The buffer is stereoscopic. This flag is not supported in the current generic implementation
        /// </summary>
        PFD_STEREO = 2,
        /// <summary>
        /// Indicates whether a device can swap individual layer planes with pixel formats that include 
        /// double-buffered overlay or underlay planes. Otherwise all layer planes are swapped together 
        /// as a group. When this flag is set, wglSwapLayerBuffers is supported
        /// </summary>        
        PFD_SWAP_LAYER_BUFFERS = 2048,
        /// <summary>
        /// The requested pixel format can either have or not have a depth buffer. To select a pixel format
        /// without a depth buffer, you must specify this flag. The requested pixel format can be with or 
        /// without a depth buffer. Otherwise, only pixel formats with a depth buffer are considered
        /// </summary>
        /// <remarks>You can specify the following bit flags when calling ChoosePixelFormat</remarks>
        PFD_DEPTH_DONTCARE = 536870912,
        /// <summary>
        /// The requested pixel format can be either single- or double-buffered
        /// </summary>
        /// <remarks>You can specify the following bit flags when calling ChoosePixelFormat</remarks>
        PFD_DOUBLEBUFFER_DONTCARE = 1073741824,
        /// <summary>
        /// The requested pixel format can be either monoscopic or stereoscopic
        /// </summary>
        /// <remarks>You can specify the following bit flags when calling ChoosePixelFormat</remarks>
        PFD_STEREO_DONTCARE = -2147483648,
        /// <summary>
        /// Specifies the content of the back buffer in the double-buffered main color plane following
        /// a buffer swap. Swapping the color buffers causes the content of the back buffer to be copied
        /// to the front buffer. The content of the back buffer is not affected by the swap. PFD_SWAP_COPY
        /// is a hint only and might not be provided by a driver
        /// </summary>
        PFD_SWAP_COPY = 1024,
        /// <summary>
        /// Specifies the content of the back buffer in the double-buffered main color plane following a 
        /// buffer swap. Swapping the color buffers causes the exchange of the back buffer's content with 
        /// the front buffer's content. Following the swap, the back buffer's content contains the front 
        /// buffer's content before the swap. PFD_SWAP_EXCHANGE is a hint only and might not be provided by
        /// a driver 
        /// </summary>
        PFD_SWAP_EXCHANGE = 512,
        /// <summary>
        /// RGBA pixels. Each pixel has four components in this order: red, green, blue, and alpha
        /// </summary>
        PFD_TYPE_RGBA = 0,
        /// <summary>
        /// Color-index pixels. Each pixel uses a color-index value
        /// </summary>
        PFD_TYPE_COLORINDEX = 1
    }

    [Flags]
    public enum PixelType : byte
    {
        /// <summary>
        /// RGBA pixels. Each pixel has four components in this order: red, green, blue, and alpha
        /// </summary>
        PFD_TYPE_RGBA = 0,
        /// <summary>
        /// Color-index pixels. Each pixel uses a color-index value
        /// </summary>
        PFD_TYPE_COLORINDEX = 1
    }

    /// <summary>
    /// The PIXELFORMATDESCRIPTOR structure describes the pixel format of a drawing surface
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct PixelFormatDescriptor
    {
        /// <summary>
        /// Specifies the size of this data structure. This value should be set to sizeof(PIXELFORMATDESCRIPTOR).
        /// </summary>
        public ushort nSize;

        /// <summary>
        /// Specifies the version of this data structure. This value should be set to 1.
        /// </summary>
        public ushort nVersion;

        /// <summary>
        /// A set of bit flags that specify properties of the pixel buffer
        /// </summary>
        public PFDBitFlag dwFlags;

        /// <summary>
        /// Specifies the type of pixel data
        /// </summary>
        public PixelType iPixelType;

        /// <summary>
        /// Specifies the number of color bitplanes in each color buffer. For RGBA pixel types, it is the size of
        /// the color buffer, excluding the alpha bitplanes. For color-index pixels, it is the size of the color-index buffer
        /// </summary>
        public byte cColorBits;

        /// <summary>
        /// Specifies the number of red bitplanes in each RGBA color buffer
        /// </summary>
        public byte cRedBits;

        /// <summary>
        /// Specifies the shift count for red bitplanes in each RGBA color buffer
        /// </summary>
        public byte cRedShift;

        /// <summary>
        /// Specifies the number of green bitplanes in each RGBA color buffer
        /// </summary>
        public byte cGreenBits;

        /// <summary>
        /// Specifies the shift count for green bitplanes in each RGBA color buffer
        /// </summary>
        public byte cGreenShift;

        /// <summary>
        /// Specifies the shift count for green bitplanes in each RGBA color buffer
        /// </summary>
        public byte cBlueBits;

        /// <summary>
        /// Specifies the shift count for blue bitplanes in each RGBA color buffer
        /// </summary>
        public byte cBlueShift;

        /// <summary>
        /// Specifies the number of alpha bitplanes in each RGBA color buffer. 
        /// Alpha bitplanes are not supported
        /// </summary>
        public byte cAlphaBits;

        /// <summary>
        /// Specifies the shift count for alpha bitplanes in each RGBA color buffer. 
        /// Alpha bitplanes are not supported
        /// </summary>
        public byte cAlphaShift;

        /// <summary>
        /// Specifies the total number of bitplanes in the accumulation buffer
        /// </summary>
        public byte cAccumBits;

        /// <summary>
        /// Specifies the number of red bitplanes in the accumulation buffer
        /// </summary>
        public byte cAccumRedBits;

        /// <summary>
        /// Specifies the number of green bitplanes in the accumulation buffer
        /// </summary>
        public byte cAccumGreenBits;

        /// <summary>
        /// Specifies the number of blue bitplanes in the accumulation buffer
        /// </summary>
        public byte cAccumBlueBits;

        /// <summary>
        /// Specifies the number of alpha bitplanes in the accumulation buffer
        /// </summary>
        public byte cAccumAlphaBits;

        /// <summary>
        /// Specifies the depth of the depth (z-axis) buffer
        /// </summary>
        public byte cDepthBits;

        /// <summary>
        /// Specifies the depth of the stencil buffer
        /// </summary>
        public byte cStencilBits;

        /// <summary>
        /// Specifies the number of auxiliary buffers. Auxiliary buffers are not supported
        /// </summary>
        public byte cAuxBuffers;

        /// <summary>
        /// Ignored. Earlier implementations of OpenGL used this member, but it is no longer used
        /// </summary>
        public byte iLayerType;

        /// <summary>
        /// Specifies the number of overlay and underlay planes. Bits 0 through 3 specify up to 15 
        /// overlay planes and bits 4 through 7 specify up to 15 underlay planes
        /// </summary>
        public byte bReserved;

        /// <summary>
        /// Ignored. Earlier implementations of OpenGL used this member, but it is no longer used
        /// </summary>
        public uint dwLayerMask;

        /// <summary>
        /// Specifies the transparent color or index of an underlay plane. When the pixel type is RGBA,
        /// dwVisibleMask is a transparent RGB color value. When the pixel type is color index, it is a
        /// transparent index value
        /// </summary>
        public uint dwVisibleMask;

        /// <summary>
        /// Ignored. Earlier implementations of OpenGL used this member, but it is no longer used
        /// </summary>
        public uint dwDamageMask;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Point
    {
        long x;
        long y;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct NativeMessage
    {
        public IntPtr Handle;
        public uint Message;
        public IntPtr WParameter;
        public IntPtr LParameter;
        public uint Time;
        public Point Location;
    }

    public static class WinApi
    {
        [DllImport("user32.dll")]
        public static extern IntPtr GetDC(IntPtr hWnd);
        [DllImport("gdi32.dll")]
        public static extern bool SwapBuffers(IntPtr hdc);
        [DllImport("gdi32.dll")]
        public static extern int ChoosePixelFormat(IntPtr hdc, [In] ref PixelFormatDescriptor ppfd);
        [DllImport("gdi32.dll")]
        public static extern bool SetPixelFormat(IntPtr hdc, int iPixelFormat, ref PixelFormatDescriptor ppfd);

        [DllImport("opengl32.dll")]
        public static extern int wglMakeCurrent(IntPtr hdc, IntPtr hrc);
        [DllImport("opengl32.dll")]
        public static extern IntPtr wglCreateContext(IntPtr hdc);

        [DllImport("user32.dll")]
        public static extern int PeekMessage(out NativeMessage message, IntPtr window, uint filterMin, uint filterMax, uint remove);
    }
}
