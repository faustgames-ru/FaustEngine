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

        private void _jump_Click(object sender, System.EventArgs e)
        {
            GameSpine.Instance.Jump();
        }

        private void _walk_Click(object sender, System.EventArgs e)
        {
            GameSpine.Instance.Walk();
        }

        private void _run_Click(object sender, System.EventArgs e)
        {
            GameSpine.Instance.Run();
        }
    }
}
