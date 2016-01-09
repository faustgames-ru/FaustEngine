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
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.panel1 = new System.Windows.Forms.Panel();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this._editorModeSelect = new System.Windows.Forms.ToolStripButton();
            this._editorModeDrawCurve = new System.Windows.Forms.ToolStripButton();
            this._undo = new System.Windows.Forms.ToolStripButton();
            this._redo = new System.Windows.Forms.ToolStripButton();
            this._cameraModeMove = new System.Windows.Forms.ToolStripButton();
            this._cameraModeRotate = new System.Windows.Forms.ToolStripButton();
            this._zomboEditScene = new Zombo.Editor.Controls.ZomboEditSceneControl();
            this._toolbox.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
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
            this._cameraModeRotate});
            this._toolbox.Location = new System.Drawing.Point(0, 0);
            this._toolbox.Name = "_toolbox";
            this._toolbox.Size = new System.Drawing.Size(821, 25);
            this._toolbox.TabIndex = 1;
            this._toolbox.Text = "toolStrip1";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 435);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(821, 22);
            this.statusStrip1.TabIndex = 2;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.trackBar1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 366);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(821, 69);
            this.panel1.TabIndex = 3;
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(12, 12);
            this.trackBar1.Maximum = 170;
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(780, 45);
            this.trackBar1.TabIndex = 0;
            this.trackBar1.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
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
            this._editorModeDrawCurve.Click += new System.EventHandler(this.EditorModeSwitch);
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
            // _cameraModeMove
            // 
            this._cameraModeMove.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._cameraModeMove.Image = global::Zombo.Editor.Properties.Resources.hand;
            this._cameraModeMove.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._cameraModeMove.Name = "_cameraModeMove";
            this._cameraModeMove.Size = new System.Drawing.Size(23, 22);
            this._cameraModeMove.Text = "Move";
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
            this._cameraModeRotate.Click += new System.EventHandler(this.CameraModeSwitch);
            // 
            // _zomboEditScene
            // 
            this._zomboEditScene.Dock = System.Windows.Forms.DockStyle.Fill;
            this._zomboEditScene.Location = new System.Drawing.Point(0, 25);
            this._zomboEditScene.Name = "_zomboEditScene";
            this._zomboEditScene.Size = new System.Drawing.Size(821, 341);
            this._zomboEditScene.TabIndex = 0;
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(821, 457);
            this.Controls.Add(this._zomboEditScene);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this._toolbox);
            this.Controls.Add(this.statusStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainWindow";
            this.Text = "Zombo Editor";
            this._toolbox.ResumeLayout(false);
            this._toolbox.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
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
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton _cameraModeMove;
    }
}

