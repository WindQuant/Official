using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WAPIWrapperCSharp;
using System.Collections;

namespace BreakThroughStrategy
{
    public partial class Form1 : Form
    {
        WindAPI w = Form3.w;
        Bitmap bmap;
        Graphics gph;
        ArrayList cycles = new ArrayList();
        ArrayList dateTimeCycles = new ArrayList();
        ArrayList originNums = new ArrayList();
        //ArrayList allLabels = new ArrayList();
        int volume = 100;
        ulong rid = 0;
        string rwCode = "";
        WindData wsqData;
        float px = 0, py = 0;
        float open = 0;
        DateTime lastTime = DateTime.Now;
        double Kmax = 0, Kmin = 0;
        int maxCount = 0, minCount = 0;
        int beHigh = 2;
        bool doBTUp = false, doBTDown = false;
        //int dis;
        Label minLabel = new Label();
        Label maxLabel = new Label();
        String smax = "", smin = "";
        public Form1()
        {
            InitializeComponent();
            this.MaximizeBox = false;
            Form3.showLogon = false;
            richTextBox1.Text = "    说明：该程序为使用wind模拟帐号进行运行的多周期突破策略，默认覆盖5分钟、15分钟两个周期的K线进行策略交易，可新增周期。\r\n    只需修改帐号，和相应的WindCode的即可进行日内跟踪行情实时模拟交易。\r\n   默认周期为5分钟和15分钟，交易量为100股。";
            cycles.Add(5);
            cycles.Add(15);
            dateTimeCycles.Add(DateTime.Now);
            dateTimeCycles.Add(DateTime.Now);
            originNums.Add(0);
            originNums.Add(0);
            label5.Text = "";
            label4.Text = "";
            label6.Text = "";
            label7.Text = "";
            label8.Text = "";
            label9.Text = "";
            label10.Text = "";
            //label11.Text = "";
            //label12.Text = "";
            label13.Text = "";
            label14.Text = "";
            //allLabels.Add(label12);
            //dis = label12.Location.Y - label11.Location.Y;
            w.start();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            w.cancelRequest(0);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            string windCode = textBox4.Text;
            string svolume = textBox3.Text;
            if (!int.TryParse(svolume, out volume)) {
                volume = 100;
            }
            WindData wd = w.wsi(windCode, "open", DateTime.Today.AddHours(9.5), DateTime.Now, "BarSize=1");
            if (wd.errorCode != 0)
            {
                richTextBox1.Text += "\r\n";
                richTextBox1.Text += w.getErrorMsg(wd.errorCode);
                return;
            }
            open = (float)((double[])wd.data)[0];
            Kmin = open;
            Kmax = 0;
            w.cancelRequest(rid);
            updateKLines(windCode);
            label4.Text = "当前价";
            label7.Text = "K线最高价";
            label7.ForeColor = Color.Red;
            label8.Text = "K线最低价";
            label8.ForeColor = Color.Green;
            label13.Text = "开盘价";
            label13.ForeColor = Color.SkyBlue;
            label14.Text = Convert.ToString(open);
            label14.ForeColor = Color.SkyBlue;
            //label11.Text = "5分钟K线图";
            //label12.Text = "15分钟K线图";
            //label11.ForeColor = myPens[0].Color;
            //label12.ForeColor = myPens[1].Color;
            label5.Text = windCode + " 最近15分钟价格走势图";
            //for (int i = 1; i < allLabels.Count; i++)
            //{
            //    Controls.Add((Label)allLabels[i]);
            //    ((Label)allLabels[i]).BringToFront();
            //}
        }


        private void getMaxMin(double[] datas, out double max, out double min) {
            max = Kmax;
            min = Kmin;
            foreach(double data in datas){
                if (data > max) {
                    max = data;
                }
                else if (data < min) {
                    min = data;
                }
            }
        }

        private void updateKLines(string windCode) {
            WindData wd;
            px = 0;
            py = 0;
            for (int i = 0; i < originNums.Count; i++)
            {
                originNums[i] = 0;
            }
            bmap = new Bitmap(548, 349);
            for (int i = 0; i < cycles.Count; i++)
            {
                string options = "BarSize=" + Convert.ToString(cycles[i]);
                wd = w.wsi(windCode, "open,close,high,low", DateTime.Today.AddHours(9.5), DateTime.Now, options);
                if (wd.errorCode != 0)
                {
                    richTextBox1.Text += "\r\n";
                    richTextBox1.Text += Convert.ToString(cycles[i]);
                    richTextBox1.Text += "分钟周期：";
                    richTextBox1.Text += w.getErrorMsg(wd.errorCode);
                    return;
                }
                drawKLines(wd, (int)cycles[i], i);
                getMaxMin((double[])wd.data,out Kmax,out Kmin);
            }
            doRequest(windCode);
            smax = Kmax.ToString("f2");
            label9.Text = smax;
            label9.ForeColor = Color.Red;
            smin = Kmin.ToString("f2");
            label10.Text = smin;
            label10.ForeColor = Color.Green;
            drawMaxMin();
        }

