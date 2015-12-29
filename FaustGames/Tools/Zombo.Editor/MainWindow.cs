using System;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Zombo.Editor
{
    public partial class MainWindow : Form
    {
        const string EditorModeToolStripButtonName = "_editorMode";

        public MainWindow()
        {
            InitializeComponent();
            InvalidateModeButtons();
        }

        private void InvalidateModeButtons()
        {
            var mode = Marshal.PtrToStringAnsi(_zomboEditScene.ZomboEditor.GetMode());
            foreach (var toolButton in _toolbox.Items.OfType<ToolStripButton>().Where(tb => tb.Name.StartsWith(EditorModeToolStripButtonName)))
            {
                toolButton.Checked = toolButton.Name == EditorModeToolStripButtonName + mode;
            }
            _undo.Enabled = _zomboEditScene.ZomboEditor.IsUndoAvaliable();
            _redo.Enabled = _zomboEditScene.ZomboEditor.IsRedoAvaliable();
        }

        private void EditorModeSwitch(object sender, EventArgs e)
        {
            var button = sender as ToolStripButton;
            if (button == null)
                return;
            var modeName = button.Name.Replace(EditorModeToolStripButtonName, string.Empty);
            _zomboEditScene.ZomboEditor.SetMode(modeName);
            InvalidateModeButtons();
        }

        private void _undo_Click(object sender, EventArgs e)
        {
            _zomboEditScene.ZomboEditor.Undo();
            InvalidateModeButtons();
        }

        private void _redo_Click(object sender, EventArgs e)
        {
            _zomboEditScene.ZomboEditor.Redo();
            InvalidateModeButtons();
        }
    }
}
