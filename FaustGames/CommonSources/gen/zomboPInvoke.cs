/* ============================================================== */
/* This file is automatically generated. Please do not modify it. */
/* ============================================================== */

using System;
using System.Runtime.InteropServices;

namespace zombo
{
	public class Version
	{
#if __UNIFIED__
		public const string Dll = "__Internal";
#else
		public const string Dll = "zombo";
#endif
	}
	
	public class ZomboEditor
	{
		public IntPtr ClassInstance;
		public void SetRootPath (String rootPath)
		{
			zombo_ZomboEditor_setRootPath(ClassInstance, rootPath);
		}
		
		[DllImport(Version.Dll)]
		static extern private void zombo_ZomboEditor_setRootPath (IntPtr classInstance, String rootPath);
		public void UpdateMouse (int rawX, int rawY, int windowX, int windowY, uint buttons)
		{
			zombo_ZomboEditor_updateMouse(ClassInstance, rawX, rawY, windowX, windowY, buttons);
		}
		
		[DllImport(Version.Dll)]
		static extern private void zombo_ZomboEditor_updateMouse (IntPtr classInstance, int rawX, int rawY, int windowX, int windowY, uint buttons);
		public void UpdateEnvironment (int w, int h, float ellapsedTime)
		{
			zombo_ZomboEditor_updateEnvironment(ClassInstance, w, h, ellapsedTime);
		}
		
		[DllImport(Version.Dll)]
		static extern private void zombo_ZomboEditor_updateEnvironment (IntPtr classInstance, int w, int h, float ellapsedTime);
		public void Load ()
		{
			zombo_ZomboEditor_load(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void zombo_ZomboEditor_load (IntPtr classInstance);
		public void Update ()
		{
			zombo_ZomboEditor_update(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void zombo_ZomboEditor_update (IntPtr classInstance);
		public void Render ()
		{
			zombo_ZomboEditor_render(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void zombo_ZomboEditor_render (IntPtr classInstance);
		public void Release ()
		{
			zombo_ZomboEditor_release(ClassInstance);
		}
		
		[DllImport(Version.Dll)]
		static extern private void zombo_ZomboEditor_release (IntPtr classInstance);
	}
	
	public class zombo
	{
		static public void InitZomboRenderContext ()
		{
			initZomboRenderContext();
		}
		
		[DllImport(Version.Dll)]
		static extern private void initZomboRenderContext ();
		static public ZomboEditor CreateZomboEditor ()
		{
			return new ZomboEditor{ ClassInstance = createZomboEditor() };
		}
		
		[DllImport(Version.Dll)]
		static extern private IntPtr createZomboEditor ();
	}
	
}

