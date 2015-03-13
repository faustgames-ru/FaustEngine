namespace GameSampleWindowForms
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
            this.components = new System.ComponentModel.Container();
            this._timer = new System.Windows.Forms.Timer(this.components);
            this._renderRegion = new System.Windows.Forms.Panel();
            this._textureVelocity = new System.Windows.Forms.TrackBar();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this._wavesVelocity = new System.Windows.Forms.TrackBar();
            this.label3 = new System.Windows.Forms.Label();
            this._wavesScale = new System.Windows.Forms.TrackBar();
            this.label4 = new System.Windows.Forms.Label();
            this._normalScaleZ = new System.Windows.Forms.TrackBar();
            this.label5 = new System.Windows.Forms.Label();
            this._envOffsetY = new System.Windows.Forms.TrackBar();
            this.label6 = new System.Windows.Forms.Label();
            this._envScaleY = new System.Windows.Forms.TrackBar();
            ((System.ComponentModel.ISupportInitialize)(this._textureVelocity)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this._wavesVelocity)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this._wavesScale)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this._normalScaleZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this._envOffsetY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this._envScaleY)).BeginInit();
            this.SuspendLayout();
            // 
            // _timer
            // 
            this._timer.Enabled = true;
            this._timer.Interval = 10;
            this._timer.Tick += new System.EventHandler(this._timer_Tick);
            // 
            // _renderRegion
            // 
            this._renderRegion.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._renderRegion.Location = new System.Drawing.Point(194, 12);
            this._renderRegion.Name = "_renderRegion";
            this._renderRegion.Size = new System.Drawing.Size(738, 477);
            this._renderRegion.TabIndex = 0;
            // 
            // _textureVelocity
            // 
            this._textureVelocity.Location = new System.Drawing.Point(12, 28);
            this._textureVelocity.Maximum = 100;
            this._textureVelocity.Minimum = -100;
            this._textureVelocity.Name = "_textureVelocity";
            this._textureVelocity.Size = new System.Drawing.Size(169, 45);
            this._textureVelocity.TabIndex = 2;
            this._textureVelocity.TickFrequency = 10;
            this._textureVelocity.TickStyle = System.Windows.Forms.TickStyle.Both;
            this._textureVelocity.Value = -1;
            this._textureVelocity.ValueChanged += new System.EventHandler(this._textureVelocity_ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(78, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "texture velocity";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 76);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "waves velocity";
            // 
            // _wavesVelocity
            // 
            this._wavesVelocity.Location = new System.Drawing.Point(12, 92);
            this._wavesVelocity.Maximum = 100;
            this._wavesVelocity.Minimum = -100;
            this._wavesVelocity.Name = "_wavesVelocity";
            this._wavesVelocity.Size = new System.Drawing.Size(169, 45);
            this._wavesVelocity.TabIndex = 4;
            this._wavesVelocity.TickFrequency = 10;
            this._wavesVelocity.TickStyle = System.Windows.Forms.TickStyle.Both;
            this._wavesVelocity.Value = -1;
            this._wavesVelocity.ValueChanged += new System.EventHandler(this._wavesVelocity_ValueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 140);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(66, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "waves scale";
            // 
            // _wavesScale
            // 
            this._wavesScale.Location = new System.Drawing.Point(12, 156);
            this._wavesScale.Maximum = 500;
            this._wavesScale.Name = "_wavesScale";
            this._wavesScale.Size = new System.Drawing.Size(169, 45);
            this._wavesScale.TabIndex = 6;
            this._wavesScale.TickFrequency = 10;
            this._wavesScale.TickStyle = System.Windows.Forms.TickStyle.Both;
            this._wavesScale.ValueChanged += new System.EventHandler(this._wavesScale_ValueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 204);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(74, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "normal scale z";
            // 
            // _normalScaleZ
            // 
            this._normalScaleZ.Location = new System.Drawing.Point(12, 220);
            this._normalScaleZ.Maximum = 1000;
            this._normalScaleZ.Minimum = -1000;
            this._normalScaleZ.Name = "_normalScaleZ";
            this._normalScaleZ.Size = new System.Drawing.Size(169, 45);
            this._normalScaleZ.TabIndex = 8;
            this._normalScaleZ.TickFrequency = 100;
            this._normalScaleZ.TickStyle = System.Windows.Forms.TickStyle.Both;
            this._normalScaleZ.Value = -1;
            this._normalScaleZ.ValueChanged += new System.EventHandler(this._normalScaleZ_ValueChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 268);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(62, 13);
            this.label5.TabIndex = 11;
            this.label5.Text = "env offset y";
            // 
            // _envOffsetY
            // 
            this._envOffsetY.Location = new System.Drawing.Point(12, 284);
            this._envOffsetY.Maximum = 100;
            this._envOffsetY.Minimum = -100;
            this._envOffsetY.Name = "_envOffsetY";
            this._envOffsetY.Size = new System.Drawing.Size(169, 45);
            this._envOffsetY.TabIndex = 10;
            this._envOffsetY.TickFrequency = 10;
            this._envOffsetY.TickStyle = System.Windows.Forms.TickStyle.Both;
            this._envOffsetY.ValueChanged += new System.EventHandler(this._envOffsetY_ValueChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(12, 332);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(61, 13);
            this.label6.TabIndex = 13;
            this.label6.Text = "env scale y";
            // 
            // _envScaleY
            // 
            this._envScaleY.Location = new System.Drawing.Point(12, 348);
            this._envScaleY.Maximum = 100;
            this._envScaleY.Name = "_envScaleY";
            this._envScaleY.Size = new System.Drawing.Size(169, 45);
            this._envScaleY.TabIndex = 12;
            this._envScaleY.TickFrequency = 10;
            this._envScaleY.TickStyle = System.Windows.Forms.TickStyle.Both;
            this._envScaleY.ValueChanged += new System.EventHandler(this._envScaleY_ValueChanged);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(944, 501);
            this.Controls.Add(this.label6);
            this.Controls.Add(this._envScaleY);
            this.Controls.Add(this.label5);
            this.Controls.Add(this._envOffsetY);
            this.Controls.Add(this.label4);
            this.Controls.Add(this._normalScaleZ);
            this.Controls.Add(this.label3);
            this.Controls.Add(this._wavesScale);
            this.Controls.Add(this.label2);
            this.Controls.Add(this._wavesVelocity);
            this.Controls.Add(this.label1);
            this.Controls.Add(this._textureVelocity);
            this.Controls.Add(this._renderRegion);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Main";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Main_FormClosed);
            ((System.ComponentModel.ISupportInitialize)(this._textureVelocity)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this._wavesVelocity)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this._wavesScale)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this._normalScaleZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this._envOffsetY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this._envScaleY)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Timer _timer;
        private System.Windows.Forms.Panel _renderRegion;
        private System.Windows.Forms.TrackBar _textureVelocity;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TrackBar _wavesVelocity;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TrackBar _wavesScale;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TrackBar _normalScaleZ;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TrackBar _envOffsetY;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TrackBar _envScaleY;
    }
}

