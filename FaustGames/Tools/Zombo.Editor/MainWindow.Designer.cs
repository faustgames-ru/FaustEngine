namespace Zombo.Editor
{
    partial class MainWindow
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainWindow));
            this._toolbox = new System.Windows.Forms.ToolStrip();
            this._editorModeSelect = new System.Windows.Forms.ToolStripButton();
            this._editorModeDrawCurve = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this._undo = new System.Windows.Forms.ToolStripButton();
            this._redo = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this._cameraModeMove = new System.Windows.Forms.ToolStripButton();
            this._cameraModeRotate = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this._comboFov = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripTextBox1 = new System.Windows.Forms.ToolStripTextBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this._zomboEditScene = new Zombo.Editor.Controls.ZomboEditSceneControl();
            this._toolbox.SuspendLayout();
            this.SuspendLayout();
            // 
            // _toolbox
            // 
            this._toolbox.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._editorModeSelect,
            this._editorModeDrawCurve,
            this.toolStripSeparator1,
            this._undo,
            this._redo,
            this.toolStripSeparator2,
            this._cameraModeMove,
            this._cameraModeRotate,
            this.toolStripSeparator3,
            this.toolStripLabel1,
            this._comboFov,
            this.toolStripSeparator4,
            this.toolStripTextBox1});
            this._toolbox.Location = new System.Drawing.Point(0, 0);
            this._toolbox.Name = "_toolbox";
            this._toolbox.Size = new System.Drawing.Size(747, 25);
            this._toolbox.TabIndex = 1;
            this._toolbox.Text = "toolStrip1";
            this._toolbox.Visible = false;
            // 
            // _editorModeSelect
            // 
            this._editorModeSelect.CheckOnClick = true;
            this._editorModeSelect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._editorModeSelect.Image = global::Zombo.Editor.Properties.Resources.selection;
            this._editorModeSelect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._editorModeSelect.Name = "_editorModeSelect";
            this._editorModeSelect.Size = new System.Drawing.Size(23, 22);
            this._editorModeSelect.Text = "Select";
            this._editorModeSelect.Visible = false;
            this._editorModeSelect.Click += new System.EventHandler(this.EditorModeSwitch);
            // 
            // _editorModeDrawCurve
            // 
            this._editorModeDrawCurve.CheckOnClick = true;
            this._editorModeDrawCurve.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._editorModeDrawCurve.Image = global::Zombo.Editor.Properties.Resources.curve;
            this._editorModeDrawCurve.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._editorModeDrawCurve.Name = "_editorModeDrawCurve";
            this._editorModeDrawCurve.Size = new System.Drawing.Size(23, 22);
            this._editorModeDrawCurve.Text = "Curve";
            this._editorModeDrawCurve.Visible = false;
            this._editorModeDrawCurve.Click += new System.EventHandler(this.EditorModeSwitch);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            this.toolStripSeparator1.Visible = false;
            // 
            // _undo
            // 
            this._undo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._undo.Image = global::Zombo.Editor.Properties.Resources.undo;
            this._undo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._undo.Name = "_undo";
            this._undo.Size = new System.Drawing.Size(23, 22);
            this._undo.Text = "Undo";
            this._undo.Click += new System.EventHandler(this._undo_Click);
            // 
            // _redo
            // 
            this._redo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._redo.Image = global::Zombo.Editor.Properties.Resources.redo;
            this._redo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._redo.Name = "_redo";
            this._redo.Size = new System.Drawing.Size(23, 22);
            this._redo.Text = "Redo";
            this._redo.Click += new System.EventHandler(this._redo_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            this.toolStripSeparator2.Visible = false;
            // 
            // _cameraModeMove
            // 
            this._cameraModeMove.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._cameraModeMove.Image = global::Zombo.Editor.Properties.Resources.hand;
            this._cameraModeMove.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._cameraModeMove.Name = "_cameraModeMove";
            this._cameraModeMove.Size = new System.Drawing.Size(23, 22);
            this._cameraModeMove.Text = "Move";
            this._cameraModeMove.Visible = false;
            this._cameraModeMove.Click += new System.EventHandler(this.CameraModeSwitch);
            // 
            // _cameraModeRotate
            // 
            this._cameraModeRotate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._cameraModeRotate.Image = global::Zombo.Editor.Properties.Resources.rot;
            this._cameraModeRotate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._cameraModeRotate.Name = "_cameraModeRotate";
            this._cameraModeRotate.Size = new System.Drawing.Size(23, 22);
            this._cameraModeRotate.Text = "Rotate";
            this._cameraModeRotate.Visible = false;
            this._cameraModeRotate.Click += new System.EventHandler(this.CameraModeSwitch);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(73, 22);
            this.toolStripLabel1.Text = "Field of view";
            // 
            // _comboFov
            // 
            this._comboFov.Items.AddRange(new object[] {
            "0",
            "10",
            "20",
            "40",
            "60",
            "90",
            "100",
            "120"});
            this._comboFov.Name = "_comboFov";
            this._comboFov.Size = new System.Drawing.Size(75, 25);
            this._comboFov.Text = "0";
            this._comboFov.TextChanged += new System.EventHandler(this._comboFov_TextChanged);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripTextBox1
            // 
            this.toolStripTextBox1.Name = "toolStripTextBox1";
            this.toolStripTextBox1.Size = new System.Drawing.Size(100, 25);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 412);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(747, 22);
            this.statusStrip1.TabIndex = 2;
            this.statusStrip1.Text = "statusStrip1";
            this.statusStrip1.Visible = false;
            // 
            // _zomboEditScene
            // 
            this._zomboEditScene.Dock = System.Windows.Forms.DockStyle.Fill;
            this._zomboEditScene.Location = new System.Drawing.Point(0, 0);
            this._zomboEditScene.Name = "_zomboEditScene";
            this._zomboEditScene.Size = new System.Drawing.Size(747, 412);
            this._zomboEditScene.TabIndex = 0;
            this._zomboEditScene.MouseDown += new System.Windows.Forms.MouseEventHandler(this._zomboEditScene_MouseDown);
            this._zomboEditScene.MouseUp += new System.Windows.Forms.MouseEventHandler(this._zomboEditScene_MouseUp);
            this._zomboEditScene.Move += new System.EventHandler(this._zomboEditScene_Move);
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(747, 434);
            this.Controls.Add(this._zomboEditScene);
            this.Controls.Add(this._toolbox);
            this.Controls.Add(this.statusStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainWindow";
            this.Text = "Zombo Editor";
            this.Load += new System.EventHandler(this.MainWindow_Load);
            this._toolbox.ResumeLayout(false);
            this._toolbox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Controls.ZomboEditSceneControl _zomboEditScene;
        private System.Windows.Forms.ToolStrip _toolbox;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripButton _editorModeSelect;
        private System.Windows.Forms.ToolStripButton _editorModeDrawCurve;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton _undo;
        private System.Windows.Forms.ToolStripButton _redo;
        private System.Windows.Forms.ToolStripButton _cameraModeRotate;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton _cameraModeMove;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripComboBox _comboFov;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripTextBox toolStripTextBox1;
    }
}

