
namespace IPC_Installer
{
    partial class InstallerForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            button1 = new Button();
            SuspendLayout();
            // 
            // button1
            // 
            button1.BackColor = SystemColors.ActiveBorder;
            button1.FlatStyle = FlatStyle.Flat;
            button1.Location = new Point(12, 12);
            button1.MaximumSize = new Size(176, 60);
            button1.MinimumSize = new Size(176, 60);
            button1.Name = "button1";
            button1.Size = new Size(176, 60);
            button1.TabIndex = 0;
            button1.Text = "Install\r\n";
            button1.UseVisualStyleBackColor = false;
            button1.Click += InstallButton_Click;
            // 
            // Installer
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            BackColor = SystemColors.ControlText;
            ClientSize = new Size(206, 93);
            Controls.Add(button1);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            MaximumSize = new Size(224, 140);
            MinimumSize = new Size(224, 140);
            Name = "Installer";
            ShowIcon = false;
            StartPosition = FormStartPosition.CenterScreen;
            Load += Installer_Load;
            ResumeLayout(false);
        }

        private void InstallButton(object sender, EventArgs e)
        {
            throw new NotImplementedException();
        }

        #endregion

        private Button button1;
    }
}