        private void drawMaxMin() {
            float fpx = 48, fpx1 = 528;
            float fpyMax = -((10 *(float)Kmax) / open - 10) * 175 * beHigh + 175;
            float fpyMin = -((10 * (float)Kmin) / open - 10) * 175 * beHigh + 175;
            float fpyOpen = 175;
            float[] dashValues = { 2, 3 };
            Pen maxPen = new Pen(Color.Red, 1);
            Pen openPen = new Pen(Color.LightBlue, 1);
            Pen minPen = new Pen(Color.Green, 1);
            maxPen.DashPattern = dashValues;
            openPen.DashPattern = dashValues;
            minPen.DashPattern = dashValues;
            gph.DrawLine(maxPen,new PointF(fpx,fpyMax),new PointF(fpx1,fpyMax));
            gph.DrawLine(minPen, new PointF(fpx, fpyMin), new PointF(fpx1, fpyMin));
            gph.DrawLine(openPen, new PointF(fpx, fpyOpen), new PointF(fpx1, fpyOpen));

            maxLabel.Text = smax;
            maxLabel.Font = label1.Font;
            maxLabel.ForeColor = maxPen.Color;
            maxLabel.Location = new Point(20, (int)fpyMax);
            maxLabel.AutoSize = true;
            Controls.Add(maxLabel);
            maxLabel.BringToFront();

            minLabel.Text = smin;
            minLabel.Font = label1.Font;
            minLabel.ForeColor = minPen.Color;
            minLabel.Location = new Point(20, (int)fpyMin);
            minLabel.AutoSize = true;
            Controls.Add(minLabel);
            minLabel.BringToFront();
            pictureBox1.Image = bmap;
        }

        private void doRequest(string windCode) {
            if (windCode == rwCode)
                return;
            else {
                rid = w.wsq(windCode, "rt_last", "", myCallback);
                rwCode = windCode;
            }
        }

        Pen[] myPens = new Pen[] { Pens.Blue,Pens.Green,Pens.Orange,Pens.Red,Pens.Purple,Pens.AliceBlue,Pens.DarkSeaGreen};

        public void drawKLines(WindData wd,int cycle,int colorNum) {
            double[,] data = (double[,])wd.getDataByFunc("wsi");
            gph = Graphics.FromImage(bmap);
            DateTime[] tlist = wd.timeList;
            PointF cpt = new PointF(50, 175);
            int oNum = (int)originNums[colorNum];
            //注释这段代码的功能是画K线
            //for (int i = 0; i < tlist.Length; i++) {
            //    int per = cycle * 2;
            //    PointF tmp = new PointF(48+per*(i+oNum),-(float)((10*data[i,0])/open-10)*175*beHigh+175);
            //    PointF tmp1 = new PointF(48 + per * (i + oNum) + per * 4 / 5, -(float)((10 * data[i, 1]) / open - 10) * 175 * beHigh + 175);
            //    PointF tmp2 = new PointF(48 + per * (i + oNum) + per * 2 / 5, -(float)((10 * data[i, 2]) / open - 10) * 175 * beHigh + 175);
            //    PointF tmp3 = new PointF(48 + per * (i + oNum) + per * 2 / 5, -(float)((10 * data[i, 3]) / open - 10) * 175 * beHigh + 175);
            //    if (tmp1.Y == tmp.Y)
            //        gph.DrawLine(myPens[colorNum%7], tmp, tmp1);
            //    else
            //        gph.DrawRectangle(myPens[colorNum%7], System.Math.Min(tmp.X, tmp1.X), System.Math.Min(tmp1.Y, tmp.Y), per * 4 / 5, System.Math.Abs(tmp1.Y - tmp.Y));
            //    gph.DrawLine(myPens[colorNum % 7],tmp2,tmp3);
            //}
            pictureBox1.Image = bmap;
            originNums[colorNum] = (int)originNums[colorNum] + tlist.Length;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            w.tlogout();
            Form3.showLogon = true;
            this.Hide();
        }

        public delegate void SetDelegate();

        SetDelegate sd;
        SetDelegate sd1;
        SetDelegate sd2;

