using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WAPIWrapperCSharp;

namespace WindConsoleAppSample
{
    class RBreakSample
    {
        public void DoIt()
        {
            WindAPI w = new WindAPI();
            w.start();

            //取昨天的最高、最低、收盘
            double dLastHigh = 0; double dLastLow = 0; double dLastClose = 0;
            WindData wd = w.wss("600999.SH", "high,low,close", "tradeDate=20141202;priceAdj=U;cycle=D");
            //WindData wd = w.wss("601000.SH", "high,low,close", "tradeDate=20141202;priceAdj=U;cycle=D");
            //WindData wd = w.wss("M1309.DCE", "high,low,close", "tradeDate=20130411;priceAdj=U;cycle=D");
            double[,] lastPriceData = (double[,])wd.getDataByFunc("wss", true);
            if (null == lastPriceData)
                return;
            dLastHigh = lastPriceData[0, 0];
            dLastLow = lastPriceData[0, 1];
            dLastClose = lastPriceData[0, 2];

            //计算出6个价位
            double dSsetup = dLastHigh + 0.35 * (dLastClose - dLastLow);//观察卖出价
            double dSenter = (1.07 / 2) * (dLastHigh + dLastLow) - 0.07 * dLastLow;//反转卖出价
            double dBenter = (1.07 / 2) * (dLastHigh + dLastLow) - 0.07 * dLastHigh;//反转买入价
            double dBsetup = dLastLow - 0.35 * (dLastHigh - dLastClose); //观察买入价
            double dBbreak = dSsetup + 0.25 * (dSenter - dBsetup); //突破买入价
            double dSbreak = dBsetup - 0.205 * (dSenter - dBsetup); //突破卖出价

            //某天的交易数据
            WindData wdWSI = w.wsi("600999.SH", "high,low,close", "2014-12-03 09:30:00", "2014-12-03 15:00:00", "");
            //WindData wdWSI = w.wsi("601000.SH", "high,low,close", "2014-12-03 09:30:00", "2014-12-03 15:00:00", "");
            //WindData wdWSI = w.wsi("M1309.DCE", "high,low,close", "2013-04-12 09:30:00", "2013-04-12 15:00:00", "");
            double[,] curPriceData = (double[,])wdWSI.getDataByFunc("wsi", true);
            if (null == curPriceData)
                return;
            int nTimeCount = curPriceData.GetLength(0);
            if (nTimeCount < 0)
                return;
            int nPriceDim = curPriceData.GetLength(1);
            if (nPriceDim < 3)
                return;
            //策略初值，1表示做多，-1表示做空，0表示无操作
            List<int> curHoldList = new List<int>(nTimeCount);
            for (int i = 0; i < nTimeCount; i++)
            {
                curHoldList.Add(0);
            }
            double dCurHigh = curPriceData[0, 0];
            double dCurLow = curPriceData[0, 1];
            double dCurPrice = curPriceData[0, 2];
            for (int i = 0; i < nTimeCount; i++)
            {
                dCurHigh = (curPriceData[i, 0] > dCurHigh) ? curPriceData[i, 0] : dCurHigh;
                dCurLow = (curPriceData[i, 1] > dCurLow) ? curPriceData[i, 1] : dCurLow;
                dCurPrice = curPriceData[i, 2];

                //当前持仓
                int nCurHold = curHoldList.Sum();

                bool bCon1 = (dCurPrice > dBbreak) && (0 == nCurHold); //空仓做多条件
                bool bCon2 = (dCurPrice < dSbreak) && (0 == nCurHold); //空仓做空条件
                bool bCon3 = (nCurHold > 0) && (dCurHigh > dSsetup) && (dCurPrice < dSenter); //多单反转卖出条件:holding>0 and 今高>观察卖出价 and c<反转卖出价; 
                bool bCon4 = (nCurHold < 0) && (dCurLow < dBsetup) && (dCurPrice > dBenter); //空单反转买入条件:=holding<0 and 今低<观察买入价 and c>反转买入价;

                //交易
                if (bCon3)//多单反转
                {
                    curHoldList[i] = -1;//平多 //sell
                }
                else if (bCon2) //空单做空
                {
                    curHoldList[i] = -1; //sell
                }
                else if (bCon4) //空单反转
                {
                    curHoldList[i] = 1;//buy
                }
                else if (bCon1) //空仓做多
                {
                    curHoldList[i] = 1;//buy
                }
                else
                {
                    curHoldList[i] = 0;
                }
            }

            int nCurHole = curHoldList.Sum();
            //尾盘轧平
            if (1 == nCurHole && 1 == curHoldList.Last())
                curHoldList[nTimeCount - 1] = 0;
            else if (1 == nCurHole)
                curHoldList[nTimeCount - 1] = -1;
            else if (-1 == nCurHole && -1 == curHoldList.Last())
                curHoldList[nTimeCount - 1] = 0;
            else if (-1 == nCurHole)
                curHoldList[nTimeCount - 1] = 1;

            //统计收益
            double dProfit = 0;
            for (int i = 0; i < nTimeCount; i++)
            {
                dProfit += curPriceData[i, 2] * curHoldList[i];
            }

            Console.WriteLine("策略收益:" + dProfit);
            w.stop();
        }
    }
}
