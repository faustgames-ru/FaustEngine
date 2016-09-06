using GraphicsBinding;

namespace Zombo.Editor
{
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
