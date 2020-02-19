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
            this.label1 = new System.Windows.Forms.Label();
            this.portsComboBox = new System.Windows.Forms.ComboBox();
            this.consoleBox = new System.Windows.Forms.ListBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.ledColor = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.startMinute = new System.Windows.Forms.NumericUpDown();
            this.endMinute = new System.Windows.Forms.NumericUpDown();
            this.startHour = new System.Windows.Forms.NumericUpDown();
            this.endHour = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.startMinute)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.endMinute)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.startHour)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.endHour)).BeginInit();
            this.SuspendLayout();
            // 
            // grnOnBtn
            // 
            this.grnOnBtn.Location = new System.Drawing.Point(111, 102);
            this.grnOnBtn.Name = "grnOnBtn";
            this.grnOnBtn.Size = new System.Drawing.Size(93, 26);
            this.grnOnBtn.TabIndex = 0;
            this.grnOnBtn.Text = "Green On";
            this.grnOnBtn.UseVisualStyleBackColor = true;
            this.grnOnBtn.Click += new System.EventHandler(this.grnOnBtn_Click);
            // 
            // grnOffBtn
            // 
            this.grnOffBtn.Location = new System.Drawing.Point(213, 102);
            this.grnOffBtn.Name = "grnOffBtn";
            this.grnOffBtn.Size = new System.Drawing.Size(93, 26);
            this.grnOffBtn.TabIndex = 1;
            this.grnOffBtn.Text = "Green Off";
            this.grnOffBtn.UseVisualStyleBackColor = true;
            this.grnOffBtn.Click += new System.EventHandler(this.grnOffBtn_Click);
            // 
            // redOn
            // 
            this.redOn.Location = new System.Drawing.Point(111, 134);
            this.redOn.Name = "redOn";
            this.redOn.Size = new System.Drawing.Size(93, 26);
            this.redOn.TabIndex = 2;
            this.redOn.Text = "Red On";
            this.redOn.UseVisualStyleBackColor = true;
            this.redOn.Click += new System.EventHandler(this.redOn_Click);
            // 
            // redOff
            // 
            this.redOff.Location = new System.Drawing.Point(213, 134);
            this.redOff.Name = "redOff";
            this.redOff.Size = new System.Drawing.Size(93, 26);
            this.redOff.TabIndex = 3;
            this.redOff.Text = "Red Off";
            this.redOff.UseVisualStyleBackColor = true;
            this.redOff.Click += new System.EventHandler(this.redOff_Click);
            // 
            // connectBtn
            // 
            this.connectBtn.Location = new System.Drawing.Point(195, 26);
            this.connectBtn.Name = "connectBtn";
            this.connectBtn.Size = new System.Drawing.Size(93, 26);
            this.connectBtn.TabIndex = 4;
            this.connectBtn.Text = "Connect";
            this.connectBtn.UseVisualStyleBackColor = true;
            this.connectBtn.Click += new System.EventHandler(this.connectBtn_Click);
            // 
            // disconnectBtn
            // 
            this.disconnectBtn.Location = new System.Drawing.Point(309, 25);
            this.disconnectBtn.Name = "disconnectBtn";
            this.disconnectBtn.Size = new System.Drawing.Size(93, 26);
            this.disconnectBtn.TabIndex = 5;
            this.disconnectBtn.Text = "Disconnect";
            this.disconnectBtn.UseVisualStyleBackColor = true;
            this.disconnectBtn.Click += new System.EventHandler(this.disconnectBtn_Click);
            // 
            // updateTime
            // 
            this.updateTime.Location = new System.Drawing.Point(12, 102);
            this.updateTime.Name = "updateTime";
            this.updateTime.Size = new System.Drawing.Size(93, 26);
            this.updateTime.TabIndex = 6;
            this.updateTime.Text = "Update Time";
            this.updateTime.UseVisualStyleBackColor = true;
            this.updateTime.Click += new System.EventHandler(this.updateTime_Click);
            // 
            // addTimeBtn
            // 
            this.addTimeBtn.Location = new System.Drawing.Point(267, 269);
            this.addTimeBtn.Name = "addTimeBtn";
            this.addTimeBtn.Size = new System.Drawing.Size(93, 26);
            this.addTimeBtn.TabIndex = 7;
            this.addTimeBtn.Text = "Add Event";
            this.addTimeBtn.UseVisualStyleBackColor = true;
            this.addTimeBtn.Click += new System.EventHandler(this.addTimeBtn_Click);
            // 
            // clearEEPromBtn
            // 
            this.clearEEPromBtn.Location = new System.Drawing.Point(12, 328);
            this.clearEEPromBtn.Name = "clearEEPromBtn";
            this.clearEEPromBtn.Size = new System.Drawing.Size(93, 26);
            this.clearEEPromBtn.TabIndex = 8;
            this.clearEEPromBtn.Text = "Clear All";
            this.clearEEPromBtn.UseVisualStyleBackColor = true;
            this.clearEEPromBtn.Click += new System.EventHandler(this.clearEEPromBtn_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 33);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(56, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "COM Port:";
            // 
            // portsComboBox
            // 
            this.portsComboBox.FormattingEnabled = true;
            this.portsComboBox.Location = new System.Drawing.Point(78, 29);
            this.portsComboBox.Name = "portsComboBox";
            this.portsComboBox.Size = new System.Drawing.Size(100, 21);
            this.portsComboBox.TabIndex = 10;
            // 
            // consoleBox
            // 
            this.consoleBox.FormattingEnabled = true;
            this.consoleBox.Location = new System.Drawing.Point(13, 382);
            this.consoleBox.Name = "consoleBox";
            this.consoleBox.Size = new System.Drawing.Size(342, 56);
            this.consoleBox.TabIndex = 11;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(12, 366);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 13);
            this.label2.TabIndex = 12;
            this.label2.Text = "Console:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(9, 83);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(82, 13);
            this.label3.TabIndex = 13;
            this.label3.Text = "Basic Functions";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(9, 173);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(77, 13);
            this.label4.TabIndex = 14;
            this.label4.Text = "Time Events";
            // 
            // ledColor
            // 
            this.ledColor.FormattingEnabled = true;
            this.ledColor.Items.AddRange(new object[] {
            "Green",
            "Red"});
            this.ledColor.Location = new System.Drawing.Point(87, 209);
            this.ledColor.Name = "ledColor";
            this.ledColor.Size = new System.Drawing.Size(76, 21);
            this.ledColor.TabIndex = 16;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(13, 212);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(55, 13);
            this.label6.TabIndex = 17;
            this.label6.Text = "LED Color";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(10, 242);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(55, 13);
            this.label7.TabIndex = 19;
            this.label7.Text = "Start Time";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(10, 276);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(52, 13);
            this.label5.TabIndex = 23;
            this.label5.Text = "End Time";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(169, 242);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(10, 13);
            this.label8.TabIndex = 24;
            this.label8.Text = ":";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(169, 276);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(10, 13);
            this.label9.TabIndex = 25;
            this.label9.Text = ":";
            // 
            // startMinute
            // 
            this.startMinute.Location = new System.Drawing.Point(185, 239);
            this.startMinute.Maximum = new decimal(new int[] {
            59,
            0,
            0,
            0});
            this.startMinute.Name = "startMinute";
            this.startMinute.Size = new System.Drawing.Size(76, 20);
            this.startMinute.TabIndex = 26;
            // 
            // endMinute
            // 
            this.endMinute.Location = new System.Drawing.Point(185, 274);
            this.endMinute.Maximum = new decimal(new int[] {
            59,
            0,
            0,
            0});
            this.endMinute.Name = "endMinute";
            this.endMinute.Size = new System.Drawing.Size(76, 20);
            this.endMinute.TabIndex = 27;
            // 
            // startHour
            // 
            this.startHour.Location = new System.Drawing.Point(87, 239);
            this.startHour.Maximum = new decimal(new int[] {
            24,
            0,
            0,
            0});
            this.startHour.Name = "startHour";
            this.startHour.Size = new System.Drawing.Size(76, 20);
            this.startHour.TabIndex = 28;
            this.startHour.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // endHour
            // 
            this.endHour.Location = new System.Drawing.Point(87, 274);
            this.endHour.Maximum = new decimal(new int[] {
            24,
            0,
            0,
            0});
            this.endHour.Name = "endHour";
            this.endHour.Size = new System.Drawing.Size(76, 20);
            this.endHour.TabIndex = 29;
            this.endHour.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(419, 450);
            this.Controls.Add(this.endHour);
            this.Controls.Add(this.startHour);
            this.Controls.Add(this.endMinute);
            this.Controls.Add(this.startMinute);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.ledColor);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.consoleBox);
            this.Controls.Add(this.portsComboBox);
            this.Controls.Add(this.label1);
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
            this.Text = "Timer Interface";
            this.Load += new System.EventHandler(this.MainForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.startMinute)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.endMinute)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.startHour)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.endHour)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

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
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox portsComboBox;
        private System.Windows.Forms.ListBox consoleBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox ledColor;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.NumericUpDown startMinute;
        private System.Windows.Forms.NumericUpDown endMinute;
        private System.Windows.Forms.NumericUpDown startHour;
        private System.Windows.Forms.NumericUpDown endHour;
    }
}

