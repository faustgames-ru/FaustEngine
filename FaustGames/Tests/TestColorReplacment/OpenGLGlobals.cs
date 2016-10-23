using GraphicsBinding;

namespace TestColorReplacment
{
    public static class OpenGLGlobals
    {
        public static OpenGLContext OpenGLContext = new OpenGLContext();

        public static void InitializeGLFunctionsEntires(GLWindow window)
        {
            if (IsGLInitialized) return;
            OpenGLContext.MakeCurrent(window);

            llge.llge.InitRenderContext();

            OpenGLContext.MakeCurrent(null);
        }

        public static bool IsGLInitialized = false;
    }
}
