namespace RTCProgrammer
{
    partial class MainForm
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
            this.grnOnBtn = new System.Windows.Forms.Button();
            this.grnOffBtn = new System.Windows.Forms.Button();
            this.redOn = new System.Windows.Forms.Button();
            this.redOff = new System.Windows.Forms.Button();
            this.connectBtn = new System.Windows.Forms.Button();
            this.disconnectBtn = new System.Windows.Forms.Button();
            this.updateTime = new System.Windows.Forms.Button();
            this.addTimeBtn = new System.Windows.Forms.Button();
            this.clearEEPromBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // grnOnBtn
            // 
            this.grnOnBtn.Location = new System.Drawing.Point(676, 177);
            this.grnOnBtn.Name = "grnOnBtn";
            this.grnOnBtn.Size = new System.Drawing.Size(112, 39);
            this.grnOnBtn.TabIndex = 0;
            this.grnOnBtn.Text = "Green On";
            this.grnOnBtn.UseVisualStyleBackColor = true;
            this.grnOnBtn.Click += new System.EventHandler(this.grnOnBtn_Click);
            // 
            // grnOffBtn
            // 
            this.grnOffBtn.Location = new System.Drawing.Point(676, 234);
            this.grnOffBtn.Name = "grnOffBtn";
            this.grnOffBtn.Size = new System.Drawing.Size(112, 39);
            this.grnOffBtn.TabIndex = 1;
            this.grnOffBtn.Text = "Green Off";
            this.grnOffBtn.UseVisualStyleBackColor = true;
            this.grnOffBtn.Click += new System.EventHandler(this.grnOffBtn_Click);
            // 
            // redOn
            // 
            this.redOn.Location = new System.Drawing.Point(676, 299);
            this.redOn.Name = "redOn";
            this.redOn.Size = new System.Drawing.Size(112, 39);
            this.redOn.TabIndex = 2;
            this.redOn.Text = "Red On";
            this.redOn.UseVisualStyleBackColor = true;
            this.redOn.Click += new System.EventHandler(this.redOn_Click);
            // 
            // redOff
            // 
            this.redOff.Location = new System.Drawing.Point(676, 356);
            this.redOff.Name = "redOff";
            this.redOff.Size = new System.Drawing.Size(112, 39);
            this.redOff.TabIndex = 3;
            this.redOff.Text = "Red Off";
            this.redOff.UseVisualStyleBackColor = true;
            this.redOff.Click += new System.EventHandler(this.redOff_Click);
            // 
            // connectBtn
            // 
            this.connectBtn.Location = new System.Drawing.Point(512, 399);
            this.connectBtn.Name = "connectBtn";
            this.connectBtn.Size = new System.Drawing.Size(112, 39);
            this.connectBtn.TabIndex = 4;
            this.connectBtn.Text = "Connect";
            this.connectBtn.UseVisualStyleBackColor = true;
            this.connectBtn.Click += new System.EventHandler(this.connectBtn_Click);
            // 
            // disconnectBtn
            // 
            this.disconnectBtn.Location = new System.Drawing.Point(676, 401);
            this.disconnectBtn.Name = "disconnectBtn";
            this.disconnectBtn.Size = new System.Drawing.Size(112, 39);
            this.disconnectBtn.TabIndex = 5;
            this.disconnectBtn.Text = "Diconnect";
            this.disconnectBtn.UseVisualStyleBackColor = true;
            this.disconnectBtn.Click += new System.EventHandler(this.disconnectBtn_Click);
            // 
            // updateTime
            // 
            this.updateTime.Location = new System.Drawing.Point(12, 189);
            this.updateTime.Name = "updateTime";
            this.updateTime.Size = new System.Drawing.Size(112, 39);
            this.updateTime.TabIndex = 6;
            this.updateTime.Text = "Update Time";
            this.updateTime.UseVisualStyleBackColor = true;
            this.updateTime.Click += new System.EventHandler(this.updateTime_Click);
            // 
            // addTimeBtn
            // 
            this.addTimeBtn.Location = new System.Drawing.Point(279, 399);
            this.addTimeBtn.Name = "addTimeBtn";
            this.addTimeBtn.Size = new System.Drawing.Size(112, 39);
            this.addTimeBtn.TabIndex = 7;
            this.addTimeBtn.Text = "Add Time";
            this.addTimeBtn.UseVisualStyleBackColor = true;
            this.addTimeBtn.Click += new System.EventHandler(this.addTimeBtn_Click);
            // 
            // clearEEPromBtn
            // 
            this.clearEEPromBtn.Location = new System.Drawing.Point(87, 401);
            this.clearEEPromBtn.Name = "clearEEPromBtn";
            this.clearEEPromBtn.Size = new System.Drawing.Size(112, 39);
            this.clearEEPromBtn.TabIndex = 8;
            this.clearEEPromBtn.Text = "Clear All";
            this.clearEEPromBtn.UseVisualStyleBackColor = true;
            this.clearEEPromBtn.Click += new System.EventHandler(this.clearEEPromBtn_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.clearEEPromBtn);
            this.Controls.Add(this.addTimeBtn);
            this.Controls.Add(this.updateTime);
            this.Controls.Add(this.disconnectBtn);
            this.Controls.Add(this.connectBtn);
            this.Controls.Add(this.redOff);
            this.Controls.Add(this.redOn);
            this.Controls.Add(this.grnOffBtn);
            this.Controls.Add(this.grnOnBtn);
            this.Name = "MainForm";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button grnOnBtn;
        private System.Windows.Forms.Button grnOffBtn;
        private System.Windows.Forms.Button redOn;
        private System.Windows.Forms.Button redOff;
        private System.Windows.Forms.Button connectBtn;
        private System.Windows.Forms.Button disconnectBtn;
        private System.Windows.Forms.Button updateTime;
        private System.Windows.Forms.Button addTimeBtn;
        private System.Windows.Forms.Button clearEEPromBtn;
    }
}

