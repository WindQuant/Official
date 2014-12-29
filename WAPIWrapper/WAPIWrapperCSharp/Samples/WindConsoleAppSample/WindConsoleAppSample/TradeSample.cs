using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WAPIWrapperCSharp;
using WindCommon;

namespace WindConsoleAppSample
{
    class TradeSample
    {
        public void DoIt()
        {
            string strErrorMsg;
            WindAPI w = new WindAPI();
            
            //登录WFT
            int nRetCode = w.start();
            if (0 != nRetCode)//登录失败
            {
                strErrorMsg = w.getErrorMsg(nRetCode);
                Console.Write(strErrorMsg);
                return;
            }

            //登录账号
            Console.Write("\r\ntlogon……");
            WindData wd = w.tlogon("0000", "0", "w088801001", "******", "SHSZ");
            string strLogonId = wd.GetLogonId();

            //下单浦发银行
            wd = w.torder("600000.SH", "Buy", "12.0", "200", "OrderType=LMT;LogonID=" + strLogonId);//单账户登录可以不指定LogonId=1
            //获取下单ID
            string strRequestID1 = wd.GetOrderRequestID();
            Console.WriteLine("RequestID=" + strRequestID1);

            //下单白云机场
            wd = w.torder("600004.SH", "Buy", "12.00", "300", "OrderType=LMT;LogonID=" + strLogonId);//单账户登录可以不指定LogonId=1
            //获取下单ID
            string strRequestID2 = wd.GetOrderRequestID();

            //查询
            Console.WriteLine("query……");
            wd = w.tquery("Order", "RequestID=" + strRequestID1);
            //获取浦发银行OrderNumber
            string strOrderNumber = wd.GetOrderNumber();

            //浦发银行撤单
            Console.WriteLine("cancel……");
            wd = w.tcancel(strOrderNumber);

            //再次查询
            Console.WriteLine("query after cancel……");
            wd = w.tquery("Order");
            string strQueryInfoAfter = WindDataMethod.WindDataToString(wd, "tquery");
            Console.Write(strQueryInfoAfter);

            //登出
            Console.WriteLine("tlogout……");
            w.tlogout();

            //退出WindAPI
            w.stop();
        }

        private string GetLogonId(WindData wd)
        {
            return GetDataByFiledName(wd, "LogonID");
        }

        private string GetRequestID(WindData wd)
        {
            return GetDataByFiledName(wd, "RequestID");
        }

        private string GetOrderNumber(WindData wd)
        {
            return GetDataByFiledName(wd, "OrderNumber");
        }

        private string GetDataByFiledName(WindData wd, string strFiledName)
        {
            if (wd.errorCode != 0)
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
}
