using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WindDataCOMLib;
using System.Timers;

namespace WAPIWrapperCSharp
{
    public class WindData
    {
        public string[] codeList { get; set; }
        public string[] fieldList { get; set; }
        public DateTime[] timeList { get; set; }
        public object data { get; set; }
        public int errorCode { get; set; }

        //获取字段长度
        public int GetCodeLength()
        {
            if (null == codeList)
                return 0;
            return codeList.Length;
        }
        public int GetFieldLength()
        {
            if (null == fieldList)
                return 0;
            return fieldList.Length;
        }
        public int GetTimeLength()
        {
            if (null == timeList)
                return 0;
            return timeList.Length;
        }
        public int GetDataLength()
        {
            if (null == data || !(data is Array))
                return 0;
            return ((Array)data).Length;
        }

        //获取登录Id
        public string GetLogonId()
        {
            return GetDataByFiledName(this, "LogonId");
        }

        //获取下单请求Id
        public string GetOrderRequestID()
        {
            return GetDataByFiledName(this, "RequestID");
        }

        //获取下单Number
        public string GetOrderNumber()
        {
            return GetDataByFiledName(this, "OrderNumber");
        }

        //获取错误信息
        public string GetErrorMsg()
        {
            if (0 == errorCode)
                return string.Empty;

            string strErrorMsg = string.Empty;
            if (GetFieldLength() > 0 && GetDataLength() > 0)
            {
                string strErrorMsgField = string.Empty;
                for (int i = 0; i < GetFieldLength(); i++)
                {
                    strErrorMsgField = fieldList[i];
                    if (strErrorMsgField == "ErrorMsg" || strErrorMsgField == "OUTMESSAGE")
                    {
                        object error = ((Array)data).GetValue(i, 0);
                        strErrorMsg = error.ToString();
                        break;
                    }    
                }
            }
            return strErrorMsg;
        }

        //获得特定格式的数据
        public object getDataByFunc(string funcName,bool sameType = true) {
            if (codeList == null && fieldList == null && timeList == null)
                throw new Exception("WindData未初始化");
            if(errorCode != 0)
                return data;
            object tmpdata = data;
            switch (funcName) {
                case "wsd":
                case "wsi":
                case "wst":
                case "edb":{
                    tmpdata = likeWSD(sameType);
                    break;
                }
                case "wss": 
                case "wsq":
                case "wset":
                case "wpf":{
                    tmpdata = likeWSS(sameType);
                    break;
                }
                case "bktstart":
                case "bktquery":
                case "bktstatus":
                case "bktend":
                case "bktsummary":
                case "bktdelete":
                case "bktstrategy": {
                    tmpdata = likeBktstart(sameType);
                    break;
                }
            }
            return tmpdata;
        }

        private object getDataAtPos(int pos) {
            if (data is Array)
                return ((Array)data).GetValue(pos);
            else
                return null;
        }

        private enum DataType{ 
            TypeDouble = 1,
            TypeInt,
            TypeString,
            TypeDateTime,
            TypeInt64,
            TypeObject
        }

        private int getDataType() {
            if (data is double[])
                return (int)DataType.TypeDouble;
            else if (data is int[])
                return (int)DataType.TypeInt;
            else if (data is String[])
                return (int)DataType.TypeString;
            else if (data is DateTime[])
                return (int)DataType.TypeDateTime;
            else if (data is Int64[])
                return (int)DataType.TypeInt64;
            else if (data is object[])
                return (int)DataType.TypeObject;
            return -1;
        }

