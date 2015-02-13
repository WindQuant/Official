\name{w.tspecial}
\alias{w.tspecial}
\title{
Send orders to trading systems
}
\description{
   tspecial is used to send special order(such as cover lock/unlock, ETF Subscription/Redemption) to trading systems.\cr
   To show the guide dialog, please input w.menu("torder").\cr
   data <- w.tspecial(SecurityCode, TradeSide, OrderVolume, ...)\cr
   \cr
   Description:\cr
        SecurityCode      Security Codes.\cr
        TradeSide   	  0/lock; 1/unlock;.\cr
        OrderVolume       Ordering Volumes.\cr
        
   Options:\cr
   			LogonID           Logon ID.\cr
             
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $ErrorCode	   the error ID (0 is OK).\cr   
}
\usage{
w.tspecial(SecurityCode, TradeSide, OrderVolume, ...)
}
\examples{
library(WindR)
w.start()

#Please firstly create a filter named myKDJ.
#w.tspecial('600104.SH, 'lock', 100)   
}
\keyword{ tspecial }
