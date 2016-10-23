namespace TestColorReplacment
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
            this._hair = new System.Windows.Forms.Button();
            this._arms = new System.Windows.Forms.Button();
            this._hands = new System.Windows.Forms.Button();
            this._thighs = new System.Windows.Forms.Button();
            this._foots = new System.Windows.Forms.Button();
            this._weapon = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // _hair
            // 
            this._hair.Location = new System.Drawing.Point(76, 12);
            this._hair.Name = "_hair";
            this._hair.Size = new System.Drawing.Size(75, 23);
            this._hair.TabIndex = 0;
            this._hair.Tag = "0";
            this._hair.UseVisualStyleBackColor = true;
            this._hair.Click += new System.EventHandler(this._hair_Click);
            // 
            // _arms
            // 
            this._arms.Location = new System.Drawing.Point(76, 41);
            this._arms.Name = "_arms";
            this._arms.Size = new System.Drawing.Size(75, 23);
            this._arms.TabIndex = 1;
            this._arms.Tag = "0";
            this._arms.UseVisualStyleBackColor = true;
            this._arms.Click += new System.EventHandler(this._arms_Click);
            // 
            // _hands
            // 
            this._hands.Location = new System.Drawing.Point(76, 99);
            this._hands.Name = "_hands";
            this._hands.Size = new System.Drawing.Size(75, 23);
            this._hands.TabIndex = 2;
            this._hands.Tag = "0";
            this._hands.UseVisualStyleBackColor = true;
            this._hands.Click += new System.EventHandler(this._hands_Click);
            // 
            // _thighs
            // 
            this._thighs.Location = new System.Drawing.Point(76, 70);
            this._thighs.Name = "_thighs";
            this._thighs.Size = new System.Drawing.Size(75, 23);
            this._thighs.TabIndex = 3;
            this._thighs.Tag = "0";
            this._thighs.UseVisualStyleBackColor = true;
            this._thighs.Click += new System.EventHandler(this._thighs_Click);
            // 
            // _foots
            // 
            this._foots.Location = new System.Drawing.Point(76, 128);
            this._foots.Name = "_foots";
            this._foots.Size = new System.Drawing.Size(75, 23);
            this._foots.TabIndex = 4;
            this._foots.Tag = "0";
            this._foots.UseVisualStyleBackColor = true;
            this._foots.Click += new System.EventHandler(this._foots_Click);
            // 
            // _weapon
            // 
            this._weapon.Location = new System.Drawing.Point(76, 157);
            this._weapon.Name = "_weapon";
            this._weapon.Size = new System.Drawing.Size(75, 23);
            this._weapon.TabIndex = 5;
            this._weapon.Tag = "0";
            this._weapon.UseVisualStyleBackColor = true;
            this._weapon.Click += new System.EventHandler(this._weapon_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(26, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Hair";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 46);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(30, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "Arms";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 75);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(38, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Hands";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 104);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(39, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "Thighs";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 133);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(33, 13);
            this.label5.TabIndex = 10;
            this.label5.Text = "Foots";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(12, 162);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(48, 13);
            this.label6.TabIndex = 11;
            this.label6.Text = "Weapon";
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(793, 463);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this._weapon);
            this.Controls.Add(this._foots);
            this.Controls.Add(this._thighs);
            this.Controls.Add(this._hands);
            this.Controls.Add(this._arms);
            this.Controls.Add(this._hair);
            this.Name = "MainWindow";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button _hair;
        private System.Windows.Forms.Button _arms;
        private System.Windows.Forms.Button _hands;
        private System.Windows.Forms.Button _thighs;
        private System.Windows.Forms.Button _foots;
        private System.Windows.Forms.Button _weapon;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
    }
}

