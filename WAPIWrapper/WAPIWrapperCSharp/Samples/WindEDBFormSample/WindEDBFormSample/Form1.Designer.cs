namespace WindEDBFormSample
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            this.Excute = new System.Windows.Forms.Button();
            this.startDateTimePicker = new System.Windows.Forms.DateTimePicker();
            this.endDateTimePicker = new System.Windows.Forms.DateTimePicker();
            this.myChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.radioButton_KQ = new System.Windows.Forms.RadioButton();
            this.radioButton_GD = new System.Windows.Forms.RadioButton();
            this.label = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.myChart)).BeginInit();
            this.SuspendLayout();
            // 
            // Excute
            // 
            this.Excute.Location = new System.Drawing.Point(595, 29);
            this.Excute.Name = "Excute";
            this.Excute.Size = new System.Drawing.Size(75, 23);
            this.Excute.TabIndex = 0;
            this.Excute.Text = "执行";
            this.Excute.UseVisualStyleBackColor = true;
            this.Excute.Click += new System.EventHandler(this.Excute_Click);
            // 
            // startDateTimePicker
            // 
            this.startDateTimePicker.CustomFormat = "yyyy-MM-dd";
            this.startDateTimePicker.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.startDateTimePicker.Location = new System.Drawing.Point(137, 29);
            this.startDateTimePicker.MinDate = new System.DateTime(2000, 1, 1, 0, 0, 0, 0);
            this.startDateTimePicker.Name = "startDateTimePicker";
            this.startDateTimePicker.Size = new System.Drawing.Size(98, 21);
            this.startDateTimePicker.TabIndex = 1;
            this.startDateTimePicker.Value = new System.DateTime(2014, 1, 1, 0, 0, 0, 0);
            // 
            // endDateTimePicker
            // 
            this.endDateTimePicker.CustomFormat = "yyyy-MM-dd";
            this.endDateTimePicker.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.endDateTimePicker.Location = new System.Drawing.Point(331, 29);
            this.endDateTimePicker.MinDate = new System.DateTime(2000, 1, 1, 0, 0, 0, 0);
            this.endDateTimePicker.Name = "endDateTimePicker";
            this.endDateTimePicker.Size = new System.Drawing.Size(98, 21);
            this.endDateTimePicker.TabIndex = 2;
            this.endDateTimePicker.Value = new System.DateTime(2014, 11, 19, 11, 19, 15, 0);
            // 
            // myChart
            // 
            chartArea1.AxisY.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount;
            chartArea1.AxisY.ScaleBreakStyle.StartFromZero = System.Windows.Forms.DataVisualization.Charting.StartFromZero.No;
            chartArea1.Name = "ChartArea1";
            this.myChart.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.myChart.Legends.Add(legend1);
            this.myChart.Location = new System.Drawing.Point(63, 65);
            this.myChart.Name = "myChart";
            this.myChart.Size = new System.Drawing.Size(580, 363);
            this.myChart.TabIndex = 3;
            this.myChart.Text = "chart1";
            this.myChart.Click += new System.EventHandler(this.myChart_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(65, 32);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 12);
            this.label1.TabIndex = 4;
            this.label1.Text = "起始时间：";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("宋体", 9F);
            this.label2.Location = new System.Drawing.Point(260, 33);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(65, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "终止时间：";
            // 
            // radioButton_KQ
            // 
            this.radioButton_KQ.AutoSize = true;
            this.radioButton_KQ.Checked = true;
            this.radioButton_KQ.Location = new System.Drawing.Point(465, 2);
            this.radioButton_KQ.Name = "radioButton_KQ";
            this.radioButton_KQ.Size = new System.Drawing.Size(71, 16);
            this.radioButton_KQ.TabIndex = 6;
            this.radioButton_KQ.TabStop = true;
            this.radioButton_KQ.Text = "克强指数";
            this.radioButton_KQ.UseVisualStyleBackColor = true;
            this.radioButton_KQ.CheckedChanged += new System.EventHandler(this.radioButton_KQ_CheckedChanged);
            // 
            // radioButton_GD
            // 
            this.radioButton_GD.AutoSize = true;
            this.radioButton_GD.Location = new System.Drawing.Point(465, 28);
            this.radioButton_GD.Name = "radioButton_GD";
            this.radioButton_GD.Size = new System.Drawing.Size(71, 16);
            this.radioButton_GD.TabIndex = 7;
            this.radioButton_GD.Text = "固定投资";
            this.radioButton_GD.UseVisualStyleBackColor = true;
            this.radioButton_GD.CheckedChanged += new System.EventHandler(this.radioButton_GD_CheckedChanged);
            // 
            // label
            // 
            this.label.Location = new System.Drawing.Point(63, 446);
            this.label.Name = "label";
            this.label.ReadOnly = true;
            this.label.Size = new System.Drawing.Size(580, 21);
            this.label.TabIndex = 8;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(704, 481);
            this.Controls.Add(this.label);
            this.Controls.Add(this.radioButton_GD);
            this.Controls.Add(this.radioButton_KQ);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.myChart);
            this.Controls.Add(this.endDateTimePicker);
            this.Controls.Add(this.startDateTimePicker);
            this.Controls.Add(this.Excute);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.myChart)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button Excute;
        private System.Windows.Forms.DateTimePicker startDateTimePicker;
        private System.Windows.Forms.DateTimePicker endDateTimePicker;
        private System.Windows.Forms.DataVisualization.Charting.Chart myChart;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.RadioButton radioButton_KQ;
        private System.Windows.Forms.RadioButton radioButton_GD;
        private System.Windows.Forms.TextBox label;
    }
}

