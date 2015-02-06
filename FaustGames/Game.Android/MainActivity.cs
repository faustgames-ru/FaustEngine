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
	[Activity (Label = "Game.Android",
				#if __ANDROID_11__
				HardwareAccelerated=false,
				#endif
		ConfigurationChanges = ConfigChanges.Orientation | ConfigChanges.KeyboardHidden,
		MainLauncher = true,
		Icon = "@drawable/icon")]
	public class MainActivity : Activity
	{
		OpenGlView view;

		protected override void OnCreate (Bundle bundle)
		{
			base.OnCreate (bundle);
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
			view = new OpenGlView (this);
			SetContentView (view);
		}

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


