using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WAPIWrapperCSharp;

namespace WindCSharpNewSample
{
    public partial class Form4 : Form
    {
        WindAPI w = WindQuantCSharpSycSample.w;
        public Form4()
        {
            InitializeComponent();
            tabControl1.TabPages[0].Text = "登出";
            tabControl1.TabPages[1].Text = "委托下单";
            tabControl1.TabPages[2].Text = "撤销委托";
            tabControl1.TabPages[3].Text = "查询";
            this.MaximizeBox = false;
            comboBox4.Items.AddRange(new string[] {"深圳","上海","深圳特","港股","郑州商品期货","上海商品期货","大连商品期货","中金所"});
            comboBox4.SelectedIndex = -1;
            comboBox1.Items.AddRange(new string[] { "买入开仓/股票买入", "卖出开仓", "买入平仓", "卖出平仓/股票卖出", "买入平今仓", "卖出平今仓", "备兑开仓", "备兑平仓" });
            comboBox1.SelectedIndex = -1;
            comboBox2.Items.AddRange(new string[] { "限价委托", "对方最优价格委托", "本方最优价格委托", "即时成交剩余撤销", "最优五档剩余撤销", "全额成交或撤销委托(市价FOK)" });
            comboBox2.SelectedIndex = -1;
            comboBox3.Items.AddRange(new string[] { "投机","保值"});
            comboBox3.SelectedIndex = -1;
            comboBox5.Items.AddRange(new string[]{"资金查询","持仓查询","当日委托查询","当日成交查询","股东账号查询","登录ID查询"});
            comboBox5.SelectedIndex = 0;
            comboBox6.Items.AddRange(new string[] {"全部","可撤"});
            comboBox6.SelectedIndex = 0;
            textBox8.Enabled = false;
            textBox9.Enabled = false;
            comboBox6.Enabled = false;
        }

        private string[] cb1List = new string[] { "Buy", "Short", "Cover", "Sell", "CoverToday", "SellToday", "CoveredShort", "CoveredCover" };
        private string[] cb2List = new string[] { "LMT", "BOC", "BOP", "ITC", "B5TC", "FOK" };
        private string[] cb3List = new string[] { "SPEC", "HEDG" };
        private string[] cb4List = new string[] {"SZ","SH","OC","HK","CZC","SHF","DCE","CFE"};
        private string[] cb5List = new string[] {"Capital","Position","Order","Trade","Account","LogonID"};

        private void Form4_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Dispose();
            Environment.Exit(0);
        }

        private void updateGrid(WindData wd) {
            dataGridView1.RowHeadersWidth = 120;
            if (wd.data is object[])
            {
                object[] tmpdata = (object[])wd.data;
                int dlength = tmpdata.Length;
                dataGridView1.ColumnCount = 1;
                dataGridView1.RowCount = dlength;
                for (int i = 0; i < dlength; i++)
                {
                     dataGridView1.Rows[i].HeaderCell.Value = wd.fieldList[i];
                     dataGridView1.Rows[i].Cells[0].Value = string.Format("{0}", ((Array)wd.data).GetValue(i));
                }
             }
             else {
                object[,] tmpdata = (object[,])wd.data;
                int dlength = tmpdata.Length;
                int flength = wd.fieldList.Length;
                int width = dlength / flength;
                dataGridView1.ColumnCount = width;
                dataGridView1.RowCount = flength;
                for (int i = 0; i < flength; i++)
                    for (int j = 0; j < width;j++ )
                    {
                        dataGridView1.Rows[i].HeaderCell.Value = wd.fieldList[i];
                        dataGridView1.Rows[i].Cells[j].Value = string.Format("{0}", ((object[,])wd.data)[i, j]);
                    } 
             }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            WindData wd;
            string sid = textBox1.Text;
            int id;
            if (sid == "")
            {
                wd = w.tlogout();
            }
            else {
                if (int.TryParse(sid, out id))
                    wd = w.tlogout(id);
                else
                    wd = w.tlogout(-1);
            }
            updateGrid(wd);
        }

        private void mustInput() {
            MessageBox.Show("必填项不能为空"); 
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string rid = textBox6.Text;
            if (rid == "")
            {
                mustInput();
                return;
            }
            string lid = textBox7.Text;
            string options = "";
            if (comboBox4.SelectedIndex != -1)
            {
                options += "MarketType=";
                options += cb4List[comboBox4.SelectedIndex];
            }
            if (lid != "") {
                options += ";LogonID=";
                options += lid;
            }
            WindData wd = w.tcancel(rid, options);
            updateGrid(wd);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            string lid = textBox2.Text, windCode = textBox3.Text, price = textBox4.Text, volumn = textBox5.Text;
            if (windCode == "" || price == "" || volumn == "" || comboBox1.SelectedIndex == -1||comboBox2.SelectedIndex == -1) {
                mustInput();
                return;
            }
            string options = "OrderType="+cb2List[comboBox2.SelectedIndex];
            string tradeSide = cb1List[comboBox1.SelectedIndex];
            if (comboBox3.SelectedIndex != -1) {
                options += ";HedgeType=";
                options += cb3List[comboBox3.SelectedIndex];
            }
            if (lid != "") {
                options += ";LogonID=";
                options += lid;
            }
            WindData wd = w.torder(windCode, tradeSide, price, volumn, options);
            updateGrid(wd);
        }

        private void cb5_SelectedIndexChanged(object sender, EventArgs e)
        {
            bool isEnabled = (comboBox5.SelectedIndex == 2);
            textBox9.Enabled = isEnabled;
            comboBox6.Enabled = isEnabled;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            int index = comboBox5.SelectedIndex;
            string qryCode = cb5List[index],lid = textBox10.Text,rid = textBox9.Text,oid = textBox8.Text;
            string options = "";
            if (lid != "") {
                options += "LogonId=";
                options += lid;
            }
            if (index == 2) {
                if (rid != "")
                {
                    options += ";RequestID=";
                    options += rid;
                }
                if (comboBox6.SelectedIndex == 1) {
                    options += ";OrderType=Withdrawable";
                }
            }
            WindData wd = w.tquery(qryCode,options);
            updateGrid(wd);
        }
    }
}