        private void drawLines() {
            float ppx = 0, ppy = 0;
            float tmpPrice = (float)((double[])wsqData.data)[0];
            WindData wd;
            ppy = -((10*tmpPrice)/open-10)*175*beHigh+175;
            if (DateTime.Now >= lastTime.AddMinutes(15))
            {
                px = 0;
                py = 0;
                lastTime = DateTime.Now;
                updateKLines(rwCode);
                return;
            }
            else {
                ppx = (float)(DateTime.Now.ToOADate() - lastTime.ToOADate()) * 24 * 60 * 32 + 48;
            }
            if (px != 0 || py != 0)
            {
                Pen blackPen = new Pen(Color.Black,2);
                gph.DrawLine(blackPen, new PointF(px, py), new PointF(ppx, ppy));
                pictureBox1.Image = bmap;
            }
            px = ppx;
            py = ppy;
            if (maxCount == 1 && tmpPrice < Kmax)
                maxCount--;
            if (minCount == 1 && tmpPrice > Kmin)
                minCount--;
            if (tmpPrice > Kmax)
            {
                maxCount++;
                Kmax = tmpPrice;
            }
            if (tmpPrice < Kmin)
            {
                minCount++;
                Kmin = tmpPrice;
            }
            for (int i = 0; i < cycles.Count; i++) {
                if (DateTime.Now > ((DateTime)dateTimeCycles[i]).AddMinutes((int)cycles[i]+0.5)) {
                    wd = w.wsi(rwCode, "open,close,high,low",(DateTime)dateTimeCycles[i], DateTime.Now, "BarSize=" + Convert.ToString(cycles[i]));
                    if (wd.errorCode == 0)
                    {
                        drawKLines(wd, (int)cycles[i], i);
                        dateTimeCycles[i] = DateTime.Now.AddMinutes(-1);
                        getMaxMin((double[])wd.data, out Kmax, out Kmin);
                    }
                }
            }
        }

        private void doBreakThrough() {
            if (maxCount == 2 && !doBTUp)
            {
                richTextBox1.Text += "\r\n    最高价突破，执行买入。";
                w.torder(rwCode, "Buy", ((double[])wsqData.data)[0], volume, "");
                maxCount = 0;
                doBTUp = true;
            }
            else if (minCount == 2 && !doBTDown) {
                richTextBox1.Text += "\r\n    最低价突破，执行卖出。";
                w.torder(rwCode, "Sell", ((double[])wsqData.data)[0], volume, "");
                minCount = 0;
            }
        }

        private void updatePrice() {
            label6.Text = Convert.ToString(((Array)wsqData.data).GetValue(0));
        }

        public void myCallback(WindData wd) {
            if (wd.errorCode == 0)
            {
                wsqData = wd;
                sd = new SetDelegate(drawLines);
                sd1 = new SetDelegate(doBreakThrough);
                sd2 = new SetDelegate(updatePrice);
                this.pictureBox1.Invoke(sd);
                this.richTextBox1.Invoke(sd1);
                this.label6.Invoke(sd2);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            int cycle;
            if (int.TryParse(textBox2.Text, out cycle))
            {
                if (!cycles.Contains(cycle))
                {
                    cycles.Add(cycle);
                    dateTimeCycles.Add(DateTime.Now);
                    originNums.Add(0);
                }
                else
                {
                    MessageBox.Show("该周期已存在");
                    return;
                }
            }
            else
            {
                MessageBox.Show("请输入整数周期");
                return;
            }
            string text = "    现周期为";
            for (int i = 0; i < cycles.Count - 1; i++)
                text = text + Convert.ToString(cycles[i]) + "分钟、";
            text = text + Convert.ToString(cycles[cycles.Count - 1]) + "分钟。";
            richTextBox1.Text += "\r\n" + text;
            //Label tmpLabel = new Label();
            //tmpLabel.Text = Convert.ToString(cycle) + "分钟K线图";
            //tmpLabel.ForeColor = myPens[(cycles.Count - 1) % 7].Color;
            //tmpLabel.Font = label11.Font;
            //tmpLabel.Location = new Point(label12.Location.X, ((Label)(allLabels[allLabels.Count - 1])).Location.Y + dis);
            //tmpLabel.AutoSize = true;
            //tmpLabel.Size = label12.Size;
            //allLabels.Add(tmpLabel);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            lastTime = DateTime.Now;
            if (beHigh < 10)
                beHigh++;
            if (rwCode != "")
                updateKLines(rwCode);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            lastTime = DateTime.Now;
            if (beHigh > 2)
                beHigh--;
            if (rwCode != "")
                updateKLines(rwCode);
        }
    }
}
