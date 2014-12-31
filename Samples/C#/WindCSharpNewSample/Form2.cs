using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Timers;
using System.Threading;
using WAPIWrapperCSharp;

namespace WindCSharpNewSample
{
    public partial class Form2 : Form
    {
        WindAPI w = WindQuantCSharpSycSample.w;
        ulong rid = 0;
        WindData wsqData = new WindData();
        public Form2()
        {
            InitializeComponent();
            this.cbFuncList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbFuncList.Items.AddRange(new String[] {"     WSD","     WSS","     WSI","     WST","     WSQ","     WSET","     WEQS","     TDays","  TDaysOffset","  TDaysCount"});
            this.cbFuncList.SelectedItem = "     WSD";
            this.MaximizeBox = false;
            this.dtpStartTime.CustomFormat = "yyyy-MM-dd";
            this.dtpStartTime.Format = DateTimePickerFormat.Custom;
            this.dtpEndTime.CustomFormat = "yyyy-MM-dd";
            this.dtpEndTime.Format = DateTimePickerFormat.Custom;
            this.cbRequest.Enabled = false;
        }

        private void updateGrid(WindData wd,string func) {
            int clength = wd.codeList.Length, flength = wd.fieldList.Length, tlength = wd.timeList.Length;
            if (wd.errorCode != 0) {
                this.dataGridView1.ColumnCount = 2;
                this.dataGridView1.RowCount = 1;
                this.dataGridView1.Columns[0].HeaderText = "ErrCode";
                this.dataGridView1.Columns[1].HeaderText = "ErrMessage";
                this.dataGridView1.Rows[0].Cells[0].Value = wd.errorCode;
                this.dataGridView1.Rows[0].Cells[1].Value = w.getErrorMsg(wd.errorCode);
                return;
            }
            switch (func) {
                case "wsd": 
                case "wsi":
                case "wst":{
                    this.dataGridView1.ColumnCount = clength * flength;
                    this.dataGridView1.RowCount = tlength+1;
                    this.dataGridView1.RowHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                    for (int i = 0; i < this.dataGridView1.ColumnCount; i++)
                    {
                        this.dataGridView1.Columns[i].HeaderText = "";
                        if (i % flength == flength / 2)
                            this.dataGridView1.Columns[i].HeaderText = wd.codeList[i / flength];
                    }
                    for (int m = 0; m < clength; m++)
                        for (int j = 0; j < flength; j++)
                        {
                            this.dataGridView1.Rows[0].Cells[m * flength + j].Value = wd.fieldList[j];
                        }
                    this.dataGridView1.Rows[0].HeaderCell.Value = "";
                    for (int k = 1; k < this.dataGridView1.RowCount; k++) {
                        if (func == "wsd")
                        {
                            this.dataGridView1.RowHeadersWidth = 120;
                            this.dataGridView1.Rows[k].HeaderCell.Value = wd.timeList[k - 1].ToString("yyyy-MM-dd");
                        }
                        else
                        {
                            this.dataGridView1.RowHeadersWidth = 160;
                            this.dataGridView1.Rows[k].HeaderCell.Value = wd.timeList[k - 1].ToString("yyyy-MM-dd HH:mm:ss");
                        }
                    }
                    object[,] odata = (object[,])wd.getDataByFunc(func, false);
                    for (int l = 0; l < tlength; l++)
                        for (int n = 0; n < clength * flength;n++ )
                            this.dataGridView1.Rows[l+1].Cells[n].Value = string.Format("{0}",odata[l,n]);
                    break;
                }
                case "wss":
                case "wset":
                case "wsq": {
                    this.dataGridView1.ColumnCount = flength;
                    this.dataGridView1.RowCount = clength;
                    this.dataGridView1.RowHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                    this.dataGridView1.RowHeadersWidth = 120;
                    for (int i = 0; i < this.dataGridView1.ColumnCount; i++)
                    {
                        this.dataGridView1.Columns[i].HeaderText = wd.fieldList[i];
                    }
                    for (int j = 0; j < this.dataGridView1.RowCount; j++) {
                        this.dataGridView1.Rows[j].HeaderCell.Value = wd.codeList[j];
                    }
                    object[,] odata = (object[,])wd.getDataByFunc(func, false);
                    //System.Console.WriteLine(odata[0,0]);
                    for (int l = 0; l < clength; l++)
                        for (int n = 0; n < flength; n++)
                            this.dataGridView1.Rows[l].Cells[n].Value = string.Format("{0}", odata[l, n]);
                    break;
                }
                default: {
                    this.dataGridView1.ColumnCount = 1;
                    this.dataGridView1.RowCount = ((Array)wd.data).Length;
                    this.dataGridView1.Columns[0].HeaderText = wd.fieldList[0];
                    for (int j = 0; j < this.dataGridView1.RowCount;j++ )
                        this.dataGridView1.Rows[j].Cells[0].Value = ((Array)wd.data).GetValue(j);
                    break;
                }
            }
            return;
        }

