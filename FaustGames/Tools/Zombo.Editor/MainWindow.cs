﻿using System;
using System.Linq;
using System.Runtime.InteropServices;
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

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
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
            _comboFov.TextChanged -= _comboFov_TextChanged;
            var fov = (int)Math.Round(_zomboEditScene.ZomboCamera.GetFov() * 180.0f / Math.PI);
            _comboFov.Text = $"{fov}";
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
            }
        }

        private void InvalidateUndoButtons()
        {
            _undo.Enabled = _zomboEditScene.ZomboEditor.IsUndoAvaliable();
            _redo.Enabled = _zomboEditScene.ZomboEditor.IsRedoAvaliable();
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
    }
}
