using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WAPIWrapperCSharp;
using WindCommon;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private WindAPI m_w = null;
        private UInt64 m_uReqId = 0;

        private void wsq_Click(object sender, EventArgs e)
        {
            if (!start())
                return;
            //查询一次
            WindData wd = m_w.wsq("AAL.L,ABF.L", "rt_time,rt_pre_close,rt_open,rt_last,rt_latest,rt_chg", "");
            
            string s = WindDataMethod.WindDataToString(wd, "wsq");
            ShowInRichText(s);
        }

        //启动、登录
        private bool start()
        {
            if (null == m_w)
            {
                m_w = new WindAPI();
            }

            int nStatus = 0;
            if (!m_w.isconnected())
                nStatus = m_w.start();
            if (0 != nStatus)
                return false;
            return true;
        }

        //停止
        private void stop()
        {
            if (null != m_w)
                m_w.stop();
        }

        delegate void ShowWindData(string s);

        private void DisplayWindData(ShowWindData show, string s)
        {
            richTextBox1.Invoke(show, new object[] { s });
        }

        private void ShowInRichText(string s)
        {
            richTextBox1.AppendText(s);
        }

        private void wsq_continue_Click(object sender, EventArgs e)
        {
            if (!start())
                return;

            ulong rid = m_w.wsq("AAL.L,ABF.L", "rt_time,rt_pre_close,rt_open,rt_last,rt_latest,rt_chg", "", myCallback, true);//全部更新
            //ulong rid = m_w.wsq("0700.HK,0001.HK", "rt_ask1,rt_bid1,rt_last,rt_asize1,rt_bsize1", "", myCallback, true);
            m_uReqId = rid;
        }

        private void btn_stop_Click(object sender, EventArgs e)
        {
            stop();
        }

        private void CancelReq_Click(object sender, EventArgs e)
        {
            if (null != m_w)
                m_w.cancelRequest(m_uReqId);
        }

        public void myCallback(WindData wd)
        {
            //用户代码区域
            //订阅返回数据存放在WindData参数wd中，可以对其进行分析操作

            string s = WindDataMethod.WindDataToString(wd, "wsq");
            DisplayWindData(new ShowWindData(ShowInRichText), s);
        }

        private void wsq_continue_part_Click(object sender, EventArgs e)
        {
            if (!start())
                return;
            //ulong rid = m_w.wsq("0700.HK,0011.HK,AAL.L,ABF.L", "rt_time,rt_pre_close,rt_open,rt_last,rt_latest,rt_chg", "", myCallback, false);//只更新改变
            ulong rid = m_w.wsq("AAL.L,ABF.L", "rt_time,rt_pre_close,rt_open,rt_last,rt_latest,rt_chg", "", myCallback, false);//只更新改变
            m_uReqId = rid;
        }
    }
}