        private void btExecute_Click(object sender, EventArgs e)
        {
            int index = this.cbFuncList.SelectedIndex;
            String windCodes, indicators, startTime, endTime, options;
            windCodes = this.tbWindCodes.Text;
            indicators = this.tbIndicators.Text;
            startTime = this.dtpStartTime.Text;
            endTime =this.dtpEndTime.Text;
            options = this.tbOptions.Text;
            WindData wd = new WindData();
            switch (index) {
                case 0:{
                    wd = w.wsd(windCodes, indicators, startTime, endTime, options);
                    updateGrid(wd,"wsd");
                    break;
                }
                case 1: {
                    wd = w.wss(windCodes, indicators, options);
                    updateGrid(wd,"wss");
                    break;    
                }
                case 2: {
                    wd = w.wsi(windCodes, indicators, startTime, endTime, options);
                    updateGrid(wd,"wsi");
                    break;
                }
                case 3: {
                    wd = w.wst(windCodes,indicators,startTime,endTime,options);
                    updateGrid(wd,"wst");
                    break;
                }
                case 4: {
                    wd = w.wsq(windCodes,indicators,options);
                    updateGrid(wd,"wsq");
                    break;
                }
                case 5: {
                    wd = w.wset(windCodes,options);
                    updateGrid(wd,"wset");
                    break;
                }
                case 6: {
                    wd = w.weqs(windCodes, options);
                    updateGrid(wd,"weqs");
                    break;
                }
                case 7: {
                    wd = w.tdays(startTime, endTime, options);
                    updateGrid(wd,"tdays");
                    break;
                }
                case 8: {
                    wd = w.tdaysoffset(startTime, -30, options);
                    updateGrid(wd,"tdaysoffset");
                    break;
                }
                case 9: {
                    wd = w.tdayscount(startTime, endTime, options);
                    updateGrid(wd,"tdayscount");
                    break;
                }
            }
        }