        private object likeWSD(bool sameType) {
            int clength = codeList.Length,flength = fieldList.Length,tlength = timeList.Length;
            if (!sameType) {
                object[,] tmpdata = new object[tlength, clength * flength];
                for (int i = 0; i < tlength; i++)
                    for (int j = 0; j < clength * flength; j++)
                        tmpdata[i, j] = getDataAtPos(i * clength * flength + j);
                return tmpdata;
            }
            int typenum = getDataType();
            switch (typenum) { 
                case 1:
                    {
                        double[,] tmpdata = new double[tlength, clength * flength];
                        for (int i = 0; i < tlength; i++)
                            for (int j = 0; j < clength * flength; j++)
                                tmpdata[i, j] = (double)getDataAtPos(i * clength * flength + j);
                        return tmpdata;
                    }
                case 2:
                    {
                        int[,] tmpdata = new int[tlength, clength * flength];
                        for (int i = 0; i < tlength; i++)
                            for (int j = 0; j < clength * flength; j++)
                                tmpdata[i, j] = (int)getDataAtPos(i * clength * flength + j);
                        return tmpdata;
                    }
                case 3:
                    {
                        string[,] tmpdata = new string[tlength, clength * flength];
                        for (int i = 0; i < tlength; i++)
                            for (int j = 0; j < clength * flength; j++)
                                tmpdata[i, j] = (string)getDataAtPos(i * clength * flength + j);
                        return tmpdata;
                    }
                case 4:
                    {
                        DateTime[,] tmpdata = new DateTime[tlength, clength * flength];
                        for (int i = 0; i < tlength; i++)
                            for (int j = 0; j < clength * flength; j++)
                                tmpdata[i, j] = (DateTime)getDataAtPos(i * clength * flength + j);
                        return tmpdata;
                    }
                case 5:
                    {
                        Int64[,] tmpdata = new Int64[tlength, clength * flength];
                        for (int i = 0; i < tlength; i++)
                            for (int j = 0; j < clength * flength; j++)
                                tmpdata[i, j] = (Int64)getDataAtPos(i * clength * flength + j);
                        return tmpdata;
                    }
                case 6:
                    {
                        object[,] tmpdata = new object[tlength, clength * flength];
                        for (int i = 0; i < tlength; i++)
                            for (int j = 0; j < clength * flength; j++)
                                tmpdata[i, j] = (object)getDataAtPos(i * clength * flength + j);
                        return tmpdata;
                    }
                default:
                    return data;
            }
        }

        private object likeWSS(bool sameType) {
            int clength = codeList.Length, flength = fieldList.Length;
            if (!sameType)
            {
                object[,] tmpdata = new object[clength, flength];
                for (int i = 0; i < clength; i++)
                    for (int j = 0; j < flength; j++)
                        tmpdata[i, j] = getDataAtPos(i * flength + j);
                return tmpdata;
            }
            int typenum = getDataType();
            switch (typenum)
            {
                case 1:
                    {
                        double[,] tmpdata = new double[clength,flength];
                        for (int i = 0; i < clength; i++)
                            for (int j = 0; j < flength; j++)
                                tmpdata[i, j] = (double)getDataAtPos(i * flength + j);
                        return tmpdata;
                    }
                case 2:
                    {
                        int[,] tmpdata = new int[clength, flength];
                        for (int i = 0; i < clength; i++)
                            for (int j = 0; j < flength; j++)
                                tmpdata[i, j] = (int)getDataAtPos(i * flength + j);
                        return tmpdata;
                    }
                case 3:
                    {
                        string[,] tmpdata = new string[clength, flength];
                        for (int i = 0; i < clength; i++)
                            for (int j = 0; j < flength; j++)
                                tmpdata[i, j] = (string)getDataAtPos(i * flength + j);
                        return tmpdata;
                    }
                case 4:
                    {
                        DateTime[,] tmpdata = new DateTime[clength, flength];
                        for (int i = 0; i < clength; i++)
                            for (int j = 0; j < flength; j++)
                                tmpdata[i, j] = (DateTime)getDataAtPos(i * flength + j);
                        return tmpdata;
                    }
                case 5:
                    {
                        Int64[,] tmpdata = new Int64[clength, flength];
                        for (int i = 0; i < clength; i++)
                            for (int j = 0; j < flength; j++)
                                tmpdata[i, j] = (Int64)getDataAtPos(i * flength + j);
                        return tmpdata;
                    }
                case 6:
                    {
                        object[,] tmpdata = new object[clength, flength];
                        for (int i = 0; i < clength; i++)
                            for (int j = 0; j < flength; j++)
                                tmpdata[i, j] = (object)getDataAtPos(i * flength + j);
                        return tmpdata;
                    }
                default:
                    return data;
            }
        }

