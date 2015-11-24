using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ContentMapGenerator
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            var appPath = Application.StartupPath;
            var folder = "Content";
            var path = Path.Combine(appPath, folder);
            if (Directory.Exists(path))
            {
                ParseDirectory(path, path);
            }

            /*
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
            */
        }

        static void ParseDirectory(string root, string folder)
        {
        }
    }
}