        private void cbFuncList_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.label3.Enabled = true;
            this.label2.Enabled = true;
            this.label4.Enabled = true;
            this.label6.Enabled = true;
            this.label5.Enabled = true;
            this.dtpStartTime.Enabled = true;
            this.dtpEndTime.Enabled = true;
            this.tbOptions.Enabled = true;
            this.tbIndicators.Enabled = true;
            this.tbWindCodes.Enabled = true;
            this.dtpStartTime.CustomFormat = "yyyy-MM-dd";
            this.dtpStartTime.Format = DateTimePickerFormat.Custom;
            this.dtpEndTime.CustomFormat = "yyyy-MM-dd";
            this.dtpEndTime.Format = DateTimePickerFormat.Custom;
            int index = this.cbFuncList.SelectedIndex;
            switch (index) {
                case 0: {
                    this.tbWindCodes.Text = "000001.SZ";
                    this.tbIndicators.Text = "high,low,open,close,amt";
                    this.dtpStartTime.Text = "2014-06-09";
                    this.dtpEndTime.Text = "2014-07-08";
                    this.tbOptions.Text = "Fill=Previous";
                    this.cbRequest.Enabled = false;
                    break;
                }
                case 1: {
                    this.label3.Enabled = false;
                    this.label2.Enabled = false;
                    this.label6.Enabled = false;
                    this.dtpStartTime.Enabled = false;
                    this.dtpEndTime.Enabled = false;
                    this.tbWindCodes.Text = "000001.SZ,000002.SZ";
                    this.tbIndicators.Text = "comp_name,comp_name_eng,regcapital";
                    this.tbOptions.Enabled = false;
                    this.tbOptions.Text = "";
                    this.cbRequest.Enabled = false;
                    break;
                }
                case 2: {
                    this.dtpStartTime.CustomFormat = "yyyy-MM-dd HH:mm:ss";
                    this.dtpStartTime.Format = DateTimePickerFormat.Custom;
                    this.dtpEndTime.CustomFormat = "yyyy-MM-dd HH:mm:ss";
                    this.dtpEndTime.Format = DateTimePickerFormat.Custom;
                    this.tbWindCodes.Text = "000001.SZ";
                    this.tbIndicators.Text = "close,amt";
                    this.dtpStartTime.Text = "2013-11-06 09:40:00";
                    this.dtpEndTime.Text = "2013-11-06 10:40:00";
                    this.tbOptions.Enabled = false;
                    this.tbOptions.Text = "Barsize=3";
                    this.label6.Enabled = false;
                    this.cbRequest.Enabled = false;
                    break;
                }
                case 3: {
                    this.dtpStartTime.CustomFormat = "yyyy-MM-dd HH:mm:ss";
                    this.dtpStartTime.Format = DateTimePickerFormat.Custom;
                    this.dtpEndTime.CustomFormat = "yyyy-MM-dd HH:mm:ss";
                    this.dtpEndTime.Format = DateTimePickerFormat.Custom;
                    this.tbWindCodes.Text = "600000.SH";
                    this.tbIndicators.Text = "high,last";
                    this.dtpStartTime.Text = "2014-08-11 09:30:00";
                    this.dtpEndTime.Text = "2014-08-11 14:20:20";
                    this.tbOptions.Enabled = false;
                    this.tbOptions.Text = "";
                    this.label6.Enabled = false;
                    this.cbRequest.Enabled = false;
                    break;
                }
                case 4: {
                    this.tbWindCodes.Text = "IC1412.CFE,600000.sh";
                    this.tbIndicators.Text = "rt_last,rt_amt";
                    this.label3.Enabled = false;
                    this.label2.Enabled = false;
                    this.label6.Enabled = false;
                    this.dtpStartTime.Enabled = false;
                    this.dtpEndTime.Enabled = false;
                    this.tbOptions.Enabled = false;
                    this.tbOptions.Text = "";
                    this.cbRequest.Enabled = true;
                    this.cbRequest.Checked = false;
                    break;
                }
                case 5: {
                    this.tbWindCodes.Text = "IndexConstituent";
                    this.tbIndicators.Text = "";
                    this.tbIndicators.Enabled = false;
                    this.label5.Enabled = false;
                    this.label3.Enabled = false;
                    this.label2.Enabled = false;
                    this.dtpStartTime.Enabled = false;
                    this.dtpEndTime.Enabled = false;
                    this.cbRequest.Enabled = false;
                    this.tbOptions.Text = "date=20140710;windcode=000300.SH";
                    break;
                }
                case 6: {
                    this.tbWindCodes.Text = "布林上轨突破";
                    this.tbIndicators.Text = "";
                    this.tbIndicators.Enabled = false;
                    this.label5.Enabled = false;
                    this.label3.Enabled = false;
                    this.label2.Enabled = false;
                    this.dtpStartTime.Enabled = false;
                    this.dtpEndTime.Enabled = false;
                    this.cbRequest.Enabled = false;
                    this.tbOptions.Text = "";
                    this.label6.Enabled = false;
                    this.tbOptions.Enabled = false;
                    break;
                }
                case 7: {
                    this.label5.Enabled = false;
                    this.label4.Enabled = false;
                    this.tbWindCodes.Text = "";
                    this.tbWindCodes.Enabled = false;
                    this.tbIndicators.Text = "";
                    this.tbIndicators.Enabled = false;
                    this.cbRequest.Enabled = false;
                    this.tbOptions.Text = "Days=Weekdays";
                    this.dtpStartTime.Text = "2014-06-09";
                    this.dtpEndTime.Text = "2014-07-08";
                    break;
                }
                case 8:{
                    this.label5.Enabled = false;
                    this.label4.Enabled = false;
                    this.tbWindCodes.Text = "";
                    this.tbWindCodes.Enabled = false;
                    this.tbIndicators.Text = "";
                    this.tbIndicators.Enabled = false;
                    this.cbRequest.Enabled = false;
                    this.tbOptions.Text = "";
                    this.dtpStartTime.Text = "2014-07-09";
                    this.dtpEndTime.Enabled = false;
                    this.label2.Enabled = false;
                    break;
                }
                case 9: {
                    this.label5.Enabled = false;
                    this.label4.Enabled = false;
                    this.tbWindCodes.Text = "";
                    this.tbWindCodes.Enabled = false;
                    this.tbIndicators.Text = "";
                    this.tbIndicators.Enabled = false;
                    this.cbRequest.Enabled = false;
                    this.tbOptions.Text = "Days=Weekdays";
                    this.tbOptions.Enabled = false;
                    this.label6.Enabled = false;
                    this.dtpStartTime.Text = "2014-06-09";
                    this.dtpEndTime.Text = "2014-07-08";
                    break;
                }
            }
        }
     
        public delegate void SetDelegate();
        
        private void cbRequest_CheckedChanged(object sender, EventArgs e)
        {
            string windCodes = this.tbWindCodes.Text;
            string indicators = this.tbIndicators.Text;
            string options = this.tbOptions.Text;
            if (cbRequest.Checked)
            {
                rid = w.wsq(windCodes, indicators, options, myCallback);
            }
            else {
                w.cancelRequest(rid);
            }
        }

        public void SetCb()
        {
            if (cbFuncList.SelectedIndex != 4 || cbRequest.Checked != true)
                return;
            updateGrid(wsqData,"wsq");
        }

        SetDelegate sd;

        public void myCallback(WindData wd) {
            wsqData = wd;
            //System.Console.WriteLine(((double[])wd.data)[0]);
            sd = new SetDelegate(SetCb);
            this.cbRequest.Invoke(sd);
            this.cbFuncList.Invoke(sd);
            this.dataGridView1.Invoke(sd);
        }

        private void Form2_FormClosing(object sender, FormClosingEventArgs e)
        {
            w.stop();
            Environment.Exit(0);
            //w.cancelRequest(rid);
        }
    }
}
