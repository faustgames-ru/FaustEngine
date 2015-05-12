using System.ComponentModel;
using System.Windows.Forms;
using OpenGLWindow;

namespace GameSampleSpine
{
    public partial class Main : Form
    {
        public Main()
        {
            InitializeComponent();
            if (components == null)
            {
                components = new Container();
            }
        }

        public IContainer GetComponents()
        {
            return components;
        }
    }
}
