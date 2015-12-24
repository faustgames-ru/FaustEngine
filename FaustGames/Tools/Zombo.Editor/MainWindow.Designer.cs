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
            this.toolStripSeparator1});
            this._toolbox.Location = new System.Drawing.Point(0, 0);
            this._toolbox.Name = "_toolbox";
            this._toolbox.Size = new System.Drawing.Size(875, 25);
            this._toolbox.TabIndex = 1;
            this._toolbox.Text = "toolStrip1";
            // 
            // _editorModeSelect
            // 
            this._editorModeSelect.CheckOnClick = true;
            this._editorModeSelect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._editorModeSelect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._editorModeSelect.Name = "_editorModeSelect";
            this._editorModeSelect.Size = new System.Drawing.Size(23, 22);
            this._editorModeSelect.Text = "toolStripButton1";
            this._editorModeSelect.Click += new System.EventHandler(this.EditorModeSwitch);
            // 
            // _editorModeDrawCurve
            // 
            this._editorModeDrawCurve.CheckOnClick = true;
            this._editorModeDrawCurve.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._editorModeDrawCurve.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._editorModeDrawCurve.Name = "_editorModeDrawCurve";
            this._editorModeDrawCurve.Size = new System.Drawing.Size(23, 22);
            this._editorModeDrawCurve.Text = "toolStripButton2";
            this._editorModeDrawCurve.Click += new System.EventHandler(this.EditorModeSwitch);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 411);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(875, 22);
            this.statusStrip1.TabIndex = 2;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // _zomboEditScene
            // 
            this._zomboEditScene.Dock = System.Windows.Forms.DockStyle.Fill;
            this._zomboEditScene.Location = new System.Drawing.Point(0, 25);
            this._zomboEditScene.Name = "_zomboEditScene";
            this._zomboEditScene.Size = new System.Drawing.Size(875, 386);
            this._zomboEditScene.TabIndex = 0;
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(875, 433);
            this.Controls.Add(this._zomboEditScene);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this._toolbox);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainWindow";
            this.Text = "Zombo Editor";
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
    }
}

