namespace WallpapersWindowsClient
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
            this._renderPanel = new System.Windows.Forms.Panel();
            this._instrumentsPanel = new System.Windows.Forms.Panel();
            this.splitter1 = new System.Windows.Forms.Splitter();
            this.SuspendLayout();
            // 
            // _renderPanel
            // 
            this._renderPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this._renderPanel.Location = new System.Drawing.Point(239, 0);
            this._renderPanel.Name = "_renderPanel";
            this._renderPanel.Size = new System.Drawing.Size(632, 413);
            this._renderPanel.TabIndex = 0;
            // 
            // _instrumentsPanel
            // 
            this._instrumentsPanel.Dock = System.Windows.Forms.DockStyle.Left;
            this._instrumentsPanel.Location = new System.Drawing.Point(0, 0);
            this._instrumentsPanel.Name = "_instrumentsPanel";
            this._instrumentsPanel.Size = new System.Drawing.Size(239, 413);
            this._instrumentsPanel.TabIndex = 1;
            // 
            // splitter1
            // 
            this.splitter1.BackColor = System.Drawing.SystemColors.ControlDark;
            this.splitter1.Location = new System.Drawing.Point(239, 0);
            this.splitter1.Name = "splitter1";
            this.splitter1.Size = new System.Drawing.Size(3, 413);
            this.splitter1.TabIndex = 2;
            this.splitter1.TabStop = false;
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(871, 413);
            this.Controls.Add(this.splitter1);
            this.Controls.Add(this._renderPanel);
            this.Controls.Add(this._instrumentsPanel);
            this.Name = "MainWindow";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel _renderPanel;
        private System.Windows.Forms.Panel _instrumentsPanel;
        private System.Windows.Forms.Splitter splitter1;
    }
}

