using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using FaustGames.Framework.WinForms;

namespace GameSampleSpine
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            var mainWindow = new Main();
            var plaform = new PlatformWinForms(mainWindow.GetComponents(), mainWindow, new GameSpine());
            Application.Run(mainWindow);
        }
    }
}