        private object likeBktstart(bool sameType) {
            int flength = fieldList.Length;
            object[] tmpdata = new object[flength];
            for (int i = 0; i < flength; i++)
                tmpdata[i] = ((object[,])data)[i, 0];
            return tmpdata;
        }

        private static string GetDataByFiledName(WindData wd, string strFiledName)
        {
            if (null == wd)
                return string.Empty;
            if (wd.errorCode != 0)
                return string.Empty;
            if (null == wd.fieldList || null == wd.data || !(wd.data is object[,]))
                return string.Empty;
            int nFieldIndex = -1;
            for (int i = 0; i < wd.fieldList.Length; i++)
            {
                if (0 == string.Compare(wd.fieldList[i], strFiledName, true))
                {
                    nFieldIndex = i;
                    break;
                }
            }

            if (-1 == nFieldIndex)
                return string.Empty;

            object[,] obj = (object[,])wd.data;

            return obj[nFieldIndex, 0].ToString();
        }
    }

    //回调函数委托，用于wsq订阅是返回推送数据
    public delegate void WindCallback(WindData wd);

    public class WindAPI
    {
        private class WSQReq
        {
            public WindData rdata { get; set; }
            public WindCallback callback { get; set; }
            public bool updateAll { get; set; }
        }

        WindDataCOM wdc = null;
        Dictionary<ulong, WSQReq> ReqList = new Dictionary<ulong, WSQReq>();
        //构造函数
        public WindAPI()
        {
            try
            {
                wdc = new WindDataCOM();
                wdc.enableAsyn(1);
                wdc.stateChanged += new _DWindDataCOMEvents_stateChangedEventHandler(ReqHandler);
            }
            catch{
                throw new Exception("WindMatlab.ocx组件未注册");
            }
        }

        private int getCodeIndex(WindData rd,string codeName) {
            if (rd != null) {
                string[] clist = rd.codeList;
                int length = clist.Length;
                for (int i = 0; i < length; i++) {
                    if (codeName == clist[i])
                        return i;
                }
            }
            return -1;
        }

        private int getFieldIndex(WindData rd, string fieldName)
        {
            if (rd != null)
            {
                string[] flist = rd.fieldList;
                int length = flist.Length;
                for (int i = 0; i < length; i++)
                {
                    if (fieldName == flist[i])
                        return i;
                }
            }
            return -1;
        }

        private WindData setData(WindData rd,object odata,int codeIndex, int fieldIndex,int dataIndex) {
            object tmpdata = rd.data;
            if (codeIndex >= rd.codeList.Length || fieldIndex >= rd.fieldList.Length)
            {
                throw new Exception("数组越界");
            }
            if (rd.data is double[])
                ((double[])tmpdata)[fieldIndex + codeIndex * rd.fieldList.Length] = ((double[])odata)[dataIndex];
            else if (rd.data is int[])
                ((int[])tmpdata)[fieldIndex + codeIndex * rd.fieldList.Length] = ((int[])odata)[dataIndex];
            else if (rd.data is String[])
                ((String[])tmpdata)[fieldIndex + codeIndex * rd.fieldList.Length] = ((String[])odata)[dataIndex];
            else if (rd.data is DateTime[])
                ((DateTime[])tmpdata)[fieldIndex + codeIndex * rd.fieldList.Length] = ((DateTime[])odata)[dataIndex];
            else if (rd.data is Int64[])
                ((Int64[])tmpdata)[fieldIndex + codeIndex * rd.fieldList.Length] = ((Int64[])odata)[dataIndex];
            else if (rd.data is object[])
                ((object[])tmpdata)[fieldIndex + codeIndex * rd.fieldList.Length] = ((object[])odata)[dataIndex];
            rd.data = tmpdata;
            return rd;
        }

