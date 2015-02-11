namespace WindowsFormsApplication1
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.wsq = new System.Windows.Forms.Button();
            this.wsq_continue = new System.Windows.Forms.Button();
            this.btn_stop = new System.Windows.Forms.Button();
            this.CancelReq = new System.Windows.Forms.Button();
            this.wsq_continue_part = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(3, 87);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(741, 412);
            this.richTextBox1.TabIndex = 0;
            this.richTextBox1.Text = "";
            // 
            // wsq
            // 
            this.wsq.Location = new System.Drawing.Point(26, 13);
            this.wsq.Name = "wsq";
            this.wsq.Size = new System.Drawing.Size(75, 23);
            this.wsq.TabIndex = 1;
            this.wsq.Text = "快照WSQ";
            this.wsq.UseVisualStyleBackColor = true;
            this.wsq.Click += new System.EventHandler(this.wsq_Click);
            // 
            // wsq_continue
            // 
            this.wsq_continue.Location = new System.Drawing.Point(138, 12);
            this.wsq_continue.Name = "wsq_continue";
            this.wsq_continue.Size = new System.Drawing.Size(75, 23);
            this.wsq_continue.TabIndex = 2;
            this.wsq_continue.Text = "订阅WSQ";
            this.wsq_continue.UseVisualStyleBackColor = true;
            this.wsq_continue.Click += new System.EventHandler(this.wsq_continue_Click);
            // 
            // btn_stop
            // 
            this.btn_stop.Location = new System.Drawing.Point(635, 58);
            this.btn_stop.Name = "btn_stop";
            this.btn_stop.Size = new System.Drawing.Size(75, 23);
            this.btn_stop.TabIndex = 3;
            this.btn_stop.Text = "Stop";
            this.btn_stop.UseVisualStyleBackColor = true;
            this.btn_stop.Click += new System.EventHandler(this.btn_stop_Click);
            // 
            // CancelReq
            // 
            this.CancelReq.Location = new System.Drawing.Point(635, 13);
            this.CancelReq.Name = "CancelReq";
            this.CancelReq.Size = new System.Drawing.Size(75, 23);
            this.CancelReq.TabIndex = 4;
            this.CancelReq.Text = "取消订阅";
            this.CancelReq.UseVisualStyleBackColor = true;
            this.CancelReq.Click += new System.EventHandler(this.CancelReq_Click);
            // 
            // wsq_continue_part
            // 
            this.wsq_continue_part.Location = new System.Drawing.Point(239, 12);
            this.wsq_continue_part.Name = "wsq_continue_part";
            this.wsq_continue_part.Size = new System.Drawing.Size(85, 23);
            this.wsq_continue_part.TabIndex = 5;
            this.wsq_continue_part.Text = "订阅WSQ_部分";
            this.wsq_continue_part.UseVisualStyleBackColor = true;
            this.wsq_continue_part.Click += new System.EventHandler(this.wsq_continue_part_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(741, 503);
            this.Controls.Add(this.wsq_continue_part);
            this.Controls.Add(this.CancelReq);
            this.Controls.Add(this.btn_stop);
            this.Controls.Add(this.wsq_continue);
            this.Controls.Add(this.wsq);
            this.Controls.Add(this.richTextBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button wsq;
        private System.Windows.Forms.Button wsq_continue;
        private System.Windows.Forms.Button btn_stop;
        private System.Windows.Forms.Button CancelReq;
        private System.Windows.Forms.Button wsq_continue_part;
    }
}

