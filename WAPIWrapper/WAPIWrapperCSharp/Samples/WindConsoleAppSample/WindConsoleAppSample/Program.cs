using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WAPIWrapperCSharp;
using WindCommon;

namespace WindConsoleAppSample
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Write("开始……");
            Console.ReadLine();

            //DoAPISameple();

            TradeSample trade = new TradeSample();
            trade.DoIt();

//             RBreakSample rbreakSample = new RBreakSample();
//             rbreakSample.DoIt();

            Console.WriteLine("End......");
            Console.ReadLine();
        }

        static void DoAPISameple()
        {
            WindAPI w = new WindAPI();
            w.start();

            //wset取沪深300指数成分
            //WindData wd = w.wset("IndexConstituent", "date=20141215;windcode=000300.SH");
            //OutputWindData(wd, "wset");

            WindData wd = w.wsd("600000.SH,600004.SH", "open", "2014-10-16", "2014-12-16", "");
            OutputWindData(wd, "wsd");

            w.stop();
        }

        static void OutputWindData(WindData wd, string strFuncName)
        {
            string s = WindDataMethod.WindDataToString(wd, strFuncName);
            Console.Write(s);
        }
    }
}