        private WindData updataReqData(WindData rd, object ocodes, object ofields,object odata) {
            if (ocodes is string) {
                if (ofields is string) {
                    rd = setData(rd, odata, getCodeIndex(rd, (string)ocodes), getFieldIndex(rd, (string)ofields),0);
                }
                else if (ofields is string[]) {
                    string[] flist = (string[])ofields;
                    for (int i = 0; i < flist.Length; i++) {
                        rd = setData(rd, odata, getCodeIndex(rd, (string)ocodes), getFieldIndex(rd, flist[i]), i);
                    }
                }
            }
            else if (ocodes is string[]) {
                string[] clist = (string[])ocodes;
                for (int j = 0; j < clist.Length; j++) { 
                    if(ofields is string)
                        rd = setData(rd, odata, getCodeIndex(rd, clist[j]), getFieldIndex(rd, (string)ofields), j);
                    else if (ofields is string[])
                    {
                        string[] flist = (string[])ofields;
                        for (int k = 0; k < flist.Length; k++)
                        {
                            rd = setData(rd, odata, getCodeIndex(rd, clist[j]), getFieldIndex(rd, flist[k]), j*flist.Length+k);
                        }
                    }
                }
            }
            return rd;
        }

        private void ReqHandler(int state, ulong rid, int err) {
            if (ReqList.ContainsKey(rid)) {
                WSQReq wr = ReqList[rid];
                if (state == 1 && err == 0) {
                    int reqState,errCode;
                    object ocodes, ofields, otimes, odata;
                    odata = wdc.readdata(rid, out ocodes, out ofields, out otimes, out reqState, out errCode);
                    if(wr.updateAll)
                        wr.callback(updataReqData(wr.rdata,ocodes,ofields,odata));
                    else
                        wr.callback(formatWD(ocodes, ofields, otimes, odata, errCode));
                }
                else if (err != 0) {
                    WindData wd = new WindData();
                    wd.errorCode = err;
                    wd.fieldList = new String[] { "ErrorMessage" };
                    wd.data = new String[] { getErrorMsg(err) };
                    wr.callback(wd);
                }
            }
            return;
        }

        //Wind接口启动函数
        public int start(string option1 = "", string option2 = "", int timeout = 5000)
        {
            return wdc.start_csharp(option1, option2, timeout);
        }

        //Wind接口终止函数
        public void stop()
        {
            wdc.stop();
        }

        //判断连接状态
        public bool isconnected()
        {
            int ret = wdc.getConnectionState(out ret);
            if (ret == 0)
                return false;
            return true;
        }

        //取消订阅函数，目前仅wsq支持订阅
        public void cancelRequest(ulong requestID)
        {
            if (requestID == 0) {
                foreach (ulong rid in ReqList.Keys) {
                    wdc.cancelRequest(rid);
                }
                ReqList.Clear();
            }
            if (ReqList.ContainsKey(requestID))
            {
                wdc.cancelRequest(requestID);
                ReqList.Remove(requestID);
            }
            return;
        }

        //将windCode数组转化为string用于各个数据函数传参
        public string formatCodes(string[] windCodes) {
            string ret = "";
            int length = windCodes.Length;
            for (int i = 0; i < length - 1; i++)
            {
                ret += windCodes[i];
                ret += ",";
            }
            ret += windCodes[length - 1];
            return ret;
        }

        //将field数组转化为string用于各个数据函数传参
        public string formatFields(string[] fields) {
            string ret = "";
            int length = fields.Length;
            for (int i = 0; i < length - 1; i++)
            {
                ret += fields[i];
                ret += ",";
            }
            ret += fields[length - 1];
            return ret;
        }

        //将option数组转化为string用于各个数据函数传参
        public string formatOptions(string[] options) {
            string ret = "";
            int length = options.Length;
            for (int i = 0; i < length - 1; i++)
            {
                ret += options[i];
                ret += ";";
            }
            ret += options[length - 1];
            return ret;
        }

