using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WAPIWrapperCSharp;
using System.Windows.Forms.DataVisualization.Charting;

namespace WindEDBFormSample
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            InitializeChart();
        }

        private void Excute_Click(object sender, EventArgs e)
        {
            InitializeChart();
            this.myChart.Series.Clear();

            string strStartDate = this.startDateTimePicker.Text;
            string strEndDate = this.endDateTimePicker.Text;

            WindAPI w = new WindAPI();
            w.start();

            WindData wd = null;
            if (this.radioButton_KQ.Checked)
                wd = w.edb("M5407921", strStartDate, strEndDate, "");
            else
                wd = w.edb("M0000272", strStartDate, strEndDate, "");

            w.stop();

            UpdateChart(wd);
        }

        private void InitializeChart()
        {
            #region 设置图表的属性
            myChart.BackColor = Color.FromArgb(211, 223, 240);//图表的背景色
            myChart.BackGradientStyle = GradientStyle.TopBottom;//图表背景色的渐变方式
            myChart.BorderlineColor = Color.FromArgb(26, 59, 105);//图表的边框颜色
            myChart.BorderlineDashStyle = ChartDashStyle.Solid;//图表的边框线条样式
            myChart.BorderlineWidth = 2;//图表边框线条的宽度
            myChart.BorderSkin.SkinStyle = BorderSkinStyle.Emboss;//图表边框的皮肤
            #endregion

            #region 设置图表的Title
            if (myChart.Titles.Count < 1)
            {
                Title title = new Title();
                title.Text = "克强指数";
                title.Font = new System.Drawing.Font("Microsoft Sans Serif", 12, FontStyle.Bold);
                title.ForeColor = Color.FromArgb(255, 0, 255);//标题字体颜色
                title.ShadowColor = Color.FromArgb(32, 0, 0, 0);//标题阴影颜色
                title.ShadowOffset = 3;//标题阴影偏移量
                myChart.Titles.Clear();
                myChart.Titles.Add(title);
            }
            #endregion

            #region 设置图表区属性
            //图表区的名字
            ChartArea chartArea = new ChartArea("Default");
            //背景色
            chartArea.BackColor = Color.FromArgb(64, 165, 191, 228);
            //背景渐变方式
            chartArea.BackGradientStyle = GradientStyle.TopBottom;
            //渐变和阴影的辅助背景色
            chartArea.BackSecondaryColor = Color.White;
            //边框颜色
            chartArea.BorderColor = Color.FromArgb(64, 64, 64, 64);
            //阴影颜色
            chartArea.ShadowColor = Color.Transparent;

            //设置X轴和Y轴线条的颜色和宽度
            chartArea.AxisX.LineColor = Color.FromArgb(64, 64, 64, 64);
            chartArea.AxisX.LineWidth = 1;
            chartArea.AxisY.LineColor = Color.FromArgb(64, 64, 64, 64);
            chartArea.AxisY.LineWidth = 1;
            chartArea.AxisY.IsLabelAutoFit = true;
            chartArea.AxisY.IntervalAutoMode = IntervalAutoMode.VariableCount;

            //设置X轴和Y轴的标题
            chartArea.AxisX.Title = "日期";
            chartArea.AxisY.Title = "收盘价";

            //设置图表区网格横纵线条的颜色和宽度
            chartArea.AxisX.MajorGrid.LineColor = Color.FromArgb(64, 64, 64, 64);
            chartArea.AxisX.MajorGrid.LineWidth = 1;
            chartArea.AxisY.MajorGrid.LineColor = Color.FromArgb(64, 64, 64, 64);
            chartArea.AxisY.MajorGrid.LineWidth = 1;

            myChart.ChartAreas.Clear();
            myChart.ChartAreas.Add(chartArea);
            #endregion

            #region 图例及图例的位置
            Legend legend = new Legend();
            legend.Alignment = StringAlignment.Center;
            legend.Docking = Docking.Bottom;

            this.myChart.Legends.Add(legend);
            #endregion
        }

        private void SetChartTitle()
        {
            if (this.radioButton_KQ.Checked)
            {
                this.myChart.Titles[0].Text = "克强指数";
            }
            else
            {
                this.myChart.Titles[0].Text = "固定资产投资完成额";
            }
        }

        private void UpdateChart(WindData wd)
        {
            SetChartTitle();

            if (wd.GetDataLength() < 1 || wd.GetTimeLength() < 1)
                return;

            if (!(wd.data is double[]))
                return;
            double[] closeData = (double[])wd.data;
            if (closeData.Length != wd.GetTimeLength())
                return;

            AddChartSerie();
            Series ser = this.myChart.Series[0];
            double dClose = 0.0;
            string strDate = string.Empty;
            for (int i = 0; i < closeData.Length; i++)
            {
                dClose = closeData[i];
                ser.Points.AddXY(wd.timeList[i].ToString("yy/MM"), dClose);
            }
        }

        private void AddChartSerie()
        {
            Series ser = new Series("收盘价");

            //Series的类型
            ser.ChartType = SeriesChartType.Line;
            //Series的边框颜色
            ser.BorderColor = Color.FromArgb(180, 26, 59, 105);
            //线条宽度
            ser.BorderWidth = 3;
            //线条阴影颜色
            ser.ShadowColor = Color.Black;
            //阴影宽度
            ser.ShadowOffset = 2;
            //是否显示数据说明
            ser.IsVisibleInLegend = true;
            //线条上数据点上是否有数据显示
            ser.IsValueShownAsLabel = false;
            //线条上的数据点标志类型
            ser.MarkerStyle = MarkerStyle.Circle;
            //线条数据点的大小
            ser.MarkerSize = 8;
            //线条颜色

            ser.Color = Color.FromArgb(220, 65, 140, 240);

            this.myChart.Series.Add(ser);
        }

        private void radioButton_KQ_CheckedChanged(object sender, EventArgs e)
        {
            SetChartTitle();
        }

        private void radioButton_GD_CheckedChanged(object sender, EventArgs e)
        {
            SetChartTitle();
        }

        private void myChart_Click(object sender, EventArgs e)
        {
            // Call Hit Test Method
            MouseEventArgs mouseEventArgs = (MouseEventArgs)e;
            HitTestResult result = this.myChart.HitTest(mouseEventArgs.X, mouseEventArgs.Y);
            if (null == result)
                return;

            if (result.ChartElementType == ChartElementType.DataPoint)
            {
                DataPoint selectedDataPoint = (DataPoint)result.Object;
                this.label.Text = "日期：" + selectedDataPoint.AxisLabel + "\t收盘价值：" + selectedDataPoint.YValues[0];
                //MessageBox.Show("日期：" + selectedDataPoint.AxisLabel + "\t收盘价值：" + selectedDataPoint.YValues[0]);
            }
        }
    }
}
