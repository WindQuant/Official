\name{w.torder}
\alias{w.torder}
\title{
Send orders to trading systems
}
\description{
   torder is used to send orders to trading systems.\cr
   To show the guide dialog, please input w.menu("torder").\cr
   data <- w.torder(SecurityCode, TradeSide, OrderPrice, OrderVolume, ...)\cr
   \cr
   Description:\cr
        SecurityCode      Security Codes.\cr
        TradeSide   			1/buy; 2/short; 3/cover; 4/sell; 5/coverToday; 6/sellToday.\cr
        OrderPrice   			ordering Prices.\cr
        OrderVolume       Ordering Volumes.\cr
        
   Options:\cr
   			MarketType        0/SZ; 1/SZ; 2/OC; 6/HK; 7/CZC; 8/SHF; 9/DCE; 10/CFE;
   			OrderType         0/LMT; 1/BOC; 2/BOP; 3/ITC; 4/B5TC; 5/FOK; 6/B5TL;
   			HedgeType         for futures,0/SPEC;1/HEDG;
   			LogonID           Logon ID.\cr
             
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $ErrorCode	   the error ID (0 is OK).\cr   
}
\usage{
w.torder(SecurityCode, TradeSide, OrderPrice, OrderVolume, ...)
}
\examples{
library(WindR)
w.start()

#Please firstly create a filter named myKDJ.
#w.torder('600000.SH, 'buy', 9.8, 100)   
}
\keyword{ torder }
