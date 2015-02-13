\name{w.wsq}
\alias{w.wsq}
\title{
Retrieve Wind quote data
}
\description{
   WSQ is used to retrieve real-time market data. It can request a data directly or subscribe some data..\cr
   To show the guide dialog, please input w.menu("wsq").\cr
   \cr
   for requesting data directly£º\cr
   data<- w.wsq(windcodes,windfields,option,func=NULL)\cr
   \cr
   Description:\cr
        windcodes      the Wind-code, like "600000.SH,000001.SZ".\cr
        windfields     the fields, like "rt_last_vol,rt_ask1,rt_asize1".\cr
        func           the callback function, like function(data).  \cr
                       the data includes:$ErrorCode,$RequestID,$Field,$Code,$Time,$Data. \cr
                       $Data is a 3D array[Field,Code,Time].\cr
 	 \cr
 	    for func==NULL, the output is:\cr
        $Data	         the return result, a data.frame.\cr
        $Time          the time of the data.\cr
        $ErrorCode	   the error ID (0 is OK).\cr
      for func!=NULL, the output is£º\cr
      	$ErrorCode     the error ID (0 is OK).\cr
      	$RequestID     the id of this subscription. User can use w.cancelRequest(id) to cancel this subscription. \cr
   \cr
   \cr
   Please refer \link{w.demoCallback},\link{w.cancelRequest}
}
\usage{
w.wsq(codes, fields, options = "")
}
\examples{
library(WindR)
w.start()
w.wsq("600000.SH","rt_last,rt_last_amt,rt_vol,rt_ask1,rt_ask2,rt_bid1,rt_bid2")

#callback
data<-w.wsq("600000.SH","rt_last,rt_last_amt,rt_vol,rt_ask1,rt_ask2,rt_bid1,rt_bid2",func=w.demoCallback)
w.cancelRequest(data$RequestID);
}
\keyword{ wsq }
