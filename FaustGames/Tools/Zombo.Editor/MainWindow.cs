using System;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;
using Zombo.GraphicsBinding;

namespace Zombo.Editor
{
    public partial class MainWindow : Form
    {
        const string EditorModeToolStripButtonName = "_editorMode";
        const string CameraModeToolStripButtonName = "_cameraMode";

        public MainWindow()
        {
            InitializeComponent();
            Application.Idle += ApplicationIdle;
            Closed += MainWindowClosed;
        }

        public static Control FindFocusedControl(Control control)
        {
            var container = control as ContainerControl;
            return (null != container
                ? FindFocusedControl(container.ActiveControl)
                : control);
        }

        public static string GetControlPath(Control control)
        {
            var path = control.Name;
            var parent = control.Parent;
            while (parent != null)
            {
                path = parent.Name + "/" + path;
                parent = parent.Parent;
            }
            return path;
        }

        private static ToolStripControlHost Find(Control c)
        {
            var p = c.Parent;
            while (p != null)
            {
                if (p is ToolStrip)
                    break;
                p = p.Parent;
            }
            if (p == null)
                return null;

            ToolStrip ts = (ToolStrip)p;
            foreach (ToolStripItem i in ts.Items)
            {
                var h = Find(i, c);
                if (h != null)
                    return h;
            }
            return null;
        }

        private static ToolStripControlHost Find(ToolStripItem item, Control c)
        {
            ToolStripControlHost result = null;
            if (item is ToolStripControlHost)
            {
                var h = (ToolStripControlHost)item;
                if (h.Control == c)
                {
                    result = h;
                }
            }
            else if (item is ToolStripDropDownItem)
            {
                var ddm = (ToolStripDropDownItem)item;
                foreach (ToolStripItem i in ddm.DropDown.Items)
                {
                    result = Find(i, c);
                    if (result != null)
                        break;
                }
            }
            return result;
        }

        public static ToolStripControlHost FindHost(Control control)
        {
            return Find(control);
        }

        public static bool IsControlTextEdit(Control control)
        {
            if (control is TextBox) return true;
            var host = FindHost(control);
            if (host is ToolStripTextBox) return true;
            return false;

        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            var focused = FindFocusedControl(this);
            if (focused != null)
            {
                if (IsControlTextEdit(focused))
                {
                    return base.ProcessCmdKey(ref msg, keyData);
                }
            }
            switch (keyData)
            {
                case (Keys.Control | Keys.Z):
                    _undo.PerformClick();
                    break;
                case (Keys.Control | Keys.Shift | Keys.Z):
                    _redo.PerformClick();
                    break;
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void InvalidateComboFov()
        {
            var fov = (int)Math.Round(_zomboEditScene.ZomboCamera.GetFov() * 180.0f / Math.PI);
            var newValue = $"{fov}";
            if (_comboFov.Text == newValue) return;
            _comboFov.TextChanged -= _comboFov_TextChanged;
            _comboFov.Text = newValue;
            _comboFov.TextChanged += _comboFov_TextChanged;
        }

        private void MainWindowClosed(object sender, EventArgs e)
        {
            Application.Idle -= ApplicationIdle;
        }

        bool IsApplicationIdle()
        {
            NativeMessage result;
            return WinApi.PeekMessage(out result, IntPtr.Zero, (uint)0, (uint)0, (uint)0) == 0;
        }

        private void ApplicationIdle(object sender, EventArgs e)
        {
            while (IsApplicationIdle())
            {
                _zomboEditScene.InternalCallUpdateAndRender();
                InvalidateUndoButtons();
                InvalidateCameraModeButtons();
                InvalidateComboFov();
            }
        }

        private void InvalidateUndoButtons()
        {
            var undo = _zomboEditScene.ZomboEditor.IsUndoAvaliable();
            _undo.Enabled = undo;
            var redo = _zomboEditScene.ZomboEditor.IsRedoAvaliable();
            _redo.Enabled = redo;
        }

        private void InvalidateCameraModeButtons()
        {
            var mode = Marshal.PtrToStringAnsi(_zomboEditScene.ZomboCamera.GetMode());
            foreach (var toolButton in _toolbox.Items.OfType<ToolStripButton>().Where(tb => tb.Name.StartsWith(CameraModeToolStripButtonName)))
            {
                toolButton.Checked = toolButton.Name == CameraModeToolStripButtonName + mode;
            }
        }

        private void InvalidateModeButtons()
        {
            var mode = Marshal.PtrToStringAnsi(_zomboEditScene.ZomboEditor.GetMode());
            foreach (var toolButton in _toolbox.Items.OfType<ToolStripButton>().Where(tb => tb.Name.StartsWith(EditorModeToolStripButtonName)))
            {
                toolButton.Checked = toolButton.Name == EditorModeToolStripButtonName + mode;
            }
            InvalidateUndoButtons();
        }

        private void CameraModeSwitch(object sender, EventArgs e)
        {
            var button = sender as ToolStripButton;
            if (button == null)
                return;
            var modeName = button.Name.Replace(CameraModeToolStripButtonName, string.Empty);
            _zomboEditScene.ZomboCamera.SetMode(modeName);
            InvalidateCameraModeButtons();            
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

        private void _comboFov_TextChanged(object sender, EventArgs e)
        {
            int value;
            if (int.TryParse(_comboFov.Text, out value))
            {
                _zomboEditScene.ZomboCamera.SetFov((float)(value * Math.PI / 180.0f));
            }
            else
            {
                InvalidateComboFov();
            }
        }

        private void MainWindow_Load(object sender, EventArgs e)
        {
            InvalidateModeButtons();
            InvalidateCameraModeButtons();
            InvalidateComboFov();
        }

        private void _zomboEditScene_MouseDown(object sender, MouseEventArgs e)
        {
        }

        private void _zomboEditScene_MouseUp(object sender, MouseEventArgs e)
        {
        }

        private void _zomboEditScene_Move(object sender, EventArgs e)
        {
        }

        private void MainWindow_FormClosed(object sender, FormClosedEventArgs e)
        {
            _zomboEditScene.ZomboEditor.Finish();
        }
    }
}
