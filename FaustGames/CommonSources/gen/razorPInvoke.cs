/* ============================================================== */
/* This file is automatically generated. Please do not modify it. */
/* ============================================================== */

using System;
using System.Runtime.InteropServices;

namespace razor
{
	public class Version
	{
#if __IOS__
		public const string Dll = "__Internal";
#else
		public const string Dll = "razor";
#endif
	}
	
	public class RazorGame
	{
		public IntPtr ClassInstance;
		public void SetRootPath (String rootPath)
		{
			razor_RazorGame_setRootPath(ClassInstance, rootPath);
		}
		
		[DllImport(Version.Dll)]
		static extern private void razor_RazorGame_setRootPath (IntPtr classInstance, String rootPath);
		public void UpdateMouse (int rawX, int rawY, int windowX, int windowY, uint buttons)
		{
			razor_RazorGame_updateMouse(ClassInstance, rawX, rawY, windowX, windowY, buttons);
		}
		
		[DllImport(Version.Dll)]
		static extern private void razor_RazorGame_updateMouse (IntPtr classInstance, int rawX, int rawY, int windowX, int windowY, uint buttons);
		public void UpdateEnvironment (int w, int h, float ellapsedTime)
		{
			razor_RazorGame_updateEnvironment(ClassInstance, w, h, ellapsedTime);
		}
		
		[DllImport(Version.Dll)]
		static extern private void razor_RazorGame_updateEnvironment (IntPtr classInstance, int w, int h, float ellapsedTime);
		public void Load ()
		{
			razor_RazorGame_load(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void razor_RazorGame_load (IntPtr classInstance);
		public void Update ()
		{
			razor_RazorGame_update(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void razor_RazorGame_update (IntPtr classInstance);
		public void Render ()
		{
			razor_RazorGame_render(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void razor_RazorGame_render (IntPtr classInstance);
		public void Release ()
		{
			razor_RazorGame_release(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void razor_RazorGame_release (IntPtr classInstance);
	}
	
	public class razor
	{
		static public void InitRazorRenderContext ()
		{
			initRazorRenderContext();
		}
		
		[DllImport(Version.Dll)]
		static extern private void initRazorRenderContext ();
		static public RazorGame CreateRazorGame ()
		{
			return new RazorGame{ ClassInstance = createRazorGame() };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr createRazorGame ();
	}
	
}

