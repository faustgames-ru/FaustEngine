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
            InvalidateModeButtons();
            InvalidateCameraModeButtons();
            Application.Idle += ApplicationIdle;
            Closed += MainWindowClosed;
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
            }
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
            _undo.Enabled = _zomboEditScene.ZomboEditor.IsUndoAvaliable();
            _redo.Enabled = _zomboEditScene.ZomboEditor.IsRedoAvaliable();
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

        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            _zomboEditScene.ZomboCamera.SetFov((float)(trackBar1.Value * Math.PI / 180.0));
        }
    }
}
