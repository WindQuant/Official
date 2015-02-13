\name{w.wst}
\alias{w.wst}
\title{
Retrieve Wind tick data
}
\description{
   WST is used to obtain snapshot data on ten bids and ten asks, as well as intraday transaction data.\cr
   To show the guide dialog, please input w.menu("wst"). \cr
   data <- w.wst(windcodes,windfields,starttime,endtime,option)\cr
   \cr
   Description:\cr
       windcodes      the Wind-code, like "600000.SH", only one security allowed.\cr
       windfields     the fields, like "OPEN,CLOSE,HIGH".\cr
       starttime      the start time, like "20130502 09:30:00".\cr
       endtime        the end time, like "20120502 10:30:00"\cr

       \cr
       $Data	         the return result, a data.frame.\cr
       $Code           the code of the data.\cr
       $ErrorCode	     the error ID (0 is OK).\cr
}
\usage{
w.wst(codes, fields, beginTime, endTime, options = "")
}
\examples{
library(WindR)
w.start()
w.wst("600000.SH","last,volume,amt,bid1,bsize1,ask1,asize1","20130502 09:30:00","20130502 10:30:00")  
}
\keyword{ wst }
