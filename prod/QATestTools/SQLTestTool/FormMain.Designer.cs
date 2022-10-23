namespace SQLTestTool
{
    partial class FormMain
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
            this.m_textBoxLog = new System.Windows.Forms.TextBox();
            this.button_start = new System.Windows.Forms.Button();
            this.m_textBoxCfgFile = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // m_textBoxLog
            // 
            this.m_textBoxLog.Location = new System.Drawing.Point(12, 113);
            this.m_textBoxLog.Multiline = true;
            this.m_textBoxLog.Name = "m_textBoxLog";
            this.m_textBoxLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.m_textBoxLog.Size = new System.Drawing.Size(686, 304);
            this.m_textBoxLog.TabIndex = 0;
            // 
            // button_start
            // 
            this.button_start.Location = new System.Drawing.Point(609, 10);
            this.button_start.Name = "button_start";
            this.button_start.Size = new System.Drawing.Size(75, 23);
            this.button_start.TabIndex = 1;
            this.button_start.Text = "start";
            this.button_start.UseVisualStyleBackColor = true;
            this.button_start.Click += new System.EventHandler(this.button_start_Click);
            // 
            // m_textBoxCfgFile
            // 
            this.m_textBoxCfgFile.Location = new System.Drawing.Point(75, 12);
            this.m_textBoxCfgFile.Name = "m_textBoxCfgFile";
            this.m_textBoxCfgFile.Size = new System.Drawing.Size(517, 20);
            this.m_textBoxCfgFile.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(56, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Config file:";
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(710, 429);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.m_textBoxCfgFile);
            this.Controls.Add(this.button_start);
            this.Controls.Add(this.m_textBoxLog);
            this.Name = "FormMain";
            this.Text = "Form1";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FormMain_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox m_textBoxLog;
        private System.Windows.Forms.Button button_start;
        private System.Windows.Forms.TextBox m_textBoxCfgFile;
        private System.Windows.Forms.Label label1;
    }
}

