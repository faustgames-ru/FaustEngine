using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Android.App;
using Android.Content;
using Android.Content.Res;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.OS;
using Android.Util;

namespace App8
{
    [Activity(Label = "App8", MainLauncher = true, Icon = "@drawable/icon")]
    public class MainActivity : Activity
    {
        int count = 1;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            var jniEnv = JNIEnv.Handle;
            var jAssets = Assets.Handle;
            SetContentView(Resource.Layout.Main);
            // Get our button from the layout resource,
            // and attach an event to it
            Button button = FindViewById<Button>(Resource.Id.MyButton);
            button.Click += delegate
            {

                var json = string.Empty;
                try
                {
                    setupAssetsManager(jniEnv, jAssets);
                    json = LoadJson();
                }
                catch (Exception ex)
                {
                    Log.Error("EXCEPTION", ex.StackTrace);
                }

                button.Text = string.Format($"{count++} clicks!");
            };
        }

        unsafe string LoadJson()
        {
            var fileName = "2aa0cf15-a5f0-44c6-a3c8-5c7209ec01b7/Razboinik.json";
            var result = string.Empty;
            if (existsAssets(fileName))
            {
                openAssets(fileName);
                var jsonBytes = new byte[1024*1024];
                fixed (byte* jsonPtr = jsonBytes)
                {
                    readAssets(new IntPtr(jsonPtr), jsonBytes.Length);
                    result = Marshal.PtrToStringAnsi(new IntPtr(jsonPtr));
                }
                closeAssets();
            }
            return result;
        }

        [DllImport("libsharedobject1", EntryPoint = "setupAssetsManager")]
        private static extern void setupAssetsManager(IntPtr env, IntPtr assetManager);
        [DllImport("libsharedobject1", EntryPoint = "existsAssets")]
        private static extern bool existsAssets(string assetsName);
        [DllImport("libsharedobject1", EntryPoint = "openAssets")]
        private static extern void openAssets(string assetsName);
        [DllImport("libsharedobject1", EntryPoint = "readAssets")]
        private static extern int readAssets(IntPtr buffer, int bytesLimit);
        [DllImport("libsharedobject1", EntryPoint = "closeAssets")]
        private static extern void closeAssets();
    }
}

