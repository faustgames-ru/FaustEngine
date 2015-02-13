using System;

using Android.App;
using Android.Content;
using Android.Runtime;
using Android.Util;
using Android.Views;
using Android.Widget;
using Android.OS;
using Android.Content.PM;

namespace Game.Android
{
	// the ConfigurationChanges flags set here keep the EGL context
	// from being destroyed whenever the device is rotated or the
	// keyboard is shown (highly recommended for all GL apps)
    [Activity(Label = "Game.Android",
#if __ANDROID_11__
				HardwareAccelerated=false,
				#endif
        ConfigurationChanges = ConfigChanges.Orientation | ConfigChanges.KeyboardHidden,
        MainLauncher = true,
        Icon = "@drawable/icon")]
    public class MainActivity : Activity
    {
        private OpenGlView view;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            /*
            try
            {
                Java.Lang.JavaSystem.LoadLibrary(@"\libs\armeabi\libllge.so");
            }
            catch (Java.Lang.UnsatisfiedLinkError e)
            {
                Log.Verbose("GLCube", "fail to load libllge.so");
            }
            */
            // Create our OpenGL view, and display it
            view = new OpenGlView(this);
            SetContentView(view);
            h.PostDelayed(Update, 100);
        }
        private double _sum;
        private int _count;

        private double _prepareSum;
        private int _prepareCount;
        private void Update()
        {
            if (Renderer.DTime > 0.001)
            {
                _sum += 1000.0f/Renderer.DTime;
                _count++;
            }

            if (Renderer.PrepareTime > 0.001)
            {
                _prepareSum += Renderer.PrepareTime;
                _prepareCount++;
            }

            Title = string.Format("{1:0.00}({0:0.00})", 1000.0f / Renderer.DTime, _sum / _count);
            Title = string.Format("{1:0.00}({0:0.00})", 1000.0f / Renderer.PrepareTime, _prepareSum / _prepareCount);
            h.PostDelayed(Update, 100);
        }

        Handler h = new Handler();

		protected override void OnPause ()
		{
			// never forget to do this!
			base.OnPause ();
			view.Pause ();
		}

		protected override void OnResume ()
		{
			// never forget to do this!
			base.OnResume ();
			view.Resume ();
		}
	}
}


