\name{w.tcovered}
\alias{w.tcovered}
\title{
Send orders to trading systems
}
\description{
   tcovered is used to send covered(lock/unlock) to trading systems.\cr
   To show the guide dialog, please input w.menu("tcovered").\cr
   data <- w.tcovered(SecurityCode, TradeSide, OrderVolume, ...)\cr
   \cr
   Description:\cr
        SecurityCode      Security Codes.\cr
        TradeSide   			0/lock; 1/unlock;.\cr
        OrderVolume       Ordering Volumes.\cr
        
   Options:\cr
   			LogonID           Logon ID.\cr
             
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $ErrorCode	   the error ID (0 is OK).\cr   
}
\usage{
w.tcovered(SecurityCode, TradeSide, OrderVolume, ...)
}
\examples{
library(WindR)
w.start()

#Please firstly create a filter named myKDJ.
#w.tcovered('600104.SH, 'lock', 100)   
}
\keyword{ tcovered }
