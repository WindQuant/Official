\name{WindR-package}
\alias{WindR-package}
\alias{WindR}
\docType{package}
\title{
Financial Data API of Wind Information
}
\description{
   WindR is an addin tool to retrieve financial data from Wind Information.\cr
   \cr
   WindR Methods:\cr
	\cr
   Start & Stop:\cr
       \link{w.start}						- start Wind Data API, login to Wind Server.\cr
       \link{w.stop}						- stop Wind Data API.\cr
       \link{w.menu}						- toggle the toolbar; open the guide dialog. Please use this command to create other commands.\cr
       \link{w.isconnected}			- check if the windR ok?\cr
			\cr
   Data Retrieve Functions:\cr
       \link{w.wsd}             - retrieve Wind daily data\cr
       \link{w.wsi}             - retrieve Wind inter-day data\cr
       \link{w.wst}             - retrieve Wind tick data\cr
       \link{w.wss}             - retrieve Wind snapshot data\cr
       \link{w.wsq}             - retrieve Wind quote data             \cr
       \link{w.wsqtd}           - retrieve Wind quote data             \cr
       \link{w.wset}            - retrieve some base info about market \cr
       \link{w.weqs}            - retrieve stocks by custom filter \cr
       \link{w.wpf}             - retrieve portfolio Information \cr
       \link{w.wupf}            - upload portfolio Information \cr
       \link{w.tdays}           - retrieve valid days between two days \cr
       \link{w.tdaysoffset}     - retrieve a day based on the input day \cr
       \link{w.tdayscount}      - retrieve duration between two days   \cr  
       \cr
       \link{w.tlogon}      		- logon into trading systems   \cr  
       \link{w.tlogout}      		- logout from trading systems   \cr  
       \link{w.torder}      		- send orders to trading systems   \cr  
       \link{w.tcovered}      		- send covered(lock/unlock) to trading systems   \cr  
       \link{w.tcancel}      		- cancel tradeing orders   \cr  
       \link{w.tquery}      		- query capitial,position,order,trade,account,department or logonid   \cr  
   tools Function \cr
       \link{w.asDateTime}      - from datetime double to DateTime or Date \cr
       \link{w.clearAllRequest} - clear All request \cr
       \link{w.cancelRequest}   - Cancel a request \cr
       \link{w.demoCallback}    - a demo user callback function for w.wsq.\cr
}
\details{
\tabular{ll}{
Package: \tab WindR\cr
Type: \tab Package\cr
Version: \tab 1.1.0\cr
Date: \tab 2013-08-04\cr
License: \tab It is Free. \cr
}


}
\author{
Shanghai Wind Information Co., Ltd \cr
\cr
Maintainer: Honghai Zhu \email{hhzhu@wind.com.cn}\cr
}
\examples{
library(WindR)
w.start()
w.wsd("600000.SH","open,high,low,close","2013-04-04","2013-05-04")
w.wss("000001.SZ","sec_name,trade_code")
w.wst("600000.SH","open,high,low,last","2013-05-02 09:00:00","2013-05-02 09:48:24")
w.wsi("600000.SH","low,amt","2013-05-02 09:00:00","2013-05-04 09:50:00")
w.wsq("600000.SH","rt_last,rt_last_amt,rt_vol,rt_ask1,rt_ask2,rt_bid1,rt_bid2")
w.wsqtd("600000.SH","rt_last,rt_last_amt,rt_vol,rt_ask1,rt_ask2,rt_bid1,rt_bid2")
w.menu("wsd")
w.tdays(Sys.time()-100*3600*24);
w.tdayscount(Sys.time()-100*3600*24);
w.tdaysoffset(-100);
w.wset('IndexConstituent','date=20130518;windcode=000300.SH;field=date,sec_name,i_weight')   
}