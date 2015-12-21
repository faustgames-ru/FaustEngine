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
            this._zomboEditScene = new Zombo.Editor.Controls.ZomboEditSceneControl();
            this.SuspendLayout();
            // 
            // _zomboEditScene
            // 
            this._zomboEditScene.Dock = System.Windows.Forms.DockStyle.Fill;
            this._zomboEditScene.Location = new System.Drawing.Point(0, 0);
            this._zomboEditScene.Name = "_zomboEditScene";
            this._zomboEditScene.Size = new System.Drawing.Size(348, 321);
            this._zomboEditScene.TabIndex = 0;
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(348, 321);
            this.Controls.Add(this._zomboEditScene);
            this.Name = "MainWindow";
            this.ResumeLayout(false);

        }

        #endregion

        private Controls.ZomboEditSceneControl _zomboEditScene;
    }
}

