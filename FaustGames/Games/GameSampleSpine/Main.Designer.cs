namespace GameSampleSpine
{
    partial class Main
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
            this._jump = new System.Windows.Forms.Button();
            this._walk = new System.Windows.Forms.Button();
            this._run = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // _jump
            // 
            this._jump.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this._jump.Location = new System.Drawing.Point(12, 569);
            this._jump.Name = "_jump";
            this._jump.Size = new System.Drawing.Size(75, 23);
            this._jump.TabIndex = 0;
            this._jump.Text = "jump";
            this._jump.UseVisualStyleBackColor = true;
            this._jump.Click += new System.EventHandler(this._jump_Click);
            // 
            // _walk
            // 
            this._walk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this._walk.Location = new System.Drawing.Point(93, 569);
            this._walk.Name = "_walk";
            this._walk.Size = new System.Drawing.Size(75, 23);
            this._walk.TabIndex = 1;
            this._walk.Text = "walk";
            this._walk.UseVisualStyleBackColor = true;
            this._walk.Click += new System.EventHandler(this._walk_Click);
            // 
            // _run
            // 
            this._run.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this._run.Location = new System.Drawing.Point(174, 569);
            this._run.Name = "_run";
            this._run.Size = new System.Drawing.Size(75, 23);
            this._run.TabIndex = 2;
            this._run.Text = "run";
            this._run.UseVisualStyleBackColor = true;
            this._run.Click += new System.EventHandler(this._run_Click);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(860, 601);
            this.Controls.Add(this._run);
            this.Controls.Add(this._walk);
            this.Controls.Add(this._jump);
            this.Name = "Main";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button _jump;
        private System.Windows.Forms.Button _walk;
        private System.Windows.Forms.Button _run;

    }
}

