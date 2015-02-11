using System;
using System.Diagnostics;
using llge;
using OpenTK;
using OpenTK.Graphics;
using OpenTK.Graphics.ES11;
using OpenTK.Platform;
using OpenTK.Platform.Android;

using Android.Views;
using Android.Content;
using Android.Util;

namespace Game.Android
{
	class OpenGlView : AndroidGameView, View.IOnTouchListener
	{
        private readonly Renderer _renderer;

		public OpenGlView (Context context) : base (context)
		{
            ContextRenderingApi = GLVersion.ES2;
            _renderer = new Renderer();
        }

		// This gets called when the drawing surface is ready
		protected override void OnLoad (EventArgs e)
		{
			base.OnLoad (e);


            // Run the render loop
			Run ();
		}

	    private void OnFrameBufferCreated()
	    {
            SetOnTouchListener(this);
            _renderer.Create();
        }

	    // This method is called everytime the context needs
		// to be recreated. Use it to set any egl-specific settings
		// prior to context creation
		//
		// In this particular case, we demonstrate how to set
		// the graphics mode and fallback in case the device doesn't
		// support the defaults
		protected override void CreateFrameBuffer ()
		{
		    bool tryAgain = false;
			// the default GraphicsMode that is set consists of (16, 16, 0, 0, 2, false)
			try {
				Log.Verbose ("GLCube", "Loading with default settings");

				// if you don't call this, the context won't be created
				base.CreateFrameBuffer ();
			} 
            catch (Exception ex) 
            {
				Log.Verbose ("GLCube", "{0}", ex);
			    tryAgain = true;
			}

		    if (tryAgain)
		    {
		        // this is a graphics setting that sets everything to the lowest mode possible so
		        // the device returns a reliable graphics setting.
		        try
		        {
		            Log.Verbose("GLCube", "Loading with custom Android settings (low mode)");
		            GraphicsMode = new AndroidGraphicsMode(0, 0, 0, 0, 0, false);

		            // if you don't call this, the context won't be created
		            base.CreateFrameBuffer();
		        }
		        catch (Exception ex)
		        {
		            Log.Verbose("GLCube", "{0}", ex);
		            throw new Exception("Can't load egl, aborting");
		        }
		    }
		    OnFrameBufferCreated();
		}

		// This gets called on each frame render
		protected override void OnRenderFrame (FrameEventArgs e)
		{
			// you only need to call this if you have delegates
			// registered that you want to have called
			base.OnRenderFrame (e);

		    _renderer.Render(Width, Height);

		    SwapBuffers();
		}

	    public bool OnTouch(View v, MotionEvent e)
	    {
	        return _renderer.OnTouch(v, e);
	    }
	}
}

