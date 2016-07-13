using GraphicsBinding;

namespace SpaceRazor
{
    public static class OpenGLGlobals
    {
        public static OpenGLContext OpenGLContext = new OpenGLContext();

        public static void InitializeGLFunctionsEntires(GLWindow window)
        {
            if (IsGLInitialized) return;
            OpenGLContext.MakeCurrent(window);

            razor.razor.InitRazorRenderContext();

            OpenGLContext.MakeCurrent(null);
        }

        public static bool IsGLInitialized = false;
    }
}
