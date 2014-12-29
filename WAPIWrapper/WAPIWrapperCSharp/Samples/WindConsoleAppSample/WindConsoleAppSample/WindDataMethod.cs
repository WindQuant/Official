using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WAPIWrapperCSharp;

namespace WindCommon
{
    class WindDataMethod
    {
        public static string WindDataToString(WindData wd, string strFuncName)
        {
            if (null == wd)
                return string.Empty;

            string s = string.Empty;
            string str = string.Empty;

            //请求出错，获取错误信息
            if (wd.errorCode != 0)
            {
                s += wd.GetErrorMsg() + Environment.NewLine;

                return s;
            }

            if (null != wd.codeList)
            {
                s += "Windcode = " + Environment.NewLine;
                foreach (String code in wd.codeList)
                    s += "  " + code + "\t";
                s += Environment.NewLine;
            }
            if (null != wd.fieldList)
            {
                s += "Indicators = " + Environment.NewLine;
                foreach (String field in wd.fieldList)
                    s += "  " + field + "\t";
                s += Environment.NewLine;
            }
            if (null != wd.timeList)
            {
                s += "Times = " + Environment.NewLine;
                foreach (DateTime time in wd.timeList)
                    s += "  " + time.ToString() + "\t";
                s += Environment.NewLine;
            }

            s += "Data = " + Environment.NewLine;
            object getData = wd.getDataByFunc(strFuncName, false);
            if (getData is object[,])//转化为2维数组
            {
                object[,] odata = (object[,])getData;

                switch (strFuncName)
                {
                    case "wsd":
                        {
                            string strColHead = string.Empty;
                            strColHead += "Time" + "\t\t";
                            foreach (String field in wd.fieldList)
                                strColHead += field + "\t";
                            strColHead += Environment.NewLine;

                            s = strColHead;

                            int nTimeLength = wd.timeList.Length;
                            int nCodeLength = wd.codeList.Length;
                            int nFieldLength = wd.fieldList.Length;
                            for (int i = 0; i < nTimeLength; i++)
                            {
                                s += wd.timeList[i].ToString("yyyy-MM-dd") + "\t";
                                for (int j = 0; j < nCodeLength * nFieldLength; j++)
                                {
                                    str = odata[i, j].ToString();
                                    s += str + "\t";
                                }
                                s += Environment.NewLine;
                            }
                        }
                        break;
                    case "wsi":
                    case "wst":
                        {
                            string strColHead = string.Empty;
                            bool bNeedOutputTime = true;
                            foreach (String field in wd.fieldList)
                            {
                                if (field == "time")
                                {
                                    strColHead += field + "\t\t\t";
                                    bNeedOutputTime = false;
                                }
                                else
                                    strColHead += field + "\t";
                            }
                            if (bNeedOutputTime)
                                strColHead = "time" + "\t\t\t" + strColHead;
                            strColHead += Environment.NewLine;

                            s = strColHead;

                            int nTimeLength = wd.timeList.Length;
                            int nCodeLength = wd.codeList.Length;
                            int nFieldLength = wd.fieldList.Length;
                            for (int i = 0; i < nTimeLength; i++)
                            {
                                if (bNeedOutputTime)
                                {
                                    s += wd.timeList[i].ToString() + "\t";
                                }

                                for (int j = 0; j < nCodeLength * nFieldLength; j++)
                                {
                                    str = odata[i, j].ToString();
                                    s += str + "\t";
                                }
                                s += Environment.NewLine;
                            }
                        }
                        break;
                    case "wsq":
                    case "wss":
                    case "wset":
                    case "wpf":
                        {
                            string strColHead = string.Empty;
                            foreach (String field in wd.fieldList)
                            {
                                strColHead += field + "\t";
                            }

                            s = strColHead + Environment.NewLine;

                            int nTimeLength = wd.timeList.Length;
                            int nCodeLength = wd.codeList.Length;
                            int nFieldLength = wd.fieldList.Length;
                            for (int i = 0; i < nCodeLength; i++)
                            {
                                if (i > 100)
                                    break;
                                s += wd.codeList[i] + "\t";
                                for (int j = 0; j < nTimeLength * nFieldLength; j++)
                                {
                                    if (null != odata[i, j])
                                    {
                                        str = odata[i, j].ToString();
                                        s += str + "\t";
                                    }
                                }
                                s += Environment.NewLine;
                            }
                        }
                        break;
                    default:
                        {
                            foreach (object o in odata)
                            {
                                str = o.ToString();
                                s += str + "\t";
                            }
                            s += Environment.NewLine;
                        }
                        break;
                }
            }
            else if (getData is object[])//一维数组
            {
                object[] odata = (object[])getData;
                foreach (object o in odata)
                {
                    str = o.ToString();
                    s += str + Environment.NewLine;
                }
            }
            else//简单对象
            {
                s += getData.ToString() + Environment.NewLine;
            }

            return s;
        }
    }
}