        private void checkConnection() {
            //if (!isconnected())
            //    throw new Exception("WindAPI未连接或者连接终端");
        }

        //多值函数wsd，获得日期序列
        public WindData wsd(string windCodes, string fields, string startTime, string endTime, string options)
        {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errorCode;
            odata = wdc.wsd_syn(windCodes, fields, startTime, endTime, options, out ocodes, out ofields, out otimes, out errorCode);
            return formatWD(ocodes, ofields, otimes, odata, errorCode);
        }

        public WindData wsd(string windCodes, string fields, DateTime startTime, DateTime endTime, string options)
        {
            return wsd(windCodes, fields, startTime.ToString("yyyy-MM-dd"), endTime.ToString("yyyy-MM-dd"), options);
        }

        //多值函数wsi，获得分钟序列
        public WindData wsi(string windCodes, string fields, string startTime, string endTime, string options)
        {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errCode;
            odata = wdc.wsi_syn(windCodes, fields, startTime, endTime, options, out ocodes, out ofields, out otimes, out errCode);
            return formatWD(ocodes, ofields, otimes, odata, errCode);
        }

        public WindData wsi(string windCodes, string fields, DateTime startTime, DateTime endTime, string options)
        {
            return wsi(windCodes, fields, startTime.ToString("yyyy-MM-dd HH:mm:ss"), endTime.ToString("yyyy-MM-dd HH:mm:ss"), options);
        }

        //多值函数wst，获得日内跳价
        public WindData wst(string windCodes, string fields, string startTime, string endTime, string options) {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errCode;
            odata = wdc.wst_syn(windCodes, fields, startTime, endTime, options, out ocodes, out ofields, out otimes, out errCode);
            return formatWD(ocodes, ofields, otimes, odata, errCode);
        }

        public WindData wst(string windCodes, string fields, DateTime startTime, DateTime endTime, string options){
            return wst(windCodes, fields, startTime.ToString("yyyy-MM-dd HH:mm:ss"), endTime.ToString("yyyy-MM-dd HH:mm:ss"), options);
        }

        //多值函数wss，获得历史快照
        public WindData wss(string windCodes, string fields, string options) {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errCode;
            odata = wdc.wss_syn(windCodes,fields,options,out ocodes,out ofields,out otimes,out errCode);
            return formatWD(ocodes, ofields, otimes, odata, errCode);
        }

        //多值函数wsq，获得实时行情，不带WindCallback传参为取快照
        //带WindCallback传参为订阅，数据通过fnCallback返回
        public WindData wsq(string windCodes, string fields,string options) {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errCode;
            odata = wdc.wsq_syn(windCodes, fields, options, out ocodes, out ofields, out otimes, out errCode);
            return formatWD(ocodes, ofields, otimes, odata, errCode);
        }

        public ulong wsq(string windCodes, string fields, string options, WindCallback wc,bool updateAll = true) {
            checkConnection();
            if (wc == null)
            {
                throw new Exception("订阅回调函数不能为空。");
            }
            string tmpoptions = options+";REALTIME=Y";
            int errCode;
            ulong rid = wdc.wsq(windCodes, fields, tmpoptions, out errCode);
            WSQReq wr = new WSQReq();
            wr.callback = wc;
            wr.rdata = wsq(windCodes, fields, options);
            wr.updateAll = updateAll;
            ReqList.Add(rid, wr);
            return rid;
        }

        public WindData wsqtd(string windCodes, string fields, string options)
        {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errCode;
            odata = wdc.wsqtd_syn(windCodes, fields, options, out ocodes, out ofields, out otimes, out errCode);
            return formatWD(ocodes, ofields, otimes, odata, errCode);
        }

        public ulong wsqtd(string windCodes, string fields, string options, WindCallback wc, bool updateAll = true)
        {
            checkConnection();
            if (wc == null)
            {
                throw new Exception("订阅回调函数不能为空。");
            }
            string tmpoptions = options + ";REALTIME=Y";
            int errCode;
            ulong rid = wdc.wsqtd(windCodes, fields, tmpoptions, out errCode);
            WSQReq wr = new WSQReq();
            wr.callback = wc;
            wr.rdata = wsqtd(windCodes, fields, options);
            wr.updateAll = updateAll;
            ReqList.Add(rid, wr);
            return rid;
        }

