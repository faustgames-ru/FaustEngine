using System;
using System.Linq;
using System.Windows.Forms;

namespace Zombo.Editor
{
    public partial class MainWindow : Form
    {
        const string EditorModeToolStripButtonName = "_editorMode";

        public MainWindow()
        {
            InitializeComponent();
        }

        private void EditorModeSwitch(object sender, EventArgs e)
        {
            foreach (var toolButton in _toolbox.Items.OfType<ToolStripButton>().Where(tb=>tb.Name.StartsWith(EditorModeToolStripButtonName)))
            {
                if (toolButton != sender)
                {
                    toolButton.Checked = false;
                }
                else
                {
                    toolButton.Checked = true;
                }
            }
            
        }

    }
}
