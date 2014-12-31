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
    public partial class WindQuantCSharpSycSample : Form
    {
        public static WindAPI w = new WindAPI();
        public WindQuantCSharpSycSample()
        {
            InitializeComponent();
            pictureBox1.Image = Properties.Resources.bgImage;
            this.MaximizeBox = false;
            this.comboBox1.DropDownStyle = ComboBoxStyle.DropDownList;
            this.comboBox1.Items.AddRange(new string[]{"     Wind数据接口实例","     Wind交易接口实例"});
            this.comboBox1.SelectedIndex = 0;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Hide();
            int LogRet = (int)w.start("","",5000);
            if (LogRet == 0)
            {
                if (this.comboBox1.SelectedIndex == 0)
                {
                    //MessageBox.Show("登陆成功！");
                    Form mainMenu = new Form2();
                    mainMenu.Show();
                }
                else {
                    Form mainMenu = new Form3();
                    mainMenu.Show();
                }
            }
            else
            {
                MessageBox.Show("登陆失败！" + Environment.NewLine + "请检查Wind终端是否打开。错误码" + LogRet.ToString() + "。");
                this.Show();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void WindQuantCSharpSycSample_Load(object sender, EventArgs e)
        {

        }
    }
}