        //多值函数wset，获得指定数据集
        public WindData wset(string reportName,string options) {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errCode;
            odata = wdc.wset_syn(reportName,options, out ocodes, out ofields, out otimes, out errCode);
            return formatWD(ocodes, ofields, otimes, odata, errCode);
        }

        public WindData edb(string windCodes, string startTime, string endTime, string options)
        {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errorCode;
            odata = wdc.edb_syn(windCodes, startTime, endTime, options, out ocodes, out ofields, out otimes, out errorCode);
            return formatWD(ocodes, ofields, otimes, odata, errorCode);
        }

        //组合报表函数
        public WindData wpf(string portfolioName, string viewName, string options) {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errCode;
            odata = wdc.wpf_syn(portfolioName, viewName, options,out ocodes,out ofields,out otimes,out errCode);
            return formatWD(ocodes,ofields,otimes,odata,errCode);
        }

        //组合上传函数
        public WindData wupf(string portfolioName, string tradeDate, string windCodes, string quantity, string costPrice, string options) {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errCode;
            odata = wdc.wupf_syn(portfolioName, tradeDate, windCodes, quantity, costPrice, options, out ocodes, out ofields, out otimes, out errCode);
            return formatWD(ocodes,ofields,otimes,odata,errCode);
        }

        //证劵筛选函数
        public WindData weqs(string planName,string options = "") {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errCode;
            odata = wdc.weqs_syn(planName, options, out ocodes, out ofields, out otimes, out errCode);
            return formatWD(new string[]{""}, ofields, otimes, odata, errCode);
        }

        //日历日、工作日、交易日的日期序列函数
        public WindData tdays(string startTime, string endTime, string options) {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errCode;
            odata = wdc.tdays_syn(startTime,endTime, options, out ocodes, out ofields, out otimes, out errCode);
            return formatWD(ocodes, ofields,new double[]{DateTime.Now.AddYears(1900).ToOADate()}, odata, errCode);
        }

        public WindData tdays(DateTime startTime, DateTime endTime, string options) {
            return tdays(startTime.ToString("yyyy-MM-dd"), endTime.ToString("yyyy-MM-dd"), options);
        }

        //日历日、工作日、交易日的日期偏移计算
        public WindData tdaysoffset(string startTime, int offset, string options) {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errCode;
            odata = wdc.tdaysoffset_syn(startTime, offset, options, out ocodes, out ofields, out otimes, out errCode);
            return formatWD(ocodes, ofields, otimes, odata, errCode);
        }

        public WindData tdaysoffset(DateTime startTime, int offset, string options) {
            return tdaysoffset(startTime.ToString("yyyy-MM-dd"), offset, options);
        }

        //日历日、工作日、交易日的日期天数计算
        public WindData tdayscount(string startTime, string endTime, string options) {
            checkConnection();
            object ocodes, ofields, otimes, odata;
            int errCode;
            odata = wdc.tdayscount_syn(startTime, endTime, options, out ocodes, out ofields, out otimes, out errCode);
            return formatWD(ocodes, ofields, otimes, odata, errCode);
        }

        public WindData tdayscount(DateTime startTime, DateTime endTime, string options)
        {
            return tdayscount(startTime.ToString("yyyy-MM-dd"), endTime.ToString("yyyy-MM-dd"), options);
        }

        //获取错误码相应的错误信息
        public string getErrorMsg(int errCode) {
            return (string)wdc.getErrorMsg(errCode, 1)+" "+(string)wdc.getErrorMsg(errCode,0);
        }

        //交易账号登陆
        public WindData tlogon(string brokerID, string departmentID, string accountID, string password, string accountType,string options = "") {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.tLogon(brokerID, departmentID, accountID, password, accountType, options, out ofields, out errCode);
            return formatWD(null,ofields,null,odata,errCode);
        }

