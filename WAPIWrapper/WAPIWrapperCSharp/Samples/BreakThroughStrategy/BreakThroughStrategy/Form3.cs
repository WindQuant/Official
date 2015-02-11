using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WAPIWrapperCSharp;

namespace BreakThroughStrategy
{
    public partial class Form3 : Form
    {
        public static WindAPI w = new WindAPI();
        public static bool showLogon = false;
        public Form3()
        {
            InitializeComponent();
            this.comboBox1.Items.Add("Wind模拟交易");
            this.comboBox1.SelectedIndex = 0;
            this.comboBox2.Enabled = false;
            this.comboBox3.Items.AddRange(new String[] { "上海深圳A", "上海B", "深圳B", "郑商所", "上期所", "大商所", "中金所", "上证期权" });
            this.comboBox3.SelectedIndex = 0;
        }

        private void Form3_FormClosing(object sender, FormClosingEventArgs e)
        {
            Environment.Exit(0);
        }

        private string[] types = new string[] { "SHSZ", "SHB", "SZB", "CZC", "SHF", "DCE", "CFE", "SHO" };

        private void button1_Click(object sender, EventArgs e)
        {
            string accountID, password, accountType;
            accountID = textBox1.Text;
            password = textBox2.Text;
            accountType = types[comboBox3.SelectedIndex];
            WindData wd = w.tlogon("0000", "0", accountID, password, accountType, "");
            this.Visible = false;
            if (wd.errorCode == 0)
            {
                //MessageBox.Show("登录成功！");
                Form1 bmain = new Form1();
                DialogResult dr = bmain.ShowDialog();
                if (dr == DialogResult.Cancel && showLogon)
                    this.Visible = true;
                else
                    this.Close();
            }
            else
            {
                MessageBox.Show(Convert.ToString(((object[,])wd.data)[4, 0]));
                this.Visible = true;
            }
        }
    }
}
