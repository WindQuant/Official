\name{w.tcancel}
\alias{w.tcancel}
\title{
Cancel tradeing orders
}
\description{
   tcancel is used to cancel tradeing orders.\cr
   To show the guide dialog, please input w.menu("tcancel").\cr
   data <- w.tcancel(OrderNumber='',...)\cr
   \cr
   Description:\cr
        OrderNumber    the order numbers.\cr
	
	 Options:\cr
        LogonID        the logon IDs.\cr
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $ErrorCode	   the error ID (0 is OK).\cr   
}
\usage{
w.tcancel(OrderNumber,...)
}
\examples{
library(WindR)
w.start()

#Please firstly logon, send a order and query the ordernumber.
#w.tcancel(11)   
}
\keyword{ tcancel }