        //交易账号登出
        public WindData tlogout() {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.tLogout("",out ofields,out errCode);
            return formatWD(null, ofields, null, odata, errCode);
        }

        public WindData tlogout(int logonID) {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.tLogout("LogonID="+Convert.ToString(logonID), out ofields, out errCode);
            return formatWD(null, ofields, null, odata, errCode);
        }

        //下单
        public WindData torder(string windCodes, string tradeSide, string orderPrice, string orderVolume, string options) {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.tSendOrder(windCodes, tradeSide, orderPrice, orderVolume, options, out ofields,out errCode);
            return formatWD(null, ofields, null, odata, errCode);
        }

        public WindData torder(string windCodes, string tradeSide, double orderPrice, int orderVolume, string options) {
            return torder(windCodes,tradeSide,Convert.ToString(orderPrice),Convert.ToString(orderVolume),options);
        }

        //撤单
        public WindData tcancel(string orderNumber, string options = "") {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.tCancelOrder(orderNumber, options, out ofields, out errCode);
            return formatWD(null, ofields, null, odata, errCode);
        }

        //交易情况查询
        public WindData tquery(string qryCode, string options = "") {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.tQuery(qryCode, options, out ofields, out errCode);
            return formatWD(null, ofields, null, odata, errCode);
        }

        //回测开始
        public WindData bktstart(string strategyName, string startDate, string endDate, string options = "")
        {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.bktstart(strategyName, startDate, endDate, options, out ofields, out errCode);
            return formatWD(null, ofields, null, odata, errCode);    
        }

        //回测查询
        public WindData bktquery(string qrycode, string qrytime, string options = "")
        {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.bktquery(qrycode, qrytime, options, out ofields, out errCode);
            return formatWD(null, ofields, null, odata, errCode); 
        }

        //回测下单
        public WindData bktorder(string tradeTime, string securityCode, string tradeSide, string tradeVol, string options = "")
        {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.bktorder(tradeTime, securityCode, tradeSide, tradeVol, options, out ofields, out errCode);
            return formatWD(null, ofields, null, odata, errCode); 
        }

        //回测结束
        public WindData bktend(string options = "") {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.bktend(options,out ofields,out errCode);
            return formatWD(null, ofields, null, odata, errCode); 
        }

        //查看回测状态
        public WindData bktstatus(string options )
        {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.bktstatus(options, out ofields, out errCode);
            return formatWD(null, ofields, null, odata, errCode); 
        }

        //回测概要
        public WindData bktsummary(string BktID, string View,string options = "") {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.bktsummary(BktID, View, options, out ofields, out errCode);
            return formatWD(null, ofields, null, odata, errCode);  
        }

        //回测删除
        public WindData bktdelete(string BktID, string options = "") {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.bktdelete(BktID, options, out ofields, out errCode);
            return formatWD(null, ofields, null, odata, errCode);  
        }

        //返回策略列表
        public WindData bktstrategy(string options) {
            checkConnection();
            object ofields, odata;
            int errCode;
            odata = wdc.bktstrategy(options, out ofields, out errCode);
            return formatWD(null, ofields, null, odata, errCode);
        }

        //统一处理返回值
        private WindData formatWD(object ocodes, object ofields, object otimes, object odata, int errorCode)
        {
            WindData wd = new WindData();
            wd.errorCode = errorCode;
            if (ocodes == null && ofields == null && otimes == null)
                return wd;
            if(ocodes != null)
                wd.codeList = (string[])ocodes;
            if(ofields != null)
                wd.fieldList = (string[])ofields;
            if (otimes != null)
            {
                double[] tmpTimes = (double[])otimes;
                int length = tmpTimes.Length;
                wd.timeList = new DateTime[length];
                for (int i = 0; i < length; i++)
                    wd.timeList[i] = DateTime.FromOADate(tmpTimes[i]).AddYears(-1900);
            }
            wd.data = odata;
            return wd;
        }
    }
}
